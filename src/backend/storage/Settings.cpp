#include "context/backend/storage/Settings.hpp"

namespace storage {
Settings::Settings(const std::filesystem::path& settings_file) : file_handl(settings_file) {
    SPDLOG_DEBUG("Initializing Settings, this should only happen once per execution");
    if (std::filesystem::exists(settings_file)) {
        SPDLOG_INFO("Settings file {} exists, trying to read it", settings_file.string());
        if (this->file_handl.read_in()) {
            this->data = this->file_handl.js_int.get<Settings::SettingsData>();
            if (this->data.version != PAPERSEARCH_VERSION) {
                SPDLOG_ERROR("Settings version: \"{}\" does not match!", this->data.version, PAPERSEARCH_VERSION);
                this->data.version = PAPERSEARCH_VERSION;
                this->write_settings();
            }
            SPDLOG_DEBUG("Settings loaded succesfully");
        }
    } else {
        SPDLOG_ERROR("Cannot read the Settings file {}", settings_file.string());
        this->data.version = PAPERSEARCH_VERSION;
        this->write_settings();
    }
}

Settings::SettingsData& Settings::access_data() {
    return this->data;
}

void Settings::write_settings() {
    this->file_handl.js_int = this->data;
    this->file_handl.write_out();
}

Settings& get_settings_instance() {
    static Settings settings_instance("Settings.json");
    return settings_instance;
}

void to_json(nlohmann::json& j, const Settings::SettingsData& s) {
    // Store settings:
    j = nlohmann::json{{"version", s.version} /*, {"some_setting", s.some_setting}*/};
}

void from_json(const nlohmann::json& j, Settings::SettingsData& s) {
    j.at("version").get_to(s.version);
    // Load settings:
    // if (j.contains("some_setting")) { j.at("some_setting").get_to(s.user_name); }
}
}  // namespace storage
