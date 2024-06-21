#pragma once

#include "ChromaDB/Client/ChromaApiClient.h"
#include "ChromaDB/Resources/Collection.h"
#include "ChromaDB/Utils/Utils.h"
#include "ChromaDB/Resources/QueryResponseResource.h"

#include "ChromaDB/Resources/EmbeddingResource.h"

#include "ChromaDB/Exceptions/ChromaException.h"
#include "ChromaDB/Exceptions/ChromaAuthorizationException.h"
#include "ChromaDB/Exceptions/ChromaConnectionException.h"
#include "ChromaDB/Exceptions/ChromaDimensionalityException.h"
#include "ChromaDB/Exceptions/ChromaInvalidArgumentException.h"
#include "ChromaDB/Exceptions/ChromaInvalidCollectionException.h"
#include "ChromaDB/Exceptions/ChromaNotFoundException.h"
#include "ChromaDB/Exceptions/ChromaRequestException.h"
#include "ChromaDB/Exceptions/ChromaTypeException.h"
#include "ChromaDB/Exceptions/ChromaUniqueConstraintException.h"
#include "ChromaDB/Exceptions/ChromaValueException.h"

#include "Http/httplib.h"
#include "Json/json.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <regex>

namespace chromadb {

    class Client
    {
    public:
        /*
        * @brief Construct a new Client object
        * 
        * @param scheme The scheme of the server (http or https)
        * @param host The host of the server
        * @param port The port of the server
        * @param database The database to use. If it does not exist, it will be created. (optional)
        * @param tenant The tenant to use. If it does not exist, it will be created. (optional)
        * @param authToken The authorization token to use (optional)
        * 
        * @throw ChromaException if something goes wrong
        */
        Client(const std::string& scheme, const std::string& host, const std::string& port, const std::string& database = "default_database", const std::string& tenant = "default_tenant", const std::string& authToken = "");

        /*
        * @brief Reset ChromaDB
        * 
        * @throw ChromaException if something goes wrong
        */
        void Reset();

        /*
        * @brief Get the version of the server
        * 
        * @return std::string The version of the server
        * 
        * @throw ChromaException if something goes wrong
        */
        std::string GetVersion();

        /*
        * @brief Get the heartbeat of the server
        * 
        * @return size_t The heartbeat of the server
        * 
        * @throw ChromaException if something goes wrong
        */
        size_t GetHeartbeat();

        /*
        * @brief Get the current database name
        * 
        * @return std::string The database name
        */
        std::string GetDatabase() const { return m_Database; }

        /*
        * @brief Get the current tenant name
        * 
        * @return std::string The tenant name
        */
        std::string GetTenant() const { return m_Tenant; }

        /*
        * @brief Create a new collection
        * 
        * @param name The name of the collection
        * @param metadata The metadata of the collection (optional)
        * @param embeddingFunction The embedding function of the collection (optional)
        * 
        * @return Collection The created collection
        * 
        * @throw ChromaException if something goes wrong
        */
        Collection CreateCollection(const std::string& name, const std::unordered_map<std::string, std::string>& metadata = {}, std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);

        /*
        * @brief Get a collection
        * 
        * @param name The name of the collection
        * @param embeddingFunction The embedding function of the collection (optional)
        * 
        * @return Collection The collection
        * 
        * @throw ChromaException if something goes wrong
        */
        Collection GetCollection(const std::string& name, std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);

        /*
        * @brief Get or create a collection
        * 
        * @param name The name of the collection
        * @param metadata The metadata of the collection (optional)
        * @param embeddingFunction The embedding function of the collection (optional)
        * 
        * @return Collection The collection
        * 
        * @throw ChromaException if something goes wrong
        */
        Collection GetOrCreateCollection(const std::string& name, const std::unordered_map<std::string, std::string>& metadata = {}, std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);
        
        /*
        * @brief Get all collections
        * 
        * @param embeddingFunction The embedding function of the collections (optional)
        * 
        * @return std::vector<Collection> The collections
        * 
        * @throw ChromaException if something goes wrong
        */
        std::vector<Collection> GetCollections(std::shared_ptr<EmbeddingFunction> embeddingFunction = nullptr);
        
        /*
        * @brief Get the count of all collections
        * 
        * @return size_t The count of all collections
        * 
        * @throw ChromaException if something goes wrong
        */
        size_t GetCollectionCount();
        
