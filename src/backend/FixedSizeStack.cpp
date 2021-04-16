#include "FixedSizeStack.hpp"
#include <string>

namespace backend {

void FixedSizeStack::push(char c) {
    buffer[endIndex] = c;

    if (++endIndex >= MAX_SIZE) {
        endIndex = 0;
    }

    if (size() >= MAX_SIZE && ++startIndex >= MAX_SIZE) {
        startIndex = 0;
    }

    if (numElements < MAX_SIZE) {
        numElements++;
    }
}

bool FixedSizeStack::empty() const {
    return numElements == 0;
}

size_t FixedSizeStack::size() const {
    return numElements;
}

std::string FixedSizeStack::pop() {
    std::string result;
    if (!empty()) {
        if (endIndex <= startIndex) {
            result = std::string(buffer.data() + startIndex, MAX_SIZE - startIndex);
            result.append(std::string(buffer.data(), endIndex));
        } else {
            result = std::string(buffer.data() + startIndex, endIndex - startIndex);
        }
    }
    clear();
    return result;
}

void FixedSizeStack::clear() {
    startIndex = 0;
    endIndex = 0;
    numElements = 0;
}
}  // namespace backend
