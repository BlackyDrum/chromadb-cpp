#include "ChromaDB/Collection.h"

namespace chromadb {

	Collection::Collection(const std::string& name, const std::string& id, const std::unordered_map<std::string, std::string>& metadata, std::shared_ptr<EmbeddingFunction> embeddingFunction)
		: m_Name(name), m_Id(id), m_EmbeddingFunction(embeddingFunction), m_Metadata(metadata)
	{
	}

} // namespace chromadb