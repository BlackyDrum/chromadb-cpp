#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

	class JinaEmbeddingFunction : public EmbeddingFunction
	{
	public:
		JinaEmbeddingFunction(const std::string& apiKey, const std::string& model = "jina-embeddings-v2-base-en", const std::string& baseUrl = "api.jina.ai", const std::string& path = "/v1/embeddings");

		std::vector<std::vector<float>> Generate(const std::vector<std::string>& documents);

	private:
		nlohmann::json Request(const nlohmann::json& body) override;
	};

} // namespace chromadb