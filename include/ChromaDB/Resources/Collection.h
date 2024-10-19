#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

#include "ChromaDB/Exceptions/ChromaInvalidCollectionException.h"

#include <string>
#include <memory>
#include <map>
#include <format>

namespace chromadb {

    class Collection
    {
    public:
        /*
        * @brief Get the name of the collection
        * 
        * @return std::string The name of the collection
        * 
        * @throw ChromaInvalidCollectionException If the collection has been deleted
        */
        std::string GetName() const { CheckDeleted(); return m_Name; }

        /*
        * @brief Get the id of the collection
        * 
        * @return std::string The id of the collection
        * 
        * @throw ChromaInvalidCollectionException If the collection has been deleted
        */
        std::string GetId() const { CheckDeleted(); return m_Id; }

        /*
        * @brief Get the metadata of the collection
        * 
        * @return std::unordered_map<std::string, std::string> The metadata of the collection
        * 
        * @throw ChromaInvalidCollectionException If the collection has been deleted
        */
        std::unordered_map<std::string, std::string> GetMetadata() const { CheckDeleted(); return m_Metadata; }

        /*
        * @brief Set the embedding function of the collection
        * 
        * @param embeddingFunction The embedding function of the collection
        * 
        * @throw ChromaInvalidCollectionException If the collection has been deleted
        */
        void SetEmbeddingFunction(std::shared_ptr<EmbeddingFunction> embeddingFunction) { CheckDeleted(); m_EmbeddingFunction = embeddingFunction; }

        /*
        * @brief Get the embedding function of the collection
        * 
        * @return std::shared_ptr<EmbeddingFunction> The embedding function of the collection
        * 
        * @throw ChromaInvalidCollectionException If the collection has been deleted
        */
        std::shared_ptr<EmbeddingFunction> GetEmbeddingFunction() const { CheckDeleted(); return m_EmbeddingFunction; }

        /*
        * @brief Checks if the collection has been deleted
        * 
        * @return bool True if the collection has been deleted, false otherwise
        */
        bool GetIsDeleted() const { return m_IsDeleted; }
    private:
        std::string m_Name;
        std::string m_Id;
        std::unordered_map<std::string, std::string> m_Metadata;

        std::shared_ptr<EmbeddingFunction> m_EmbeddingFunction;

        bool m_IsDeleted = false;
    private:
        Collection(const std::string& name, const std::string& id, const std::unordered_map<std::string, std::string>& metadata, std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);

        void CheckDeleted() const;

        friend class Client;
    };

} // namespace chromadb