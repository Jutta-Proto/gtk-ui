cmake_minimum_required(VERSION 3.16)

set(RESOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/ui_resources.c.xml PARENT_SCOPE)
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(RESOURCE_DEPS
                  ${CMAKE_CURRENT_SOURCE_DIR}/background.jpg # https://www.pexels.com/photo/brown-coffee-beans-4109743/
                  ${CMAKE_CURRENT_SOURCE_DIR}/theme.css
                  ${CMAKE_CURRENT_SOURCE_DIR}/coffee.png # https://www.pexels.com/photo/beverage-break-breakfast-brown-414630/
                  ${CMAKE_CURRENT_SOURCE_DIR}/espresso.png # https://www.pexels.com/photo/latte-in-cup-2648988/
                  ${CMAKE_CURRENT_SOURCE_DIR}/nfc.png # https://www.iconfinder.com/icons/6405387/nfc_smartphone_call_phone_icon
                  ${CMAKE_CURRENT_SOURCE_DIR}/gear.png # https://www.iconfinder.com/icons/211751/gear_icon
                  ${CMAKE_CURRENT_SOURCE_DIR}/coffee_gear.png # https://www.iconfinder.com/icons/211751/gear_icon
                  ${CMAKE_CURRENT_SOURCE_DIR}/coffee_maker.png
     PARENT_SCOPE)