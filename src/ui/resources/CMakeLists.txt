cmake_minimum_required(VERSION 3.16)

set(RESOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/ui_resources.c.xml PARENT_SCOPE)
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(RESOURCE_DEPS
                    ${CMAKE_CURRENT_SOURCE_DIR}/background.jpg
                    ${CMAKE_CURRENT_SOURCE_DIR}/theme.css
                    ${CMAKE_CURRENT_SOURCE_DIR}/coffee.png
     PARENT_SCOPE)