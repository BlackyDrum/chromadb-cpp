#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

#include <string>
#include <memory>
#include <map>

namespace chromadb {

	class Collection
	{
	public:
		std::string GetName() const { return m_Name; }

		std::string GetId() const { return m_Id; }

		std::unordered_map<std::string, std::string> GetMetadata() const { return m_Metadata; }

		void SetEmbeddingFunction(std::shared_ptr<EmbeddingFunction> embeddingFunction) { m_EmbeddingFunction = embeddingFunction; }

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