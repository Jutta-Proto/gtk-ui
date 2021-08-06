#include "NfcCardReader.hpp"
#include "jutta_proto/JuttaCommands.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include <cassert>
#include <cctype>
#include <chrono>
#include <logger/Logger.hpp>
#include <memory>
#include <optional>
#include <cryptopp/config_int.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <spdlog/spdlog.h>
#include <sys/stat.h>

namespace backend {
NfcCardReader::NfcCardReader() {
    stateChangedDisp.connect(sigc::mem_fun(*this, &NfcCardReader::on_state_changed_notification_from_worker_thread));
    cardDetectedDisp.connect(sigc::mem_fun(*this, &NfcCardReader::on_card_detected_notification_from_worker_thread));
}

NfcCardReader::~NfcCardReader() {
    stop();
}

void NfcCardReader::set_state(NfcCardReaderState newState) {
    if (newState != state) {
        state = newState;
        stateChangedDisp.emit();
    }
}

void NfcCardReader::start() {
    assert(!mainThread);
    SPDLOG_DEBUG("Starting NFC card detection...");
    mainThread = std::make_optional<std::thread>(&NfcCardReader::run, this);
}

void NfcCardReader::stop() {
    if (!mainThread) {
        return;
    }
    SPDLOG_DEBUG("Stoping NFC card detection...");
    if (state == NfcCardReaderState::RUNNING) {
        set_state(NfcCardReaderState::STOPPING);
    }
    mainThread->join();
    mainThread = std::nullopt;
    SPDLOG_DEBUG("Stoped NFC card detection.");
    set_state(NfcCardReaderState::NOT_RUNNING);
}

void NfcCardReader::run() {
    set_state(NfcCardReaderState::RUNNING);
    SPDLOG_DEBUG("NFC card detection started.");
    while (state == NfcCardReaderState::RUNNING) {
        stackMutex.lock();
        // NOLINTNEXTLINE (hicpp-use-nullptr)
        if (!stack.empty() && (std::chrono::steady_clock::now() - lastChar) > std::chrono::seconds{1}) {
            stack.clear();
            SPDLOG_DEBUG("NFC card detection stack cleared.");
        }
        stackMutex.unlock();
        // Sleep for one second:
        for (size_t i = 0; i < 10 && state == NfcCardReaderState::RUNNING; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    SPDLOG_INFO("NFC card detection got canceled.");
}

NfcCardReader::NfcCardReaderState NfcCardReader::get_state() const {
    return state;
}

NfcCardReader::type_signal_state_changed NfcCardReader::signal_state_changed() {
    return m_signal_state_changed;
}

NfcCardReader::type_signal_card_detected NfcCardReader::signal_card_detected() {
    return m_signal_card_detected;
}

void NfcCardReader::on_new_char_input(char c) {
    if (std::isalnum(c)) {
        stackMutex.lock();
        stack.push(c);
        lastChar = std::chrono::steady_clock::now();
        if (stack.size() >= 8) {
            // For privacy reasons do not work/store the card id directly:
            std::string cardid = stack.pop();
            lastCardId = sha_256(cardid);
            stackMutex.unlock();
            SPDLOG_INFO("Card with ID '{}' detected.", lastCardId);
            cardDetectedDisp.emit();
        }
        stackMutex.unlock();
    }
}

const std::string NfcCardReader::sha_256(const std::string& s) {
    CryptoPP::SHA256 hash;
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    hash.Update(reinterpret_cast<const CryptoPP::byte*>(s.data()), s.length());
    std::string decoded;
    decoded.resize(hash.DigestSize());
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    hash.Final(reinterpret_cast<CryptoPP::byte*>(decoded.data()));
    CryptoPP::HexEncoder enc;
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    enc.Put(reinterpret_cast<const CryptoPP::byte*>(decoded.data()), decoded.size());
    enc.MessageEnd();
    std::string encoded;
    CryptoPP::lword l = enc.MaxRetrievable();
    if (l) {
        encoded.resize(l);
        // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
        enc.Get(reinterpret_cast<CryptoPP::byte*>(encoded.data()), encoded.size());
    }
    return encoded;
}

//-----------------------------Events:-----------------------------
void NfcCardReader::on_state_changed_notification_from_worker_thread() {
    // Emit the signal handler:
    m_signal_state_changed.emit(state);
}

void NfcCardReader::on_card_detected_notification_from_worker_thread() {
    // Emit the signal handler:
    m_signal_card_detected.emit(lastCardId);
}
}  // namespace backend
