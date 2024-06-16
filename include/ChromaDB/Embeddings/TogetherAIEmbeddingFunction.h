#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

	class TogetherAIEmbeddingFunction : public EmbeddingFunction
	{
	public:
		/*
		* @brief Construct a new TogetherAIEmbeddingFunction object
		*
		* @param apiKey The API key
		* @param model The model to use (optional)
		* @param baseUrl The base URL of the server (optional)
		* @param path The path of the endpoint (optional)
		*/
		TogetherAIEmbeddingFunction(const std::string& apiKey, const std::string& model = "togethercomputer/m2-bert-80M-8k-retrieval", const std::string& baseUrl = "api.together.xyz", const std::string& path = "/v1/embeddings");

		/*
		* @brief Generate the embeddings of the documents
		*
		* @param documents The documents to generate the embeddings
		*
		* @return std::vector<std::vector<double>> The embeddings of the documents
		*
		* @throw ChromaException if something goes wrong
		*/
		std::vector<std::vector<double>> Generate(const std::vector<std::string>& documents);
	};

} // namespace chromadb