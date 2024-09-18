# Changelog

All notable changes to this project will be documented in this file.

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
