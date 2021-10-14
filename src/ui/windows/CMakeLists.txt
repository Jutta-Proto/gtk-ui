cmake_minimum_required(VERSION 3.16)

add_library(ui_windows STATIC MainWindow.hpp MainWindow.cpp)

target_link_libraries(ui_windows PUBLIC PkgConfig::GTKMM jutta_bt_proto
                                 PRIVATE logger ui_widgets ui_utils storage)
