# CMakeLists.txt --- CMake project settings
#    ex) cmake -G "Visual Studio 9 2008"
#    ex) cmake -DCMAKE_BUILD_TYPE=Release -G "MSYS Makefiles"
##############################################################################

# CMake minimum version
cmake_minimum_required(VERSION 2.4)

# enable testing
enable_testing()

# project name and language
project(microtar C CXX)

# check build type
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# make uppercase string of build type
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)

# check build type again
if (NOT uppercase_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL)$")
    message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

## set output directory (build/)
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

## enable Win32 resource
#enable_language(RC)

if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # using Clang
        set(CMAKE_C_FLAGS "-static")
        set(CMAKE_CXX_FLAGS "-static")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # using GCC
        set(CMAKE_C_FLAGS "-static -static-libgcc -static-libstdc++")
        set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
    elseif (MSVC)
        # replace "/MD" with "/MT" (building without runtime DLLs)
        set(CompilerFlags
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        foreach(CompilerFlags ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlags} "${${CompilerFlags}}")
        endforeach()
    endif()
endif()

##############################################################################

include(CheckSymbolExists)
check_symbol_exists(_fseeki64 "stdio.h" HAVE__FSEEKI64)

include_directories(.)

# libmicrotar.a
add_library(microtar STATIC microtar.c)

# microtar-read-test.exe
add_executable(microtar-read-test tests/microtar-read-test.cpp)
target_link_libraries(microtar-read-test microtar)

# microtar-write-test.exe
add_executable(microtar-write-test tests/microtar-write-test.cpp)
target_link_libraries(microtar-write-test microtar)

# microtar-memory-read-test.exe
add_executable(microtar-memory-read-test tests/microtar-memory-read-test.cpp)
target_link_libraries(microtar-memory-read-test microtar)

# microtar-memory-write-test.exe
add_executable(microtar-memory-write-test tests/microtar-memory-write-test.cpp)
target_link_libraries(microtar-memory-write-test microtar)

# tests
add_test(NAME microtar-read-test
         COMMAND $<TARGET_FILE:microtar-read-test> ${PROJECT_SOURCE_DIR}/test-data/test-file.tar)
add_test(NAME microtar-write-test
         COMMAND $<TARGET_FILE:microtar-write-test> ${PROJECT_BINARY_DIR}/written.tar
         WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
add_test(NAME microtar-memory-read-test
         COMMAND $<TARGET_FILE:microtar-memory-read-test> ${PROJECT_SOURCE_DIR}/test-data/test-file.tar)
add_test(NAME microtar-memory-write-test
         COMMAND $<TARGET_FILE:microtar-memory-write-test> ${PROJECT_SOURCE_DIR}/test-data/test-file-2.tar)

##############################################################################
