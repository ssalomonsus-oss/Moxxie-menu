cmake_minimum_required(VERSION 3.22.1)

project(OVRStartGame)

# -----------------------------
# Native library
# -----------------------------
add_library(
        OVRStartGame
        SHARED

        native-lib.cpp

        Include/XRInput.cpp
        Include/Menu/ModMenu.cpp
        Include/Menu/Settings.cpp
        Include/NotiLib/NotiLib.cpp
)

# -----------------------------
# ✅ FIX: expose Include folder
# -----------------------------
target_include_directories(
        OVRStartGame
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

# -----------------------------
# External libraries
# -----------------------------
add_subdirectory(extern/BNM-Android)
add_subdirectory(extern/Dobby)

# -----------------------------
# Android system libs
# -----------------------------
find_library(
        log-lib
        log
)

# -----------------------------
# Link everything
# -----------------------------
target_link_libraries(
        OVRStartGame

        BNM
        dobby
        ${log-lib}
)
