#include "ChromaDB/Client/Client.h"

namespace chromadb {

	Client::Client(const std::string& scheme, const std::string& host, const std::string& port, const std::string& database, const std::string& tenant, const std::string& authToken)
		: m_ChromaApiClient(scheme, host, port, authToken), m_Database(database), m_Tenant(tenant)
	{
		this->Initialize();
	}

	void Client::Initialize()
	{
		try
		{
			m_ChromaApiClient.Get("/tenants/" + m_Tenant);
		}
		catch (ChromaException)
		{
			this->CreateTenant();
		}

		try
		{
			m_ChromaApiClient.Get("/databases/" + m_Database + "?tenant=" + m_Tenant);
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

		try
		{
			m_ChromaApiClient.Post("/tenants", json);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	void Client::CreateDatabase()
	{
		nlohmann::json json = {
			{ "name", m_Database }
		};

		try
		{
			m_ChromaApiClient.Post("/databases?tenant=" + m_Tenant, json);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	std::string Client::GetVersion()
	{
		try
		{
			std::string version = m_ChromaApiClient.Get("/version").dump();

			version.erase(std::remove_if(version.begin(), version.end(), [](char c) {
				return c == '"';
				}), version.end());

			return version;
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	size_t Client::GetHeartbeat()
	{
		try 
		{
			return m_ChromaApiClient.Get("/heartbeat")["nanosecond heartbeat"];
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	Collection Client::CreateCollection(const std::string& name, const std::unordered_map<std::string, std::string>& metadata, std::shared_ptr<EmbeddingFunction> embeddingFunction)
	{
		nlohmann::json json = {
			{ "name", name },
			{ "metadata", metadata }	
		};

		if (metadata.empty())
			json.erase("metadata");

		try
		{
			m_ChromaApiClient.Post("/collections?tenant=" + m_Tenant + "&database=" + m_Database, json);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}

		return this->GetCollection(name, embeddingFunction);
	}

	Collection Client::GetCollection(const std::string& name, std::shared_ptr<EmbeddingFunction> embeddingFunction)
	{
		try
		{
			auto json = m_ChromaApiClient.Get("/collections/" + name + "?tenant=" + m_Tenant + "&database=" + m_Database);

			std::unordered_map<std::string, std::string> metadata;
			if (!json["metadata"].is_null())
				metadata = json["metadata"].get<std::unordered_map<std::string, std::string>>();

			return Collection(json["name"], json["id"], metadata, embeddingFunction);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	std::vector<Collection> Client::GetCollections(std::shared_ptr<EmbeddingFunction> embeddingFunction)
	{
		try
		{
			auto json = m_ChromaApiClient.Get("/collections?tenant=" + m_Tenant + "&database=" + m_Database);

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
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	size_t Client::GetCollectionCount()
	{
		try
		{
			return m_ChromaApiClient.Get("/count_collections?tenant=" + m_Tenant + "&database=" + m_Database);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	void Client::DeleteCollection(const std::string& name)
	{
		try
		{
			m_ChromaApiClient.Delete("/collections/" + name + "?tenant=" + m_Tenant + "&database=" + m_Database);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	void Client::DeleteCollections()
	{
		auto collections = this->GetCollections();

		for (const auto& collection : collections)
			this->DeleteCollection(collection.GetName());
	}

	Collection Client::UpdateCollection(const std::string& oldName, const std::string& newName, const std::unordered_map<std::string, std::string>& newMetadata)
	{
		nlohmann::json json = {
			{ "new_name", newName },
			{ "new_metadata", newMetadata }
		};

		if (newMetadata.empty())
			json.erase("metadata");

		Collection oldCollection = this->GetCollection(oldName);

		try
		{
			m_ChromaApiClient.Put("/collections/" + oldCollection.GetId() + "?tenant=" + m_Tenant + "&database=" + m_Database, json);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}

		return this->GetCollection(newName, oldCollection.GetEmbeddingFunction());
	}

	void Client::AddEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings, const std::vector<std::unordered_map<std::string, std::string>>& metadata, const std::vector<std::string>& documents)
	{
		ValidationResult validationResult = this->Validate(collection, ids, embeddings, metadata, documents, true);

		nlohmann::json json = {
			{ "ids", validationResult.ids },
			{ "embeddings", validationResult.embeddings },
			{ "metadatas", validationResult.metadatas },
			{ "documents", validationResult.documents }
		};

		if (validationResult.metadatas.empty())
			json.erase("metadata");

		if (validationResult.documents.empty())
			json.erase("documents");

		try
		{
			m_ChromaApiClient.Post("/collections/" + collection.GetId() + "/add", json);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	void Client::UpdateEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings, const std::vector<std::unordered_map<std::string, std::string>>& metadata, const std::vector<std::string>& documents)
	{
		ValidationResult validationResult = this->Validate(collection, ids, embeddings, metadata, documents, false);

		nlohmann::json json = {
			{ "ids", validationResult.ids },
			{ "embeddings", validationResult.embeddings },
			{ "metadatas", validationResult.metadatas },
			{ "documents", validationResult.documents }
		};

		if (validationResult.metadatas.empty())
			json.erase("metadata");

		if (validationResult.documents.empty())
			json.erase("documents");

		try
		{
			m_ChromaApiClient.Post("/collections/" + collection.GetId() + "/update", json);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	void Client::UpsertEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings, const std::vector<std::unordered_map<std::string, std::string>>& metadata, const std::vector<std::string>& documents)
	{
		ValidationResult validationResult = this->Validate(collection, ids, embeddings, metadata, documents, true);

		nlohmann::json json = {
			{ "ids", validationResult.ids },
			{ "embeddings", validationResult.embeddings },
			{ "metadatas", validationResult.metadatas },
			{ "documents", validationResult.documents }
		};

		if (validationResult.metadatas.empty())
			json.erase("metadata");

		if (validationResult.documents.empty())
			json.erase("documents");

		try
		{
			m_ChromaApiClient.Post("/collections/" + collection.GetId() + "/upsert", json);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	size_t Client::GetEmbeddingCount(const Collection& collection)
	{
		try
		{
			return m_ChromaApiClient.Get("/collections/" + collection.GetId() + "/count");
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	void Client::DeleteEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const nlohmann::json& where_document, const nlohmann::json& where)
	{
		nlohmann::json json = {
			{ "ids", ids },
			{ "where_document", where_document },
			{ "where", where }
		};

		if (where_document.empty())
			json.erase("where_document");

		if (where.empty())
			json.erase("where");

		try
		{
			m_ChromaApiClient.Post("/collections/" + collection.GetId() + "/delete", json);
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	std::vector<EmbeddingResource> Client::GetEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::string>& include, const nlohmann::json& where_document, const nlohmann::json& where)
	{
		nlohmann::json json = {
			{ "ids", ids },
			{ "include", include },
			{ "where_document", where_document },
			{ "where", where }
		};

		if (ids.empty())
			json.erase("ids");

		if (include.empty())
			json.erase("include");

		if (where_document.empty())
			json.erase("where_document");

		if (where.empty())
			json.erase("where");

		try
		{
			auto response = m_ChromaApiClient.Post("/collections/" + collection.GetId() + "/get", json);

			std::vector<EmbeddingResource> embeddings;
			for (size_t i = 0; i < response["ids"].size(); i++)
			{
				std::shared_ptr<const std::vector<double>> embeddingsPtr;
				if (!response["embeddings"].is_null() && !response["embeddings"][i].is_null())
					embeddingsPtr = std::make_shared<const std::vector<double>>(response["embeddings"][i].get<std::vector<double>>());

				std::shared_ptr<const std::unordered_map<std::string, std::string>> metadata;
				if (!response["metadatas"].is_null() && !response["metadatas"][i].is_null())
					metadata = std::make_shared<const std::unordered_map<std::string, std::string>>(response["metadatas"][i].get<std::unordered_map<std::string, std::string>>());

				std::shared_ptr<const std::string> document;
				if (!response["documents"].is_null() && !response["documents"][i].is_null())
					document = std::make_shared<const std::string>(response["documents"][i].get<std::string>());

				embeddings.emplace_back(response["ids"][i], embeddingsPtr, metadata, document);
			}

			return embeddings;
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	std::vector<QueryResponseResource> Client::Query(const Collection& collection, const std::vector<std::string>& queryTexts, const std::vector<std::vector<double>>& queryEmbeddings, size_t nResults, const std::vector<std::string>& include, const nlohmann::json& where_document, const nlohmann::json& where)
	{
		if (!((!queryEmbeddings.empty() && queryTexts.empty()) || (queryEmbeddings.empty() && !queryTexts.empty()) || (queryEmbeddings.empty() && queryTexts.empty())))
			throw ChromaInvalidArgumentException("You must provide only one of queryEmbeddings or queryTexts");

		std::vector<std::vector<double>> finalEmbeddings;
		if (queryEmbeddings.empty())
		{
			if (collection.m_EmbeddingFunction == nullptr)
				throw ChromaInvalidArgumentException("You must provide an embedding function if you did not provide embeddings");
			if (!queryTexts.empty())
				finalEmbeddings = collection.m_EmbeddingFunction->Generate(queryTexts);
			else
				throw ChromaInvalidArgumentException("If you did not provide embeddings, you must provide texts");
		}
		else
		{
			finalEmbeddings = queryEmbeddings;
		}

		nlohmann::json json = {
			{ "query_embeddings", finalEmbeddings },
			{ "n_results", nResults },
			{ "include", include },
			{ "where_document", where_document },
			{ "where", where }
		};

		if (include.empty())
			json.erase("include");

		if (where_document.empty())
			json.erase("where_document");

		if (where.empty())
			json.erase("where");

		try
		{
			auto response = m_ChromaApiClient.Post("/collections/" + collection.GetId() + "/query", json);

			std::vector<QueryResponseResource> queryResponses;
			for (size_t i = 0; i < response["ids"].size(); i++)
			{
				QueryResponseResource queryResponse;

				queryResponse.ids = response["ids"][i].get<std::vector<std::string>>();

				if (!response["embeddings"].is_null() && !response["embeddings"][i].is_null())
					queryResponse.embeddings = std::make_shared<std::vector<std::vector<double>>>(response["embeddings"][i].get<std::vector<std::vector<double>>>());

				if (!response["metadatas"].is_null() && !response["metadatas"][i].is_null())
				{
					auto metadatas = std::make_shared<std::vector<std::unordered_map<std::string, std::string>>>();
					for (const auto& metadata : response["metadatas"][i])
					{
						if (!metadata.is_null())
							metadatas->push_back(metadata.get<std::unordered_map<std::string, std::string>>());
						else
							metadatas->push_back({});
					}

					queryResponse.metadatas = std::const_pointer_cast<const std::vector<std::unordered_map<std::string, std::string>>>(metadatas);
				}

				if (!response["documents"].is_null() && !response["documents"][i].is_null())
				{
					auto documents = std::make_shared<std::vector<std::string>>();
					for (const auto& document : response["documents"][i])
					{
						if (!document.is_null())
							documents->push_back(document.get<std::string>());
						else
							documents->push_back("");
					}

					queryResponse.documents = std::const_pointer_cast<const std::vector<std::string>>(documents);
				}

				if (!response["distances"].is_null() && !response["distances"][i].is_null())
					queryResponse.distances = std::make_shared<std::vector<double>>(response["distances"][i].get<std::vector<double>>());

				queryResponses.push_back(queryResponse);
			}

			return queryResponses;
		}
		catch (ChromaException& e)
		{
			this->handleChromaApiException(e);
		}
	}

	// Source for this function: https://github.com/CodeWithKyrian/chromadb-php
	Client::ValidationResult Client::Validate(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings, const std::vector<std::unordered_map<std::string, std::string>>& metadata, const std::vector<std::string>& documents, bool requireEmbeddingsOrDocuments)
	{
		if (requireEmbeddingsOrDocuments)
		{
			if (embeddings.empty() && documents.empty())
				throw ChromaInvalidArgumentException("You must provide either embeddings or documents");
		}

		if ((!embeddings.empty() && embeddings.size() != ids.size()) || (!metadata.empty() && metadata.size() != ids.size()) || (!documents.empty() && documents.size() != ids.size()))
			throw ChromaInvalidArgumentException("The number of ids, embeddings, metadatas and documents must be the same");

		std::vector<std::vector<double>> finalEmbeddings;
		if (requireEmbeddingsOrDocuments && embeddings.empty())
		{
			if (collection.m_EmbeddingFunction == nullptr)
				throw ChromaInvalidArgumentException("You must provide an embedding function if you did not provide embeddings");
			else if (!documents.empty())
				finalEmbeddings = collection.m_EmbeddingFunction->Generate(documents);
			else
				throw ChromaInvalidArgumentException("If you did not provide embeddings, you must provide documents");
		}
		else
		{
			finalEmbeddings = embeddings;
		}

		std::vector<std::string> validatedIds = ids;
		std::transform(validatedIds.begin(), validatedIds.end(), validatedIds.begin(), [](const std::string& id) {
			if (id.empty())
				throw ChromaInvalidArgumentException("IDs must be non-empty strings");

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

			throw ChromaInvalidArgumentException("Expected IDs to be unique, found duplicates for: " + Utils::join(duplicates, ", "));
		}

		return { validatedIds, finalEmbeddings, metadata, documents };
	}

	void Client::handleChromaApiException(const ChromaException& e) {
		const auto* connectException = dynamic_cast<const ChromaConnectionException*>(&e);
		if (connectException)
			throw ChromaConnectionException(connectException->what());

		const auto* requestException = dynamic_cast<const ChromaRequestException*>(&e);
		if (requestException)
		{
			nlohmann::json error = nlohmann::json::parse(requestException->what(), nullptr, false);

			if (!error.is_discarded()) {
				std::string errorValue = error.value("error", "");

				std::smatch matches;
				if (std::regex_match(errorValue, matches, std::regex(R"((\w+)\((.*)\))")))
				{
					if (matches.size() > 2)
					{
						std::string error_type = matches[1];
						std::string message = matches[2];

						if (message.front() == '\'' && message.back() == '\'')
							message = message.substr(1, message.size() - 2);

						ChromaException::throwSpecific(message, error_type);
					}
				}

				if (error.contains("detail"))
				{
					std::string message = error["detail"].get<std::string>();
					std::string error_type = ChromaException::inferTypeFromMessage(message);

					ChromaException::throwSpecific(message, error_type);
				}

				if (error.contains("error") && error.contains("message"))
				{
					ChromaException::throwSpecific(error["message"].get<std::string>(), error["error"].get<std::string>());
				}

				if (error.contains("error"))
				{
					std::string message = error["error"].get<std::string>();
					std::string error_type = ChromaException::inferTypeFromMessage(message);

					ChromaException::throwSpecific(message, error_type);
				}
			}
		}

		throw ChromaException(e.what());
	}

} // namespace chromadb