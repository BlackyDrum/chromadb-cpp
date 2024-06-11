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
		if (!json["metadata"].is_null())
			metadata = json["metadata"].get<std::unordered_map<std::string, std::string>>();

		return Collection(json["name"], json["id"], metadata, embeddingFunction);
	}

	std::vector<Collection> Client::GetCollections(std::shared_ptr<EmbeddingFunction> embeddingFunction)
	{
		auto json = m_APIClient.Get("/collections?tenant=" + m_Tenant + "&database=" + m_Database);

		std::vector<Collection> collections;
		for (const auto& collection : json)
		{
			std::unordered_map<std::string, std::string> metadata;
			if (!collection["metadata"].is_null())
				metadata = collection["metadata"].get<std::unordered_map<std::string, std::string>>();

			collections.push_back(Collection(collection["name"], collection["id"], metadata, embeddingFunction));
		}

		return collections;
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

	void Client::AddEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings, const std::vector<std::unordered_map<std::string, std::string>>& metadata, const std::vector<std::string>& documents)
	{
		ValidationResult validationResult = this->Validate(collection, ids, embeddings, metadata, documents);

		nlohmann::json json = {
			{ "ids", validationResult.ids },
			{ "embeddings", validationResult.embeddings },
			{ "metadata", validationResult.metadatas },
			{ "documents", validationResult.documents }
		};

		if (validationResult.metadatas.empty())
			json.erase("metadata");

		if (validationResult.documents.empty())
			json.erase("documents");

		m_APIClient.Post("/collections/" + collection.GetId() + "/add", json);
	}

	size_t Client::GetEmbeddingCount(const Collection& collection)
	{
		return m_APIClient.Get("/collections/" + collection.GetId() + "/count");
	}

	// Source for this function: https://github.com/CodeWithKyrian/chromadb-php
	Client::ValidationResult Client::Validate(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings, const std::vector<std::unordered_map<std::string, std::string>>& metadata, const std::vector<std::string>& documents)
	{
		if (embeddings.empty() && documents.empty())
			throw ChromaException("You must provide either embeddings or documents");

		if ((!embeddings.empty() && embeddings.size() != ids.size()) || (!metadata.empty() && metadata.size() != ids.size()) || (!documents.empty() && documents.size() != ids.size()))
			throw ChromaException("The number of ids, embeddings, metadatas and documents must be the same");

		std::vector<std::vector<double>> finalEmbeddings;
		if (embeddings.empty())
		{
			if (collection.m_EmbeddingFunction == nullptr)
				throw ChromaException("You must provide an embedding function if you did not provide embeddings");
			else if (!documents.empty())
				finalEmbeddings = collection.m_EmbeddingFunction->Generate(documents);
			else
				throw ChromaException("If you did not provide embeddings, you must provide documents");
		}
		else
		{
			finalEmbeddings = embeddings;
		}

		std::vector<std::string> validatedIds = ids;
		std::transform(validatedIds.begin(), validatedIds.end(), validatedIds.begin(), [](const std::string& id) {
			if (id.empty())
				throw ChromaException("IDs must be non-empty strings");

			return id;
			});

		std::unordered_set<std::string> uniqueIds(validatedIds.begin(), validatedIds.end());
		if (uniqueIds.size() != validatedIds.size())
		{
			std::unordered_set<std::string> duplicates;
			for (const auto& id : validatedIds)
			{
				if (std::count(validatedIds.begin(), validatedIds.end(), id) > 1)
					duplicates.insert(id);
			}

			throw ChromaException("Expected IDs to be unique, found duplicates for: " + Utils::join(duplicates, ", "));
		}

		return { validatedIds, finalEmbeddings, metadata, documents };
	}

} // namespace chromadb