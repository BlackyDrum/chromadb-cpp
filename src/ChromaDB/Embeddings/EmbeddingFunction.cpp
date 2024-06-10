#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

	EmbeddingFunction::EmbeddingFunction(const std::string& apiKey, const std::string& model, const std::string& baseUrl, const std::string& path)
		: m_BaseUrl(baseUrl), m_Path(path), m_ApiKey(apiKey), m_Model(model)
	{
	}

} // namespace chromadb