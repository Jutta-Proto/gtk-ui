cmake_minimum_required(VERSION 3.16)

add_library(ui_widgets STATIC CoffeeSelectionWidget.hpp
                              CoffeeSelectionWidget.cpp
                              CustomCoffeeWidget.hpp
                              CustomCoffeeWidget.cpp
                              CoffeeMakerDetectionWidget.hpp
                              CoffeeMakerDetectionWidget.cpp
                              CoffeeButton.hpp
                              CoffeeButton.cpp
                              NfcCardReaderWidget.cpp
                              NfcCardReaderWidget.hpp)

target_link_libraries(ui_widgets PUBLIC PkgConfig::GTKMM
                                 PRIVATE backend)
