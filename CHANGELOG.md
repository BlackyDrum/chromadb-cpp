# Changelog

All notable changes to this project will be documented in this file.

## [1.0.1] - 2024-10-20
### Changed
- Updated ChromaDB docker image to version `0.5.15`

### Fixed
- Check embedding dimensionality to be the same

## [1.0.0] - 2024-10-19
### Changed
- Updated the C++ standard from C++17 to C++20.
- Renamed the function `GetLastRequestAdditionalMetadata()` to `GetRequestMetadata()` for clarity.
- Replaced string concatenation with `std::format` for more readable formatting.

## [0.4.3] - 2024-10-11
### Changed
- Replaced `push_back` with `emplace_back`

## [0.4.2] - 2024-09-19
### Removed
- Removed `version` from the `docker-compose.yml` file as it is no longer needed.
- Removed `DeleteCollections` method due to incompatibility with the new API.

### Added
- Added version to the ChromaDB image in `docker-compose.yml` (e.g., `chromadb/chroma:0.5.7`).

### Changed
- Accessing a deleted collection now throws an exception if the collection object is accessed.

## [0.4.1] - 2024-09-19
## Changed
- Updated error type from `AuthorizationError` to `AuthError` for compatibility with the latest ChromaDB version.
- Adjusted tests to expect other exception type.

## [0.4.0] - 2024-06-19
### Added
- `GetOrCreateCollection`: Added `GetOrCreateCollection` method to retrieve an existing collection or create a new one if it does not exist.
- Support for Retrieving Additional Metadata: Added support for retrieving additional metadata from embedding requests using the `GetLastRequestAdditionalMetadata()` method in embedding functions (e.g., JinaEmbeddingFunction, OpenAIEmbeddingFunction, ...).

## Changed
Renamed `Utils::join` to `Utils::Join`: The join function in the Utils class has been renamed to `Join` for consistency.

## [0.3.1] - 2024-06-18
### Added
- Copyright notice added to the main header

### Changed
- Members of `QueryResponseResource` and `EmbeddingResource` made const to ensure immutability.

## [0.3.0] - 2024-06-16
### Added
- `CohereEmbeddingFunction`: Added new embedding function for Cohere.
- `VoyageAIEmbeddingFunction`: Added new embedding function for VoyageAI.
- `TogetherAIEmbeddingFunction`: Added new embedding function for TogetherAI.
- Custom Exceptions for Embedding Provider Request failures: Introduced specific exceptions to handle embedding provider request failures.
- `dimension` parameter for OpenAI Embedding models: Added support for specifying the dimension parameter for third generation OpenAI embedding models.

### Fixed
- Readme namespace: Corrected namespace references in the Readme documentation.
- Character set for embedding requests: Fixed issues with character set handling in embedding requests.

## [0.2.0] - 2024-06-15
### Added
- Custom Exceptions

## [0.1.0] - 2024-06-15
### Added
- Initial release
