cmake_minimum_required(VERSION 3.16)

set(MAIN_EXECUTABLE ${PROJECT_NAME})

add_subdirectory(logger)
add_subdirectory(ui)

add_executable(${MAIN_EXECUTABLE} WIN32 main.cpp)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows" AND NOT ${CMAKE_RC_COMPILER})
    if (CMAKE_RC_COMPILER)
        set(ICON_FILE ${CMAKE_SOURCE_DIR}/resources/papersearch.ico)
        set(EXEC_NAME $<TARGET_FILE:${MAIN_EXECUTABLE}>)
        configure_file (
            "jutta_ui_res_header.h.in"
            "${CMAKE_CURRENT_BINARY_DIR}/jutta_ui_res_header.h"
        )
        target_sources(${MAIN_EXECUTABLE} PRIVATE jutta_ui.rc)
        set_property(SOURCE jutta_ui.rc PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR})
    else()
        message(WARNING "windres not initialized")
    endif()
endif()

target_link_libraries(${MAIN_EXECUTABLE} PRIVATE logger ui)
set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS JUTTA_UI_VERSION="${PROJECT_VERSION}" JUTTA_UI_VERSION_NAME="${VERSION_NAME}")
install(TARGETS ${MAIN_EXECUTABLE} RUNTIME DESTINATION)
