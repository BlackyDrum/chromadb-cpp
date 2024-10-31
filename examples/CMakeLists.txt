cmake_minimum_required(VERSION 3.10)
project(ExampleProject VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find ChromaDB
if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/ChromaDB")
    find_library(ChromaDB_LIB NAMES chromadb HINTS ${CMAKE_PREFIX_PATH}/lib)
    find_path(ChromaDB_INCLUDE_DIR NAMES ChromaDB/ChromaDB.h HINTS ${CMAKE_PREFIX_PATH}/include)
else()
    find_library(ChromaDB_LIB NAMES chromadb HINTS /usr/local/lib /usr/lib /lib)
    find_path(ChromaDB_INCLUDE_DIR NAMES ChromaDB/ChromaDB.h HINTS /usr/local/include /include)
endif()

if(NOT ChromaDB_LIB)
    message(FATAL_ERROR "ChromaDB library not found")
endif()

if(NOT ChromaDB_INCLUDE_DIR)
    message(FATAL_ERROR "ChromaDB include directory not found")
endif()

# Add the executable
add_executable(ExampleProject src/main.cpp)

# Include directories
include_directories(${ChromaDB_INCLUDE_DIR})

# Link against ChromaDB and OpenSSL
target_link_libraries(ExampleProject ${ChromaDB_LIB} OpenSSL::SSL OpenSSL::Crypto)

# Copy the DLL to the output directory on Windows
if (WIN32)
    add_custom_command(TARGET ExampleProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_PREFIX_PATH}/bin/chromadb.dll"
        $<TARGET_FILE_DIR:ExampleProject>/chromadb.dll
    )
endif()
