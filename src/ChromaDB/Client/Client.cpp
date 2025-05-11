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
            m_ChromaApiClient.Get(std::format("/api/v2/tenants/{}", m_Tenant));
        }
        catch (ChromaException)
        {
            this->CreateTenant();
        }

        try
        {
            m_ChromaApiClient.Get(std::format("/api/v2/tenants/{}/databases/{}", m_Tenant, m_Database));
        }
        catch (ChromaException)
        {
            this->CreateDatabase();
        }

        m_ChromaApiUrlPrefix = std::format("/api/v2/tenants/{}/databases/{}", m_Tenant, m_Database);
    }

    void Client::CreateTenant()
    {
        nlohmann::json json = {
            { "name", m_Tenant }
        };

        try
        {
            m_ChromaApiClient.Post("/api/v2/tenants", json);
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
            m_ChromaApiClient.Post(std::format("/api/v2/tenants/{}/databases", m_Tenant), json);
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }
    }

    void Client::Reset()
    {
        try
        {
            m_ChromaApiClient.Post("/api/v2/reset", {});
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
            std::string version = m_ChromaApiClient.Get("/api/v2/version").dump();

            version.erase(std::remove_if(version.begin(), version.end(), [](char c) {
                return c == '"';
                }), version.end());

            return version;
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return "";
    }

    size_t Client::GetHeartbeat()
    {
        try 
        {
            return m_ChromaApiClient.Get("/api/v2/heartbeat")["nanosecond heartbeat"];
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return 0;
    }

    bool Client::HealthCheck()
    {
        try
        {
            return m_ChromaApiClient.Get("/api/v2/healthcheck")["is_executor_ready"];
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return false;
    }

    UserIdentity Client::GetUserIdentity()
    {
        try
        {
            auto json = m_ChromaApiClient.Get("/api/v2/auth/identity");

            std::vector<std::string> databases;
            for (const auto& database : json["databases"])
                databases.push_back(database.get<std::string>());

            return UserIdentity(databases, json["tenant"].get<std::string>(), json["user_id"].get<std::string>());
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return UserIdentity({}, "", "");
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
            m_ChromaApiClient.Post(std::format("{}/collections", m_ChromaApiUrlPrefix), json);
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
            auto json = m_ChromaApiClient.Get(std::format("{}/collections/{}", m_ChromaApiUrlPrefix, name));

            std::unordered_map<std::string, std::string> metadata;
            if (!json["metadata"].is_null())
                metadata = json["metadata"].get<std::unordered_map<std::string, std::string>>();

            return Collection(json["name"], json["id"], metadata, embeddingFunction);
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return Collection("", "", {});
    }

    Collection Client::GetOrCreateCollection(const std::string& name, const std::unordered_map<std::string, std::string>& metadata, std::shared_ptr<EmbeddingFunction> embeddingFunction)
    {
        try
        {
            return this->GetCollection(name, embeddingFunction);
        }
        catch (ChromaException)
        {
            return this->CreateCollection(name, metadata, embeddingFunction);
        }
    }

    std::vector<Collection> Client::GetCollections(std::shared_ptr<EmbeddingFunction> embeddingFunction)
    {
        try
        {
            auto json = m_ChromaApiClient.Get(std::format("{}/collections", m_ChromaApiUrlPrefix));

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

        return {};
    }

    size_t Client::GetCollectionCount()
    {
        try
        {
            return m_ChromaApiClient.Get(std::format("{}/collections_count", m_ChromaApiUrlPrefix));
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return 0;
    }

    bool Client::CollectionExists(const std::string& name)
    {
        try
        {
            m_ChromaApiClient.Get(std::format("{}/collections/{}", m_ChromaApiUrlPrefix, name));
            return true;
        }
        catch (ChromaException)
        {
            return false;
        }
    }

    void Client::DeleteCollection(Collection& collection)
    {
        try
        {
            m_ChromaApiClient.Delete(std::format("{}/collections/{}", m_ChromaApiUrlPrefix, collection.GetName()));

            collection.m_IsDeleted = true;
        }
        catch (ChromaNotFoundException& e)
        {
            throw e;
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }
    }

    Collection Client::UpdateCollection(const std::string& oldName, const std::string& newName, const std::unordered_map<std::string, std::string>& newMetadata)
    {
        nlohmann::json json = {
            { "new_name", newName },
            { "new_metadata", newMetadata }
        };

        if (newMetadata.empty())
            json.erase("new_metadata");

        Collection oldCollection = this->GetCollection(oldName);

        try
        {
            m_ChromaApiClient.Put(std::format("{}/collections/{}", m_ChromaApiUrlPrefix, oldCollection.GetId()), json);
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
            json.erase("metadatas");

        if (validationResult.documents.empty())
            json.erase("documents");

        try
        {
            m_ChromaApiClient.Post(std::format("{}/collections/{}/add", m_ChromaApiUrlPrefix, collection.GetId()), json);
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
            json.erase("metadatas");

        if (validationResult.documents.empty())
            json.erase("documents");

        if (validationResult.embeddings.empty())
            json.erase("embeddings");

        try
        {
            m_ChromaApiClient.Post(std::format("{}/collections/{}/update", m_ChromaApiUrlPrefix, collection.GetId()), json);
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
            json.erase("metadatas");

        if (validationResult.documents.empty())
            json.erase("documents");

        try
        {
            m_ChromaApiClient.Post(std::format("{}/collections/{}/upsert", m_ChromaApiUrlPrefix, collection.GetId()), json);
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
            return m_ChromaApiClient.Get(std::format("{}/collections/{}/count", m_ChromaApiUrlPrefix, collection.GetId()));
        }
        catch (ChromaNotFoundException& e)
        {
            throw e;
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return 0;
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
            m_ChromaApiClient.Post(std::format("{}/collections/{}/delete", m_ChromaApiUrlPrefix, collection.GetId()), json);
        }
        catch (ChromaNotFoundException& e)
        {
            throw e;
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
            auto response = m_ChromaApiClient.Post(std::format("{}/collections/{}/get", m_ChromaApiUrlPrefix, collection.GetId()), json);

            std::vector<EmbeddingResource> results;
            for (size_t i = 0; i < response["ids"].size(); i++)
            {
                auto id = response["ids"][i].get<std::string>();

                std::shared_ptr<const std::vector<double>> embeddings = nullptr;
                if (!response["embeddings"].is_null() && !response["embeddings"][i].is_null())
                    embeddings = std::make_shared<const std::vector<double>>(response["embeddings"][i].get<std::vector<double>>());

                std::shared_ptr<const std::unordered_map<std::string, std::string>> metadata = nullptr;
                if (!response["metadatas"].is_null() && !response["metadatas"][i].is_null())
                    metadata = std::make_shared<const std::unordered_map<std::string, std::string>>(response["metadatas"][i].get<std::unordered_map<std::string, std::string>>());

                std::shared_ptr<const std::string> document = nullptr;
                if (!response["documents"].is_null() && !response["documents"][i].is_null())
                    document = std::make_shared<const std::string>(response["documents"][i].get<std::string>());

                results.emplace_back(id, embeddings, metadata, document);
            }

            return results;
        }
        catch (ChromaNotFoundException& e)
        {
            throw e;
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return {};
    }

    std::vector<QueryResponseResource> Client::Query(const Collection& collection, const std::vector<std::string>& queryTexts, const std::vector<std::vector<double>>& queryEmbeddings, size_t nResults, const std::vector<std::string>& include, const nlohmann::json& where_document, const nlohmann::json& where)
    {
        if (collection.GetIsDeleted())
            throw ChromaNotFoundException(std::format("Collection {} has already been deleted", collection.GetName()));

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
            auto response = m_ChromaApiClient.Post(std::format("{}/collections/{}/query", m_ChromaApiUrlPrefix, collection.GetId()), json);

            std::vector<QueryResponseResource> queryResponses;
            for (size_t i = 0; i < response["ids"].size(); i++)
            {
                auto ids = response["ids"][i].get<std::vector<std::string>>();

                std::shared_ptr<std::vector<std::vector<double>>> embeddings = nullptr;
                if (!response["embeddings"].is_null() && !response["embeddings"][i].is_null())
                    embeddings = std::make_shared<std::vector<std::vector<double>>>(response["embeddings"][i].get<std::vector<std::vector<double>>>());

                std::shared_ptr<std::vector<std::unordered_map<std::string, std::string>>> metadatas = nullptr;
                if (!response["metadatas"].is_null() && !response["metadatas"][i].is_null())
                {
                    metadatas = std::make_shared<std::vector<std::unordered_map<std::string, std::string>>>();
                    for (const auto& metadata : response["metadatas"][i])
                    {
                        if (!metadata.is_null())
                            metadatas->emplace_back(metadata.get<std::unordered_map<std::string, std::string>>());
                        else
                            metadatas->emplace_back(std::unordered_map<std::string, std::string>{});
                    }
                }

                std::shared_ptr<std::vector<std::string>> documents = nullptr;
                if (!response["documents"].is_null() && !response["documents"][i].is_null())
                {
                    documents = std::make_shared<std::vector<std::string>>();
                    for (const auto& document : response["documents"][i])
                    {
                        if (!document.is_null())
                            documents->emplace_back(document.get<std::string>());
                        else
                            documents->emplace_back("");
                    }
                }

                std::shared_ptr<std::vector<double>> distances = nullptr;
                if (!response["distances"].is_null() && !response["distances"][i].is_null())
                    distances = std::make_shared<std::vector<double>>(response["distances"][i].get<std::vector<double>>());

                queryResponses.emplace_back(ids, embeddings, metadatas, documents, distances);
            }

            return queryResponses;
        }
        catch (ChromaException& e)
        {
            this->handleChromaApiException(e);
        }

        return {};
    }

    // Source for this function: https://github.com/CodeWithKyrian/chromadb-php
    Client::ValidationResult Client::Validate(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings, const std::vector<std::unordered_map<std::string, std::string>>& metadata, const std::vector<std::string>& documents, bool requireEmbeddingsOrDocuments)
    {
        if (collection.GetIsDeleted())
            throw ChromaNotFoundException(std::format("Collection {} has already been deleted", collection.GetName()));

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

        if (!finalEmbeddings.empty())
        {
            size_t embeddingDimension = finalEmbeddings.front().size();
            for (const auto& embedding : finalEmbeddings)
            {
                if (embedding.size() != embeddingDimension)
                    throw ChromaDimensionalityException("All embeddings must have the same dimension");
            }
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

            throw ChromaInvalidArgumentException(std::format("Expected IDs to be unique, found duplicates for: {}", Utils::Join(duplicates, ", ")));
        }

        return { validatedIds, finalEmbeddings, metadata, documents };
    }

    void Client::handleChromaApiException(const ChromaException& e)
    {
        const auto* connectException = dynamic_cast<const ChromaConnectionException*>(&e);
        if (connectException)
            throw ChromaConnectionException(connectException->what());

        const auto* requestException = dynamic_cast<const ChromaRequestException*>(&e);
        if (requestException)
        {
            nlohmann::json error = nlohmann::json::parse(requestException->what(), nullptr, false);

            if (!error.is_discarded())
            {
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