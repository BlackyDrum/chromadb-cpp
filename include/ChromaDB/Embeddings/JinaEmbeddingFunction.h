#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

	class JinaEmbeddingFunction : public EmbeddingFunction
	{
	public:
		JinaEmbeddingFunction(const std::string& apiKey, const std::string& model = "jina-embeddings-v2-base-en", const std::string& baseUrl = "api.jina.ai", const std::string& path = "/v1/embeddings");

		std::vector<std::vector<double>> Generate(const std::vector<std::string>& documents);
	};

} // namespace chromadb