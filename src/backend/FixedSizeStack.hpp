#pragma once

#include <array>
#include <cstddef>
#include <string>

namespace backend {
class FixedSizeStack {
 public:
    static constexpr size_t MAX_SIZE = 10;

 private:
    /**
     * Holds max MAX_SIZE chars.
     **/
    std::array<char, MAX_SIZE> buffer{};
    /**
     * The index of the first char.
     * Only valid in case startIndex != endIndex.
     **/
    size_t startIndex{0};
    /**
     * The index after the last char.
     * Only valid in case startIndex != endIndex.
     **/
    size_t endIndex{0};
    size_t numElements{0};

 public:
    void push(char c);
    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;
    std::string pop();
    void clear();
};
}  // namespace backend
