cmake_minimum_required(VERSION 3.24)
project(juce_widgets_tests LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()
include(FetchContent)

# Match the dependency revisions used by KnobKraft Orm. Keep this setup separate
# from the library's CMakeLists.txt so embedding applications choose their own.
FetchContent_Declare(JUCE
    URL https://github.com/juce-framework/JUCE/archive/29396c22c93392d6738e021b83196283d6e4d850.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(fmt
    URL https://github.com/fmtlib/fmt/archive/407c905e45ad75fc29bf0f9bb7c5c2fd3475976f.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(spdlog
    URL https://github.com/gabime/spdlog/archive/79524ddd08a4ec981b7fea76afd08ee05f83755d.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/archive/55f93686c01528224f448c19128836e7df245f72.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(juce_utils
    URL https://github.com/christofmuc/juce-utils/archive/610332e2c5968a65cff15fe7b7fe4a34a4569eab.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(JUCE fmt spdlog json)

add_library(juce-static STATIC)
target_link_libraries(juce-static
    PRIVATE juce::juce_audio_utils juce::juce_gui_extra juce::juce_cryptography
    PUBLIC juce::juce_recommended_config_flags)
target_compile_definitions(juce-static PUBLIC
    JUCE_STANDALONE_APPLICATION=1
    JUCE_MODAL_LOOPS_PERMITTED=1
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_USE_FLAC=1
    INTERFACE $<TARGET_PROPERTY:juce-static,COMPILE_DEFINITIONS>)
target_include_directories(juce-static
    INTERFACE $<TARGET_PROPERTY:juce-static,INCLUDE_DIRECTORIES>)

FetchContent_MakeAvailable(juce_utils)
set(JUCE_WIDGETS_BUILD_CONNECTION_TESTS ON CACHE BOOL "" FORCE)
add_subdirectory(../.. juce-widgets)
