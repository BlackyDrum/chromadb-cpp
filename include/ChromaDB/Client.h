#pragma once

#include "ChromaDB/APIClient.h"
#include "ChromaDB/Collection.h"

#include "Http/httplib.h"
#include "Json/json.h"

#include <string>
#include <vector>
#include <unordered_map>

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

		size_t GetCollectionCount();

		void DeleteCollection(const std::string& name);

		void UpdateCollection(const std::string & oldName, const std::string & newName, const std::unordered_map<std::string, std::string>& newMetadata = {});
	private:
		APIClient m_APIClient;

		std::string m_Database;
		std::string m_Tenant;
		std::vector<std::vector<std::string>> m_Metadata;
	private:
		void Initialize();

		void CreateTenant();

		void CreateDatabase();
	};

} // namespace chromadb