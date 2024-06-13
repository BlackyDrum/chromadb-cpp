#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

#include <string>
#include <memory>
#include <map>

namespace chromadb {

	class Collection
	{
	public:
		/*
		* @brief Get the name of the collection
		* 
		* @return std::string The name of the collection
		*/
		std::string GetName() const { return m_Name; }

		/*
		* @brief Get the id of the collection
		* 
		* @return std::string The id of the collection
		*/
		std::string GetId() const { return m_Id; }

		/*
		* @brief Get the metadata of the collection
		* 
		* @return std::unordered_map<std::string, std::string> The metadata of the collection
		*/
		std::unordered_map<std::string, std::string> GetMetadata() const { return m_Metadata; }

		/*
		* @brief Set the embedding function of the collection
		* 
		* @param embeddingFunction The embedding function of the collection
		*/
		void SetEmbeddingFunction(std::shared_ptr<EmbeddingFunction> embeddingFunction) { m_EmbeddingFunction = embeddingFunction; }

		/*
		* @brief Get the embedding function of the collection
		* 
		* @return std::shared_ptr<EmbeddingFunction> The embedding function of the collection
		*/
		std::shared_ptr<EmbeddingFunction> GetEmbeddingFunction() const { return m_EmbeddingFunction; }
	private:
		std::string m_Name;
		std::string m_Id;
		std::unordered_map<std::string, std::string> m_Metadata;

		std::shared_ptr<EmbeddingFunction> m_EmbeddingFunction;
	private:
		Collection(const std::string& name, const std::string& id, const std::unordered_map<std::string, std::string>& metadata, std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);

		friend class Client;
	};

} // namespace chromadb