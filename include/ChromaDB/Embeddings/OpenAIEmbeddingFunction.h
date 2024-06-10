#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

	class OpenAIEmbeddingFunction : public EmbeddingFunction
	{
	public:
		OpenAIEmbeddingFunction(const std::string& apiKey, const std::string& model = "text-embedding-3-small", const std::string& baseUrl = "api.openai.com", const std::string& path = "/v1/embeddings");

		std::vector<std::vector<double>> Generate(const std::vector<std::string>& documents);
	};

} // namespace chromadb