cmake_minimum_required(VERSION 3.16)

set(PROJECT_DESCRIPTION "An unofficial UI to control JURA coffee makers.")

project("Jutta_UI"
    VERSION 0.0.0
    DESCRIPTION "${PROJECT_DESCRIPTION}"
    HOMEPAGE_URL "https://github.com/Jutta-Proto/gtk-ui")
set(VERSION_NAME "dev")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(jutta_ui_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})

    if(DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif()

    if(${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif()

    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()

message(STATUS "C++ Jutta UI Options")
message(STATUS "=======================================================")
jutta_ui_option(JUTTA_UI_BUILD_TESTS "Set to ON to build tests." ON)
jutta_ui_option(JUTTA_UI_STATIC_ANALYZE "Set to ON to enable the GCC 10 static analysis. If enabled, JUTTA_UI_ENABLE_LINTING has to be disabled." OFF)
jutta_ui_option(JUTTA_UI_ENABLE_LINTING "Set to ON to enable clang linting. If enabled, JUTTA_UI_STATIC_ANALYZE has to be disabled." OFF)
message(STATUS "=======================================================")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})

include(sanitizer)
include(gcc_analyze)
include(glib-resource-compiler)
include(FetchContent)

if(${JUTTA_UI_ENABLE_LINTING})
    message(STATUS "Enabling linting")
    include(clang-tidy)
else()
    message(STATUS "Linting is disabled")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

# gtk3 stuff
find_package(PkgConfig REQUIRED)

# Additional search path since gtkmm-3.0 will usually be installed in /usr
set(ENV{PKG_CONFIG_PATH} "/usr/lib/pkgconfig")
pkg_check_modules(GTKMM gtkmm-3.0>=3.22 REQUIRED IMPORTED_TARGET)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake"
        TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

set(CONAN_CONFIGS"Release;Debug;RelWithDebInfo")

if(NOT CMAKE_BUILD_TYPE IN_LIST CONAN_CONFIGS)
    set(CONAN_BUILD_TYPE "Debug")
else()
    set(CONAN_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()

conan_cmake_configure(REQUIRES catch2/2.13.8
    nlohmann_json/3.10.5
    spdlog/1.10.0
    cryptopp/8.7.0
    GENERATORS cmake_find_package
    BUILD missing)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings})

find_package(Catch2 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(cryptopp REQUIRED)

FetchContent_Declare(jutta_bt_proto
    GIT_REPOSITORY https://github.com/Jutta-Proto/protocol-bt-cpp.git
    GIT_TAG bcff06cd2916e2fdd8c39ac50d1c98ddb8efdca1) # 27.12.2022
FetchContent_MakeAvailable(jutta_bt_proto)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)

# Testing
if(${JUTTA_UI_BUILD_TESTS})
    message(STATUS "Testing is enabled")
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Testing is disabled")
endif()
