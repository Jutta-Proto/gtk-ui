#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>

#include "backend/storage/Serializer.hpp"

namespace backend::storage {
struct UserProfile {
    /**
     * An unique card id of the user.
     * This will be used for hashing this object.
     **/
    std::string cardId;
    /**
     * The factor for the grind time.
     * Default: 1.0 (100%)
     **/
    double beansFactor{1.0};
    /**
     * The factor for the water time.
     * Default: 1.0 (100%)
     **/
    double waterFactor{1.0};
    /**
     * The factor for the temperature.
     * Default: 1.0 (100%)
     **/
    double tempFactor{1.0};

    UserProfile() = default;
    explicit UserProfile(std::string&& cardId);
    bool operator==(const UserProfile& other) const;
};

using UserProfilesMap = std::unordered_map<size_t, UserProfile>;

struct UserProfileStorage {
    explicit UserProfileStorage(const std::filesystem::path& settings_file);

    /**
     * Returns the corresponding UserProfile for the given cardId,
     * or a new instance in case the cardId does not exist.
     * In case a new UserProfile is being returned, this fuction calls write_profiles() to store all profiles to disc.
     **/
    UserProfile* get_profile(std::string&& cardId);
    /**
     * Returns a pointer to the default user profile.
     **/
    static UserProfile* get_default_profile();
    /**
     * Writes all user profiles to disc.
     **/
    void write_profiles();

 private:
    UserProfilesMap profiles{};
    storage::Serializer file_handl;
};

UserProfileStorage& get_user_profile_storage_instance();
void to_json(nlohmann::json& j, const UserProfile& p);
void from_json(const nlohmann::json& j, UserProfile& p);
const std::string sha_256(const std::string& s);
}  // namespace backend::storage