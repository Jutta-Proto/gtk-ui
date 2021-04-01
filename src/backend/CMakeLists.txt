cmake_minimum_required(VERSION 3.16)

add_library(backend STATIC CoffeeMakerDetection.hpp
                           CoffeeMakerDetection.cpp
                           CoffeeMakerWrapper.hpp
                           CoffeeMakerWrapper.cpp
                           NfcDetection.cpp
                           NfcDetection.hpp)

target_link_libraries(backend PUBLIC jutta_proto
                              PRIVATE logger PkgConfig::GTKMM)

install(TARGETS backend)
