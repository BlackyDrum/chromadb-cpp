#include "ChromaDB/Client.h"

namespace chromadb {

	Client::Client(const std::string& scheme, const std::string& host, const std::string& port, const std::string& database, const std::string& tenant, const std::string& authToken)
		: m_APIClient(scheme, host, port, authToken), m_Database(database), m_Tenant(tenant)
	{
		this->Initialize();
	}

	void Client::Initialize()
	{
		try
		{
			m_APIClient.Get("/tenants/" + m_Tenant);
		}
		catch (ChromaException)
		{
			this->CreateTenant();
		}

		try
		{
			m_APIClient.Get("/databases/" + m_Database + "?tenant=" + m_Tenant);
		}
		catch (ChromaException)
		{
			this->CreateDatabase();
		}
	}

	void Client::CreateTenant()
	{
		nlohmann::json json = {
			{ "name", m_Tenant }
		};

		m_APIClient.Post("/tenants", json);
	}

	void Client::CreateDatabase()
	{
		nlohmann::json json = {
			{ "name", m_Database }
		};

		m_APIClient.Post("/databases?tenant=" + m_Tenant, json);
	}

	std::string Client::GetVersion()
	{
		std::string version = m_APIClient.Get("/version").dump();

		version.erase(std::remove_if(version.begin(), version.end(), [](char c) {
			return c == '"';
		}), version.end());

		return version;
	}

	size_t Client::GetHeartbeat()
	{
		return m_APIClient.Get("/heartbeat")["nanosecond heartbeat"];
	}

	Collection Client::CreateCollection(const std::string& name, const std::unordered_map<std::string, std::string>& metadata, std::shared_ptr<EmbeddingFunction> embeddingFunction)
	{
		nlohmann::json json = {
			{ "name", name },
			{ "metadata", metadata }	
		};

		if (metadata.empty())
			json.erase("metadata");

		m_APIClient.Post("/collections?tenant=" + m_Tenant + "&database=" + m_Database, json);

		return this->GetCollection(name, embeddingFunction);
	}

	Collection Client::GetCollection(const std::string& name, std::shared_ptr<EmbeddingFunction> embeddingFunction)
	{
		auto json = m_APIClient.Get("/collections/" + name + "?tenant=" + m_Tenant + "&database=" + m_Database);

		std::unordered_map<std::string, std::string> metadata;
		if (!json["metadata"].is_null()) {
			metadata = json["metadata"].get<std::unordered_map<std::string, std::string>>();
		}

		return Collection(json["name"], json["id"], metadata, embeddingFunction);
	}

	size_t Client::GetCollectionCount()
	{
		return m_APIClient.Get("/count_collections?tenant=" + m_Tenant + "&database=" + m_Database);
	}

	void Client::DeleteCollection(const std::string& name)
	{
		m_APIClient.Delete("/collections/" + name + "?tenant=" + m_Tenant + "&database=" + m_Database);
	}

	void Client::UpdateCollection(const std::string& oldName, const std::string& newName, const std::unordered_map<std::string, std::string>& newMetadata)
	{
		nlohmann::json json = {
			{ "new_name", newName },
			{ "new_metadata", newMetadata }
		};

		if (newMetadata.empty())
			json.erase("metadata");

		Collection oldCollection = this->GetCollection(oldName);

		m_APIClient.Put("/collections/" + oldCollection.GetId() + "?tenant=" + m_Tenant + "&database=" + m_Database, json);
	}

} // namespace chromadb