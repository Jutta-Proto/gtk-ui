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
                              StatusBarWidget.hpp
                              CoffeeMakerStatusWidget.cpp
                              CoffeeMakerStatusWidget.hpp
                              CoffeeMakerButton.cpp
                              CoffeeMakerButton.hpp
                              StatusCircleWidget.cpp
                              StatusCircleWidget.hpp
                              StatusOverlayWidget.cpp
                              StatusOverlayWidget.hpp)

target_link_libraries(ui_widgets PUBLIC PkgConfig::GTKMM jutta_bt_proto
                                 PRIVATE backend storage)
