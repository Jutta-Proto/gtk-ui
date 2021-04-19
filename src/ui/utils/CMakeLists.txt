cmake_minimum_required(VERSION 3.16)

add_library(ui_utils STATIC UiUtils.hpp
                            UiUtils.cpp)

target_link_libraries(ui_utils PUBLIC PkgConfig::GTKMM
                               PRIVATE logger)
