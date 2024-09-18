#include "ChromaDB/Resources/Collection.h"

namespace chromadb {

    Collection::Collection(const std::string& name, const std::string& id, const std::unordered_map<std::string, std::string>& metadata, std::shared_ptr<EmbeddingFunction> embeddingFunction)
        : m_Name(name), m_Id(id), m_EmbeddingFunction(embeddingFunction), m_Metadata(metadata)
    {
    }

    void Collection::CheckDeleted() const
    {
        if (m_IsDeleted)
            throw ChromaInvalidCollectionException("Collection " + m_Name + " is already deleted");
    }

} // namespace chromadb