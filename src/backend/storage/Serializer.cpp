#include "backend/storage/Serializer.hpp"
#include "backend/storage/Metadata.hpp"

#include <chrono>
#include <cstddef>
#include <functional>
namespace backend::storage {

void to_json(nlohmann::json& j, const Metadata& m) {
    j = nlohmann::json{{"version", m.version},
                       {"datetime", std::chrono::system_clock::to_time_t(m.datetime)},
                       {"sysFingerprint", m.sysFingerprint}};
}

void from_json(const nlohmann::json& j, Metadata& m) {
    j.at("version").get_to(m.version);
    j.at("datetime").get_to(m.datetime);
    j.at("sysFingerprint").get_to(m.sysFingerprint);
}
}  // namespace backend::storage

namespace nlohmann {
void adl_serializer<std::chrono::system_clock::time_point>::to_json(json& j, const std::chrono::system_clock::time_point& tp) {
    j = std::chrono::system_clock::to_time_t(tp);
}

void adl_serializer<std::chrono::system_clock::time_point>::from_json(const json& j, std::chrono::system_clock::time_point& tp) {
    std::time_t t = 0;
    j.get_to(t);
    tp = std::chrono::system_clock::from_time_t(t);
}
}  // namespace nlohmann
