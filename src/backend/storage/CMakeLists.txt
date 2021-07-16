cmake_minimum_required(VERSION 3.16)

add_library(storage STATIC Metadata.cpp
                           Metadata.hpp
                           Serializer.cpp
                           Serializer.hpp
                           Settings.cpp
                           Settings.hpp
                           UserProfileStorage.cpp
                           UserProfileStorage.hpp)

target_link_libraries(storage PUBLIC logger nlohmann_json::nlohmann_json
                              PRIVATE stdc++fs)
                              
target_compile_features(storage PRIVATE cxx_std_17 cxx_constexpr)
