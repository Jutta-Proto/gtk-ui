#pragma once

#include "backend/storage/Serializer.hpp"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <string_view>

namespace backend::storage {
constexpr std::string_view SETTINGS_VERSION{"1.0"};

class Settings {
 public:
    struct SettingsData {
     public:
        std::string version;
        /**
         * Store settings here:
         **/
        /*std::optional<std::string> some_setting = std::nullopt;*/

        SettingsData() noexcept = default;
        explicit SettingsData(std::string&& version) noexcept : version(version) {}

        bool operator==(const SettingsData& other) const {
            // Add every setting here for comparision:
            return this->version == other.version;
        }
    };

 private:
    storage::Serializer file_handl;
    SettingsData data;

 public:
    Settings() = delete;
    explicit Settings(const std::filesystem::path& settings_file);

    /* Get access to the internal SettingsData object
     * Note if setting was changed and shall be written to the filesystem, it' required to call write_settings() */
    [[nodiscard]] SettingsData& access_data();
    // Write the settings to the specified file
    void write_settings();
};

Settings& get_settings_instance();
void to_json(nlohmann::json& j, const Settings::SettingsData& s);
void from_json(const nlohmann::json& j, Settings::SettingsData& s);
}  // namespace backend::storage
