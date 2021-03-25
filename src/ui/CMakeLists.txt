cmake_minimum_required(VERSION 3.16)

add_subdirectory(widgets)
add_subdirectory(dialogs)
add_subdirectory(windows)
add_subdirectory(resources)

#add_library(ui STATIC UiContext.hpp UiContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/ui_resources.c)
add_library(ui STATIC UiContext.hpp UiContext.cpp)

# Propagate the variables upwards
set(RESOURCE_DEF ${RESOURCE_DEF} PARENT_SCOPE)
set(RESOURCE_DIR ${RESOURCE_DIR} PARENT_SCOPE)
set(RESOURCE_DEPS ${RESOURCE_DEPS} PARENT_SCOPE)

target_link_libraries(ui PUBLIC PkgConfig::GTKMM
                         PRIVATE logger ui_windows)
