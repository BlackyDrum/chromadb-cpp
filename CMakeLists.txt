cmake_minimum_required(VERSION 3.10)
project(chromadb VERSION 0.2.0)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

include_directories(include)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

set(SOURCES
    src/ChromaDB/Client/ChromaApiClient.cpp
    src/ChromaDB/Client/Client.cpp
    src/ChromaDB/Resources/Collection.cpp
    src/ChromaDB/Embeddings/EmbeddingFunction.cpp
    src/ChromaDB/Embeddings/JinaEmbeddingFunction.cpp
    src/ChromaDB/Embeddings/OpenAIEmbeddingFunction.cpp
    src/ChromaDB/Exceptions/ChromaException.cpp
)

add_library(chromadb SHARED ${SOURCES})

target_link_libraries(chromadb PRIVATE OpenSSL::SSL OpenSSL::Crypto)

set_target_properties(chromadb PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "include/ChromaDB/ChromaDB.h"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS chromadb
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/ChromaDB
)

install(DIRECTORY include/ChromaDB/Embeddings DESTINATION include/ChromaDB)
install(DIRECTORY include/ChromaDB/Exceptions DESTINATION include/ChromaDB)
install(DIRECTORY include/ChromaDB/Resources DESTINATION include/ChromaDB)
install(DIRECTORY include/ChromaDB/Utils DESTINATION include/ChromaDB)
install(DIRECTORY include/ChromaDB/Client DESTINATION include/ChromaDB)
install(DIRECTORY include/Http/ DESTINATION include/Http)
install(DIRECTORY include/Json/ DESTINATION include/Json)

# Add Google Test
add_subdirectory(tests/googletest)

# Enable testing
enable_testing()

# Add the test executable
add_executable(runTests tests/test_main.cpp tests/test_client.cpp)

# Ensure Google Test uses the same runtime library
if (MSVC)
    set_target_properties(gtest PROPERTIES COMPILE_FLAGS "/MD")
    set_target_properties(gtest_main PROPERTIES COMPILE_FLAGS "/MD")
endif()

# Link the test executable against Google Test and the project library
target_link_libraries(runTests gtest gtest_main chromadb OpenSSL::SSL OpenSSL::Crypto)

set_target_properties(runTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add a post-build step to copy the DLL to the output directory
add_custom_command(TARGET chromadb POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:chromadb> ${CMAKE_BINARY_DIR}/bin
)

# Add tests to CTest
add_test(NAME ClientTest COMMAND runTests)