        /*
        * @brief Update a collection
        * 
        * @param oldName The old name of the collection
        * @param newName The new name of the collection
        * @param newMetadata The new metadata of the collection (optional)
        * 
        * @return Collection The updated collection
        * 
        * @throw ChromaException if something goes wrong
        */
        Collection UpdateCollection(const std::string& oldName, const std::string& newName, const std::unordered_map<std::string, std::string>& newMetadata = {});
        
        /*
        * @brief Delete a collection
        * 
        * @param name The name of the collection
        * 
        * @throw ChromaException if something goes wrong
        */
        void DeleteCollection(const std::string& name);

        /*
        * @brief Delete all collections
        * 
        * @throw ChromaException if something goes wrong
        */
        void DeleteCollections();

        /*
        * @brief Add embeddings to a collection
        * 
        * @param collection The collection
        * @param ids The ids of the embeddings
        * @param embeddings The embeddings (optional)
        * @param metadata The metadata of the embeddings (optional)
        * @param documents The documents of the embeddings (optional)
        * 
        * @throw ChromaException if something goes wrong
        */
        void AddEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings = {}, const std::vector<std::unordered_map<std::string, std::string>>& metadata = {}, const std::vector<std::string>& documents = {});

        /*
        * @brief Get embeddings from a collection
        * 
        * @param collection The collection
        * @param ids The ids of the embeddings (optional)
        * @param include The fields to include (metadatas, documents, embeddings) (optional)
        * @param where_document The where clause for the documents (optional)
        * @param where The where clause for the metadatas (optional)
        * 
        * @return std::vector<EmbeddingResource> The embeddings
        * 
        * @throw ChromaException if something goes wrong
        */
        std::vector<EmbeddingResource> GetEmbeddings(const Collection& collection, const std::vector<std::string>& ids = {}, const std::vector<std::string>& include = { "metadatas", "documents" }, const nlohmann::json& where_document = {}, const nlohmann::json& where = {});
        
        /*
        * @brief Get the count of embeddings from a collection
        * 
        * @param collection The collection
        * 
        * @return size_t The count of embeddings
        *	
        * @throw ChromaException if something goes wrong
        */
        size_t GetEmbeddingCount(const Collection& collection);
        
        /*
        * @brief Update embeddings in a collection
        * 
        * @param collection The collection
        * @param ids The ids of the embeddings to update
        * @param embeddings The new embeddings (optional)
        * @param metadata The new metadata of the embeddings (optional)
        * @param documents The new documents of the embeddings (optional)
        * 
        * @throw ChromaException if something goes wrong
        */
        void UpdateEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings = {}, const std::vector<std::unordered_map<std::string, std::string>>& metadata = {}, const std::vector<std::string>& documents = {});

        /*
        * @brief Upsert embeddings in a collection
        * 
        * @param collection The collection
        * @param ids The ids of the embeddings to upsert
        * @param embeddings The new embeddings (optional)
        * @param metadata The new metadata of the embeddings (optional)
        * @param documents The new documents of the embeddings (optional)
        * 
        * @throw ChromaException if something goes wrong
        */
        void UpsertEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const std::vector<std::vector<double>>& embeddings = {}, const std::vector<std::unordered_map<std::string, std::string>>& metadata = {}, const std::vector<std::string>& documents = {});

        /*
        * @brief Delete embeddings from a collection
        * 
        * @param collection The collection
        * @param ids The ids of the embeddings to delete
        * @param where_document The where clause for the documents (optional)
        * @param where The where clause for the metadatas (optional)
        * 
        * @throw ChromaException if something goes wrong
        */
        void DeleteEmbeddings(const Collection& collection, const std::vector<std::string>& ids, const nlohmann::json& where_document = {}, const nlohmann::json& where = {});

        /*
        * @brief Query a collection
        * 
        * @param collection The collection
        * @param queryDocuments The query documents (optional)
        * @param queryEmbeddings The query embeddings (optional)
        * @param nResults The number of results (optional)
        * @param include The fields to include (metadatas, documents, embeddings, distances) (optional)
        * @param where_document The where clause for the documents (optional)
        * @param where The where clause for the metadatas (optional)
        * 
        * @return std::vector<QueryResponseResource> The query response
        * 
        * @throw ChromaException if something goes wrong
        */
        std::vector<QueryResponseResource> Query(const Collection& collection, const std::vector<std::string>& queryDocuments = {}, const std::vector<std::vector<double>>& queryEmbeddings = {}, size_t nResults = 10, const std::vector<std::string>& include = { "metadatas", "embeddings" }, const nlohmann::json& where_document = {}, const nlohmann::json& where = {});
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

        void handleChromaApiException(const ChromaException& e);
    };

} // namespace chromadb