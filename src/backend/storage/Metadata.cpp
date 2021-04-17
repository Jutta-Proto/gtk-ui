#include "backend/storage/Metadata.hpp"

#include <ctime>
#include <functional>

namespace backend::storage {
size_t Metadata::hash() const {
    return std::hash<unsigned>{}(this->version) ^ std::hash<std::time_t>{}(std::chrono::system_clock::to_time_t(this->datetime)) ^ std::hash<std::string>{}(this->sysFingerprint);
}

bool Metadata::operator==(const Metadata& other) const {
    return this->version == other.version &&
        this->sysFingerprint == other.sysFingerprint &&
        std::chrono::system_clock::to_time_t(this->datetime) == std::chrono::system_clock::to_time_t(other.datetime);
}
}  // namespace backend::storage
