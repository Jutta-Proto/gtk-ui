cmake_minimum_required(VERSION 3.16)

add_library(ui_widgets STATIC CoffeeSelectionWidget.hpp
                              CoffeeSelectionWidget.cpp
                              EditCustomCoffeeWidget.hpp
                              EditCustomCoffeeWidget.cpp
                              CoffeeMakerDetectionWidget.hpp
                              CoffeeMakerDetectionWidget.cpp
                              CoffeeButton.hpp
                              CoffeeButton.cpp
                              NfcCardReaderWidget.cpp
                              NfcCardReaderWidget.hpp
                              StatusBarWidget.cpp
                              StatusBarWidget.hpp)

target_link_libraries(ui_widgets PUBLIC PkgConfig::GTKMM
                                 PRIVATE backend storage)
