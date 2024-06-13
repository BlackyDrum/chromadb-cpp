#pragma once

#include "ChromaDB/ChromaApiClient.h"
#include "ChromaDB/Collection.h"
#include "ChromaDB/Utils.h"
#include "ChromaDB/QueryResponseResource.h"

#include "Embeddings/EmbeddingResource.h"

#include "Http/httplib.h"
#include "Json/json.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace chromadb {

	class Client
	{
	public:
		Client(const std::string& scheme, const std::string& host, const std::string& port, const std::string& database = "default_database", const std::string& tenant = "default_tenant", const std::string& authToken = "");

		std::string GetVersion();

		size_t GetHeartbeat();

		std::string GetDatabase() const { return m_Database; }

		std::string GetTenant() const { return m_Tenant; }

		Collection CreateCollection(const std::string& name, const std::unordered_map<std::string, std::string>& metadata = {}, std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);

		Collection GetCollection(const std::string& name, std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);
		
		std::vector<Collection> GetCollections(std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);
		
		size_t GetCollectionCount();
		
		Collection UpdateCollection(const std::string& oldName, const std::string& newName, const std::unordered_map<std::string, std::string>& newMetadata = {});
		
		void DeleteCollection(const std::string& name);

		void DeleteCollections();

		void AddEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings = {}, const std::vector<std::unordered_map<std::string, std::string>>& metadata = {}, const std::vector<std::string>& documents = {});

		std::vector<EmbeddingResource> GetEmbeddings(const Collection& collection, const std::vector<std::string>& ids = {}, const std::vector<std::string>& include = { "metadatas", "documents" }, const nlohmann::json& where_document = {}, const nlohmann::json& where = {});
		
		size_t GetEmbeddingCount(const Collection& collection);
		
		void UpdateEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings = {}, const std::vector<std::unordered_map<std::string, std::string>>& metadata = {}, const std::vector<std::string>& documents = {});

		void UpsertEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings = {}, const std::vector<std::unordered_map<std::string, std::string>>& metadata = {}, const std::vector<std::string>& documents = {});

		void DeleteEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const nlohmann::json& where_document = {}, const nlohmann::json& where = {});

		std::vector<QueryResponseResource> Query(const Collection& collection, const std::vector<std::string>& queryDocuments = {}, const std::vector<std::vector<double>>& queryEmbeddings = {}, size_t nResults = 10, const std::vector<std::string>& include = { "metadatas", "documents", "embeddings", "distances" }, const nlohmann::json& where_document = {}, const nlohmann::json& where = {});
	private:
		ChromaApiClient m_ChromaApiClient;

		std::string m_Database;
		std::string m_Tenant;

		struct ValidationResult
		{
			std::vector<std::string> ids;
			std::vector<std::vector<double>> embeddings;
			std::vector<std::unordered_map<std::string, std::string>> metadatas;
			std::vector<std::string> documents;
		};
	private:
		void Initialize();

		void CreateTenant();

		void CreateDatabase();

		ValidationResult Validate(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings, const std::vector<std::unordered_map<std::string, std::string>>& metadata, const std::vector<std::string>& documents, bool requireEmbeddingsOrDocuments);
	};

} // namespace chromadb