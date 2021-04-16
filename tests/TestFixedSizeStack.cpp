#include <catch2/catch.hpp>
#include <filesystem>
#include <string>

#include "backend/FixedSizeStack.hpp"

constexpr std::string_view TEST_TAG = "[TestFixedSizeStack]";

TEST_CASE("Empty after init", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    REQUIRE(stack.empty());
    // NOLINTNEXTLINE (readability-container-size-empty)
    REQUIRE(stack.size() == 0);
}

TEST_CASE("Empty add one", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    stack.push('1');
    REQUIRE_FALSE(stack.empty());
    REQUIRE(stack.size() == 1);
}

TEST_CASE("Clear", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    stack.push('1');
    stack.clear();
    REQUIRE(stack.empty());
    // NOLINTNEXTLINE (readability-container-size-empty)
    REQUIRE(stack.size() == 0);
}

TEST_CASE("Add multiple no overlap", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    for (size_t i = 0; i < backend::FixedSizeStack::MAX_SIZE; i++) {
        stack.push('1');
    }
    REQUIRE_FALSE(stack.empty());
    REQUIRE(stack.size() == 10);
}

TEST_CASE("Add multiple overlap only one", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    for (size_t i = 0; i < backend::FixedSizeStack::MAX_SIZE; i++) {
        stack.push(static_cast<char>('0' + (i % 10)));
    }
    stack.push('Z');
    REQUIRE_FALSE(stack.empty());
    REQUIRE(stack.size() == 10);
}

TEST_CASE("Add multiple overlap", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    for (size_t i = 0; i < backend::FixedSizeStack::MAX_SIZE * 1024; i++) {
        stack.push(static_cast<char>('0' + (i % 10)));
    }
    stack.push('Z');
    REQUIRE_FALSE(stack.empty());
    REQUIRE(stack.size() == 10);
}

TEST_CASE("Pop empty", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    REQUIRE(std::string{""} == stack.pop());
    REQUIRE(stack.empty());
    // NOLINTNEXTLINE (readability-container-size-empty)
    REQUIRE(stack.size() == 0);
}

TEST_CASE("Pop single", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    stack.push('S');
    REQUIRE(std::string{"S"} == stack.pop());
    REQUIRE(stack.empty());
    // NOLINTNEXTLINE (readability-container-size-empty)
    REQUIRE(stack.size() == 0);
}

TEST_CASE("Pop overlap only one", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    stack.push('S');
    for (size_t i = 0; i < backend::FixedSizeStack::MAX_SIZE - 1; i++) {
        stack.push(static_cast<char>('0' + (i % 10)));
    }
    stack.push('Z');
    REQUIRE(std::string{"012345678Z"} == stack.pop());
    REQUIRE(stack.empty());
    // NOLINTNEXTLINE (readability-container-size-empty)
    REQUIRE(stack.size() == 0);
}

TEST_CASE("Pop overlap", TEST_TAG.data()) {
    backend::FixedSizeStack stack{};
    for (size_t i = 0; i < 1024; i++) {
        stack.push(static_cast<char>('0' + (i % 10)));
    }
    stack.push('Z');
    REQUIRE(std::string{"567890123Z"} == stack.pop());
    REQUIRE(stack.empty());
    // NOLINTNEXTLINE (readability-container-size-empty)
    REQUIRE(stack.size() == 0);
}
