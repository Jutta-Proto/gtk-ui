#include "context/backend/storage/Serializer.hpp"
#include "context/backend/formatted_text/Text.hpp"
#include "context/backend/formatted_text/color/HSL.hpp"
#include "context/backend/formatted_text/color/RGB.hpp"
#include "context/backend/ps/PaperSearchContext.hpp"
#include "context/backend/ps/PubMapVec.hpp"
#include "context/backend/requests/Feedback.hpp"
#include "context/backend/requests/Publication.hpp"
#include "context/backend/storage/Metadata.hpp"

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/zstd.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <functional>
#include <istream>

namespace ps {
void to_json(nlohmann::json& j, const PaperSeachResultCount& p) {
    j = nlohmann::json{{"parti", p.parti}, {"exposure", p.exposure}, {"outcome", p.outcome}};
}
void from_json(const nlohmann::json& j, PaperSeachResultCount& p) {
    j.at("parti").get_to(p.parti);
    j.at("exposure").get_to(p.exposure);
    j.at("outcome").get_to(p.outcome);
}

void to_json(nlohmann::json& j, const PubMapVec& p) {
    std::vector<nlohmann::json> t{};
    t.reserve(p.size());
    for (const std::shared_ptr<requests::Publication>& pub : p) {
        nlohmann::json pubJson;
        requests::to_json(pubJson, *pub);
        t.emplace_back(std::move(pubJson));
    }
    j = t;
}
void from_json(const nlohmann::json& j, PubMapVec& p) {
    for (const nlohmann::json& pubJson : j.get<std::vector<nlohmann::json>>()) {
        std::shared_ptr<requests::Publication> pub = std::make_shared<requests::Publication>();
        requests::from_json(pubJson, *pub);
        p.add(std::move(pub));
    }
}
}  // namespace ps

namespace formatted_text {
void to_json(nlohmann::json& j, const Text& f) {
    std::vector<nlohmann::json> t{};
    t.resize(f.get_format_vector().size());
    for (size_t i = 0; i < f.get_format_vector().size(); i++) {
        t[i] = nlohmann::json{{"start", f.get_format_vector()[i].start},
                              {"len", f.get_format_vector()[i].length},
                              {"col", f.get_format_vector()[i].color},
                              {"sty", f.get_format_vector()[i].style},
                              {"f_s", f.get_format_vector()[i].font_size}};
    }
    j = {{"text", f.get_text()}, {"format", t}};
}
void from_json(const nlohmann::json& j, Text& f) {
    j.at("text").get_to(f.get_text());
    f.get_format_vector().clear();
    for (const nlohmann::json& format : j.at("format").get<std::vector<nlohmann::json>>()) {
        f.get_format_vector().emplace_back(f.get_text(), format.at("start").get<size_t>(), format.at("len").get<size_t>(), format.at("col").get<std::optional<color::RGB>>(), format.at("sty").get<uint8_t>(), format.at("f_s").get<size_t>());
    }
}
}  // namespace formatted_text

namespace formatted_text::color {
void to_json(nlohmann::json& j, const RGB& rgb) {
    uint32_t tmp = rgb.r;
    tmp = (tmp << 8U) | rgb.g;
    tmp = (tmp << 8U) | rgb.b;
    j = tmp;
}
void from_json(const nlohmann::json& j, RGB& rgb) {
    uint32_t tmp = j.get<uint32_t>();
    rgb.b = tmp & 0xffU;
    tmp = tmp >> 8U;
    rgb.g = tmp & 0xffU;
    tmp = tmp >> 8U;
    rgb.r = tmp & 0xffU;
}
void to_json(nlohmann::json& j, const HSL& hsl) {
    j = nlohmann::json{{"h", hsl.h},
                       {"l", hsl.l},
                       {"s", hsl.s}};
}
void from_json(const nlohmann::json& j, HSL& hsl) {
    j.at("h").get_to(hsl.h);
    j.at("l").get_to(hsl.l);
    j.at("s").get_to(hsl.s);
}
}  // namespace formatted_text::color

