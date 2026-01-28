cmake_minimum_required(VERSION 3.8...3.26)

project(OVRStartGame)

# ===============================
# C++ standard
# ===============================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===============================
# Dobby options
# ===============================
macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

SET_OPTION(DOBBY_DEBUG OFF)
SET_OPTION(DOBBY_GENERATE_SHARED OFF)

# add dobby subproject (adjust path if needed)
add_subdirectory(extern/Dobby dobby)

# ===============================
# Include directories (logical)
# ===============================
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/BNM-Android/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/BNM-Android/external/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/BNM-Android/external
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/BNM-Android/external/utf8
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/BNM-Android/src/private
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/Dobby/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Include         # your actual header folder (capital I)
    ${CMAKE_CURRENT_SOURCE_DIR}                  # project root so #include "Menu/..." can work
)

# ===============================
# LEGACY-HEADER WRAPPER (automatically create an 'include' tree
# to resolve includes like "include/Menu/ModMenu.hpp")
#
# This is a non-destructive workaround: it creates small wrapper headers under
# ${CMAKE_CURRENT_SOURCE_DIR}/include/... which forward-include the real files
# in ${CMAKE_CURRENT_SOURCE_DIR}/Include/...
# ===============================
set(LEGACY_INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(MAKE_DIRECTORY "${LEGACY_INCLUDE_ROOT}")

# Gather all headers under the actual Include directory
file(GLOB_RECURSE REAL_HEADERS
     LIST_DIRECTORIES OFF
     "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp"
)

foreach(real_hdr IN LISTS REAL_HEADERS)
    # relative path under Include (e.g. Menu/ModMenu.hpp)
    file(RELATIVE_PATH relpath "${CMAKE_CURRENT_SOURCE_DIR}/Include" "${real_hdr}")

    # wrapper path: include/<relpath> (create parent dirs as needed)
    set(wrapper "${LEGACY_INCLUDE_ROOT}/${relpath}")
    get_filename_component(wrapper_dir "${wrapper}" DIRECTORY)
    file(MAKE_DIRECTORY "${wrapper_dir}")

    # write a tiny forwarding header. Use absolute path to the real header so
    # the preprocessor finds it regardless of include search ordering.
    file(WRITE "${wrapper}" "/* Auto-generated wrapper to map \"include/${relpath}\" -> real header */\n")
    file(APPEND "${wrapper}" "#pragma once\n")
    # Real header absolute path (use forward slashes)
    file(APPEND "${wrapper}" "#include \"${real_hdr}\"\n")
endforeach()

# Ensure legacy include root is added (so "include/..." finds our wrappers)
list(APPEND INCLUDE_DIRS "${LEGACY_INCLUDE_ROOT}")

# ===============================
# BNM static library
# ===============================
add_library(
    BNM
    STATIC
    extern/BNM-Android/src/Class.cpp
    extern/BNM-Android/src/ClassesManagement.cpp
    extern/BNM-Android/src/Coroutine.cpp
    extern/BNM-Android/src/Defaults.cpp
    extern/BNM-Android/src/Delegates.cpp
    extern/BNM-Android/src/EventBase.cpp
    extern/BNM-Android/src/Exceptions.cpp
    extern/BNM-Android/src/FieldBase.cpp
    extern/BNM-Android/src/Hooks.cpp
    extern/BNM-Android/src/Image.cpp
    extern/BNM-Android/src/Internals.cpp
    extern/BNM-Android/src/Loading.cpp
    extern/BNM-Android/src/MethodBase.cpp
    extern/BNM-Android/src/MonoStructures.cpp
    extern/BNM-Android/src/PropertyBase.cpp
    extern/BNM-Android/src/UnityStructures.cpp
    extern/BNM-Android/src/Utils.cpp
)

target_include_directories(
    BNM
    PUBLIC
    ${INCLUDE_DIRS}
)

# ===============================
# Main shared library target
# ===============================
add_library(
    ${CMAKE_PROJECT_NAME}
    SHARED
    native-lib.cpp
    Include/XRInput.cpp
    Include/Menu/ModMenu.cpp
    Include/Menu/Settings.cpp
    Include/NotiLib/NotiLib.cpp
)

# make sure the main target sees the include dirs
target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/OpenSSL/${ANDROID_ABI}/include
)

# ===============================
# Linking
# ===============================
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    android
    log
    BNM
    dobby
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/OpenSSL/${ANDROID_ABI}/lib/libcrypto.a
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/OpenSSL/${ANDROID_ABI}/lib/libssl.a
)
