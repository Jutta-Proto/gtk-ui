cmake_minimum_required(VERSION 3.16)

add_library(ui_dialogs STATIC DetectCoffeeMakerDialog.hpp
                              DetectCoffeeMakerDialog.cpp)

target_link_libraries(ui_dialogs PUBLIC PkgConfig::GTKMM
                                 PRIVATE backend)