namespace requests {
void to_json(nlohmann::json& j, const Feedback& f) {
    j = nlohmann::json{{"name", f.name},
                       {"liked", f.liked},
                       {"comment", f.comment},
                       {"timestamp", f.timestamp}};
}

void to_json(nlohmann::json& j, const Publication& p) {
    j = nlohmann::json{{"id", p.id},
                       {"likes", p.likes},
                       {"dislikes", p.dislikes},
                       {"hidden", p.hidden},
                       {"sourceDB", p.sourceDB},
                       {"title", p.title},
                       {"issn", p.issn},
                       {"fullTextUrl", p.fullTextUrl},
                       {"abstract", p.abstract},
                       {"langs", p.langs},
                       {"authors", p.authors},
                       {"feedbackList", p.feedbackList},
                       {"duplicatePapers", p.duplicatePapers},
                       {"pubDate", p.pubDate}};
}

void from_json(const nlohmann::json& j, Feedback& f) {
    j.at("name").get_to(f.name);
    j.at("liked").get_to(f.liked);
    j.at("comment").get_to(f.comment);
    j.at("timestamp").get_to(f.timestamp);
}

void from_json(const nlohmann::json& j, Publication& p) {
    j.at("id").get_to(p.id);
    j.at("likes").get_to(p.likes);
    j.at("dislikes").get_to(p.dislikes);
    j.at("hidden").get_to(p.hidden);
    j.at("sourceDB").get_to(p.sourceDB);
    j.at("title").get_to(p.title);
    j.at("issn").get_to(p.issn);
    j.at("fullTextUrl").get_to(p.fullTextUrl);
    j.at("abstract").get_to(p.abstract);
    j.at("langs").get_to(p.langs);
    j.at("authors").get_to(p.authors);
    j.at("feedbackList").get_to(p.feedbackList);
    j.at("duplicatePapers").get_to(p.duplicatePapers);
    j.at("pubDate").get_to(p.pubDate);
}
}  // namespace requests

namespace storage {

bool Serializer::read_in() {
    if (not this->compression) {
        std::ifstream i(this->filename);
        if (i.fail()) {
            SPDLOG_ERROR("Failed to read {}", this->filename.string());
            return false;
        }
        try {
            i >> this->js_int;
        } catch (nlohmann::json::parse_error& e) {
            SPDLOG_ERROR("Error parsing {}: {}", this->filename.string(), e.what());
            return false;
        }
        spdlog::info("Read json file {}", this->filename.string());
        return true;
    }
    boost::iostreams::filtering_istream i;
    i.push(boost::iostreams::zstd_decompressor(boost::iostreams::zstd::best_compression));
    i.push(boost::iostreams::file_descriptor_source(this->filename.string()));
    try {
        i >> this->js_int;
    } catch (nlohmann::json::parse_error& e) {
        SPDLOG_ERROR("Error parsing {}: {}", this->filename.string(), e.what());
        return false;
    }
    spdlog::info("Read json file {}", this->filename.string());
    return true;
}

void Serializer::write_out() const {
    if (not this->compression) {
        std::ofstream o(this->filename);

        if (not o.is_open()) {
            SPDLOG_ERROR("Failed to write to {}", this->filename.string());
            return;
        }
        o << std::setw(1) << this->js_int;
    } else {
        boost::iostreams::filtering_ostream o;
        o.push(boost::iostreams::zstd_compressor(boost::iostreams::zstd::best_compression));
        o.push(boost::iostreams::file_descriptor_sink(this->filename.string()));
        o << this->js_int << boost::iostreams::zstd::stream_end << std::flush;
    }
}

void to_json(nlohmann::json& j, const Metadata& m) {
    j = nlohmann::json{{"creator", m.creator},
                       {"version", m.version},
                       {"datetime", std::chrono::system_clock::to_time_t(m.datetime)},
                       {"papertype", m.papertype},
                       {"sysFingerprint", m.sysFingerprint}};
}

void from_json(const nlohmann::json& j, Metadata& m) {
    j.at("creator").get_to(m.creator);
    j.at("version").get_to(m.version);
    j.at("datetime").get_to(m.datetime);
    j.at("papertype").get_to(m.papertype);
    j.at("sysFingerprint").get_to(m.sysFingerprint);
}
}  // namespace storage

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

namespace date {
void to_json(nlohmann::json& j, const year_month_day& ymd) {
    std::chrono::seconds seconds{sys_days{ymd}.time_since_epoch()};
    j = nlohmann::json{{"seconds", seconds.count()}};
}

void from_json(const nlohmann::json& j, year_month_day& ymd) {
    int seconds = 0;
    j.at("seconds").get_to(seconds);
    sys_seconds sysSeconds{std::chrono::seconds(seconds)};
    ymd = year_month_day{floor<days>(sysSeconds)};
}
}  // namespace date
