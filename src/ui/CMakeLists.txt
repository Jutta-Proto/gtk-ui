cmake_minimum_required(VERSION 3.16)

add_library(ui STATIC UiContext.hpp UiContext.cpp)

add_subdirectory(windows)

target_link_libraries(ui PUBLIC PkgConfig::GTKMM
                         PRIVATE logger ui_windows)
