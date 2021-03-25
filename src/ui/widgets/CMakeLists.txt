cmake_minimum_required(VERSION 3.16)

add_library(ui_widgets STATIC CoffeeSelection.hpp
                              CoffeeSelection.cpp
                              CustomCoffee.hpp
                              CustomCoffee.cpp
                              CoffeeMakerDetectionWidget.hpp
                              CoffeeMakerDetectionWidget.cpp)

target_link_libraries(ui_widgets PUBLIC PkgConfig::GTKMM
                                 PRIVATE backend)
