cmake_minimum_required(VERSION 3.16)

add_subdirectory(storage)

add_library(backend STATIC CoffeeMakerConnectionHandler.hpp
                           CoffeeMakerConnectionHandler.cpp
                           NfcCardReader.cpp
                           NfcCardReader.hpp
                           FixedSizeStack.cpp
                           FixedSizeStack.hpp)

target_link_libraries(backend PUBLIC jutta_bt_proto
                              PRIVATE logger PkgConfig::GTKMM cryptopp::cryptopp bt)

install(TARGETS backend)
