cmake_minimum_required(VERSION 3.10)
project(chromadb VERSION 0.0.1)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

set(SOURCES
    src/ChromaDB/ChromaApiClient.cpp
    src/ChromaDB/Client.cpp
    src/ChromaDB/Collection.cpp
    src/ChromaDB/Embeddings/EmbeddingFunction.cpp
    src/ChromaDB/Embeddings/JinaEmbeddingFunction.cpp
    src/ChromaDB/Embeddings/OpenAIEmbeddingFunction.cpp
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
install(DIRECTORY include/Http/ DESTINATION include/Http)
install(DIRECTORY include/Json/ DESTINATION include/Json)
install(FILES include/ChromaDB/ChromaApiClient.h include/ChromaDB/QueryResponseResource.h include/ChromaDB/Utils.h include/ChromaDB/Client.h include/ChromaDB/Collection.h include/ChromaDB/Exception.h DESTINATION include/ChromaDB)
