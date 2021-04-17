#pragma once

#include <chrono>
#include <string>

namespace backend::storage {
struct Metadata {
    unsigned version = getVersion();
    std::chrono::system_clock::time_point datetime = std::chrono::system_clock::now();
    std::string sysFingerprint;

    [[nodiscard]] size_t hash() const;

    static constexpr unsigned getVersion() {
        return 6;
    }

    bool operator==(const Metadata& other) const;
};
}  // namespace backend::storage
