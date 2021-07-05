#include "UserProfileStorage.hpp"
#include <cstddef>
#include <functional>

namespace backend::storage {

UserProfile::UserProfile(std::string&& cardId) : cardId(std::move(cardId)) {}

bool UserProfile::operator==(const UserProfile& other) const {
    return cardId == other.cardId && beansFactor == other.beansFactor && waterFactor == other.waterFactor && tempFactor == other.tempFactor;
}

UserProfileStorage::UserProfileStorage(const std::filesystem::path& settings_file) : file_handl(settings_file) {
    SPDLOG_DEBUG("Initializing UserProfileStorage, this should only happen once per execution");
    if (std::filesystem::exists(settings_file)) {
        SPDLOG_INFO("UserProfileStorage file {} exists, trying to read it", settings_file.string());
        if (this->file_handl.read_in()) {
            this->profiles = this->file_handl.js_int.get<std::unordered_map<size_t, UserProfile>>();
            SPDLOG_DEBUG("Settings loaded successfully");
        }
    } else {
        SPDLOG_ERROR("Cannot read the UserProfileStorage file '{}'. Creating a new one.", settings_file.string());
        this->write_profiles();
    }
}

UserProfile* UserProfileStorage::get_profile(std::string&& cardId) {
    size_t hash = std::hash<std::string>{}(cardId);
    if (profiles.find(hash) == profiles.end()) {
        profiles[hash] = UserProfile(std::move(cardId));
        write_profiles();
    }
    return &(profiles[hash]);
}

UserProfile* UserProfileStorage::get_default_profile() {
    static UserProfile profile("");
    return &profile;
}

void UserProfileStorage::write_profiles() {
    file_handl.js_int = profiles;
    file_handl.write_out();
}

UserProfileStorage& get_user_profile_storage_instance() {
    static UserProfileStorage storage_instance("UserProfiles.json");
    return storage_instance;
}

void to_json(nlohmann::json& j, const UserProfile& p) {
    j = nlohmann::json{{"cardId", p.cardId}, {"beansFactor", p.beansFactor}, {"waterFactor", p.waterFactor}, {"tempFactor", p.tempFactor}};
}

void from_json(const nlohmann::json& j, UserProfile& p) {
    j.at("cardId").get_to(p.cardId);
    j.at("beansFactor").get_to(p.beansFactor);
    j.at("waterFactor").get_to(p.waterFactor);
    j.at("tempFactor").get_to(p.tempFactor);
}
}  // namespace backend::storage