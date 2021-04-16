#pragma once

#include "context/backend/logger/Logger.hpp"
#include "context/utils/Date.hpp"

#include <chrono>
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

// Required so we can parse std::optional
#define JSON_HAS_CPP_17

namespace storage {
struct Metadata;
void to_json(nlohmann::json& j, const Metadata& m);
void from_json(const nlohmann::json& j, Metadata& m);
}  // namespace storage

namespace date {
void to_json(nlohmann::json& j, const year_month_day& ymd);
void from_json(const nlohmann::json& j, year_month_day& ymd);
}  // namespace date

namespace storage {
class Settings;
class Serializer {
 private:
    friend Settings;
    const std::filesystem::path filename;
    const bool compression = false;
    nlohmann::json js_int = nlohmann::json::object();

 public:
    explicit Serializer(const std::filesystem::path filename, bool compression = false) noexcept : filename(filename), compression(compression) {}
    explicit Serializer(const std::string&& filename, bool compression = false) noexcept : filename(std::filesystem::path(filename)), compression(compression) {}
    explicit Serializer(const std::string_view filename, bool compression = false) noexcept : filename(std::filesystem::path(filename)), compression(compression) {}

    bool read_in();
    void write_out() const;

    [[nodiscard]] bool exists(const std::string_view s) const {
        return this->js_int.contains(s);
    }

    template <class T>
    [[nodiscard]] std::optional<T> get_data(const std::string_view s) const {
        if (not this->js_int.contains(s)) {
            return std::nullopt;
        }
        T t;
        this->js_int.find(s)->get_to(t);
        return std::make_optional(t);
    }

    template <class T>
    bool get_to(const std::string_view s, T& t) const {
        if (not this->exists(s)) {
            return false;
        }
        this->js_int.find(s)->get_to(t);
        return true;
    }

    template <class T>
    void set_data(const std::string_view key, T value) {
        this->js_int.emplace(key, value);
    }
};
}  // namespace storage

namespace nlohmann {
template <typename T>
struct adl_serializer<std::optional<T>> {
    static void to_json(json& j, const std::optional<T>& opt) {
        if (opt.has_value()) {
            j = opt.value();
        } else {
            j = nullptr;
        }
    }

    static void from_json(const json& j, std::optional<T>& opt) {
        if (j.is_null()) {
            opt = std::nullopt;
        } else {
            opt = j.get<T>();
        }
    }
};

template <typename T>
struct adl_serializer<std::shared_ptr<T>> {
    static void to_json(json& j, const std::shared_ptr<T>& ptr) {
        if (ptr != nullptr) {
            j = *ptr;
        } else {
            j = nullptr;
        }
    }

    static void from_json(const json& j, std::shared_ptr<T>& ptr) {
        if (j.is_null()) {
            ptr = nullptr;
        } else {
            ptr = std::make_shared<T>(j.get<T>());
        }
    }
};

template <>
struct adl_serializer<std::chrono::system_clock::time_point> {
    static void to_json(json& j, const std::chrono::system_clock::time_point& tp);
    static void from_json(const json& j, std::chrono::system_clock::time_point& tp);
};
}  // namespace nlohmann
