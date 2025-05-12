#include <gtest/gtest.h>

#include "ChromaDB/ChromaDB.h"

#include <cstdlib>

#pragma warning(disable: 4996)

using namespace chromadb;

class ClientTest : public ::testing::Test {
protected:
    Client* client;

    std::string jinaApiKey;
protected:
    void SetUp() override
    {
        client = new Client("http", "localhost", "8080", "test_database", "test_tenant", "");

        jinaApiKey = GetEnvVar("JINA_API_KEY");
    }

    void TearDown() override
    {
        auto collections = client->GetCollections();

        for (Collection& collection : collections)
            client->DeleteCollection(collection);

        delete client;
    }
private:
    std::string GetEnvVar(const std::string& name)
    {
        const char* value = std::getenv(name.c_str());
        return value == nullptr ? "" : std::string(value);
    }
};

TEST_F(ClientTest, ConstructorInitializesCorrectly)
{
    EXPECT_EQ(client->GetDatabase(), "test_database");
    EXPECT_EQ(client->GetTenant(), "test_tenant");
}

TEST_F(ClientTest, ConstructorThrowsExceptionIfServerIsNotReachable)
{
    EXPECT_THROW(Client("http", "localhost", "8070", "test_database", "test_tenant", "authToken"), ChromaConnectionException);
}

TEST_F(ClientTest, ConstructorDoesNotThrowExceptionIfAuthTokenIsValid)
{
    EXPECT_NO_THROW(Client("http", "localhost", "8081", "test_database", "test_tenant", "authToken"));
}

TEST_F(ClientTest, DISABLED_ConstructorThrowsExceptionIfAuthTokenIsInvalid)
{
    EXPECT_THROW(Client("http", "localhost", "8081", "test_database", "test_tenant", "wrongAuthToken"), ChromaAuthorizationException);
}

TEST_F(ClientTest, ReturnsCorrectVersion)
{
    std::string version = client->GetVersion();
    EXPECT_FALSE(version.empty());
}

TEST_F(ClientTest, ReturnsCorrectHeartbeat)
{
    size_t heartbeat = client->GetHeartbeat();
    EXPECT_GT(heartbeat, 0);
}

TEST_F(ClientTest, CanCreateCollectionWithoutMetadata)
{
    Collection collection = client->CreateCollection("test_collection");
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 0);
    EXPECT_EQ(collection.GetId().empty(), false);

    Collection collection2 = client->GetCollection("test_collection");
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 0);
    EXPECT_EQ(collection2.GetId().empty(), false);

    std::vector<Collection> collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection");
    EXPECT_EQ(collections[0].GetMetadata().size(), 0);
    EXPECT_EQ(collections[0].GetId().empty(), false);
}

TEST_F(ClientTest, CanCreateCollectionWithMetadata)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    Collection collection = client->CreateCollection("test_collection", metadata);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 2);
    EXPECT_EQ(collection.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection.GetId().empty(), false);

    Collection collection2 = client->GetCollection("test_collection");
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 2);
    EXPECT_EQ(collection2.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection2.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection2.GetId().empty(), false);

    std::vector<Collection> collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection");
    EXPECT_EQ(collections[0].GetMetadata().size(), 2);
    EXPECT_EQ(collections[0].GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collections[0].GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collections[0].GetId().empty(), false);
}

TEST_F(ClientTest, CanCreateCollectionWithEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 0);
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), embeddingFunction);

    Collection collection2 = client->GetCollection("test_collection");
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 0);
    EXPECT_EQ(collection2.GetId().empty(), false);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), nullptr);
    collection2.SetEmbeddingFunction(embeddingFunction);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), embeddingFunction);

    std::vector<Collection> collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection");
    EXPECT_EQ(collections[0].GetMetadata().size(), 0);
    EXPECT_EQ(collections[0].GetId().empty(), false);
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), nullptr);
    collections[0].SetEmbeddingFunction(embeddingFunction);
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, CanCreateCollectionWithMetadataAndEmbeddingFunction)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    Collection collection = client->CreateCollection("test_collection", metadata, embeddingFunction);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 2);
    EXPECT_EQ(collection.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), embeddingFunction);

    Collection collection2 = client->GetCollection("test_collection");
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 2);
    EXPECT_EQ(collection2.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection2.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection2.GetId().empty(), false);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), nullptr);
    collection2.SetEmbeddingFunction(embeddingFunction);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), embeddingFunction);

    std::vector<Collection> collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection");
    EXPECT_EQ(collections[0].GetMetadata().size(), 2);
    EXPECT_EQ(collections[0].GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collections[0].GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collections[0].GetId().empty(), false);
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), nullptr);
    collections[0].SetEmbeddingFunction(embeddingFunction);
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, CreateCollectionThrowsExceptionIfCollectionAlreadyExists)
{
    Collection collection = client->CreateCollection("test_collection");

    EXPECT_THROW(client->CreateCollection("test_collection"), ChromaException);
}

TEST_F(ClientTest, CreateCollectionThrowsExceptionIfInvalidNameProvided)
{
    EXPECT_THROW(client->CreateCollection("te"), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, CanGetCollection)
{
    client->CreateCollection("test_collection");

    Collection collection = client->GetCollection("test_collection");
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 0);
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), nullptr);
}

TEST_F(ClientTest, CanGetCollectionWithMetadata)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    Collection collection = client->CreateCollection("test_collection", metadata);

    Collection collection2 = client->GetCollection("test_collection");
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 2);
    EXPECT_EQ(collection2.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection2.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection2.GetId().empty(), false);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), nullptr);
}

TEST_F(ClientTest, CanGetCollectionWithEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    client->CreateCollection("test_collection", {});

    Collection collection = client->GetCollection("test_collection", embeddingFunction);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 0);
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, CanGetCollectionWithMetadataAndEmbeddingFunction)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    client->CreateCollection("test_collection", metadata);

    Collection collection = client->GetCollection("test_collection", embeddingFunction);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 2);
    EXPECT_EQ(collection.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, GetCollectionThrowsExceptionIfCollectionDoesNotExist)
{
    Collection collection = client->CreateCollection("test_collection");

    EXPECT_THROW(client->GetCollection("test_collection2"), ChromaNotFoundException);
}

TEST_F(ClientTest, CanGetOrCreateCollectionWithoutMetadata)
{
    Collection collection = client->GetOrCreateCollection("test_collection");
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 0);
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), nullptr);

    Collection collection2 = client->GetCollection("test_collection");
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 0);
    EXPECT_EQ(collection2.GetId().empty(), false);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), nullptr);
}

TEST_F(ClientTest, CanGetOrCreateCollectionWithMetadata)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    Collection collection = client->GetOrCreateCollection("test_collection", metadata);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 2);
    EXPECT_EQ(collection.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), nullptr);

    Collection collection2 = client->GetCollection("test_collection");
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 2);
    EXPECT_EQ(collection2.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection2.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection2.GetId().empty(), false);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), nullptr);
}

TEST_F(ClientTest, CanGetOrCreateCollectionWithEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    Collection collection = client->GetOrCreateCollection("test_collection", {}, embeddingFunction);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 0);
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), embeddingFunction);

    Collection collection2 = client->GetCollection("test_collection");
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 0);
    EXPECT_EQ(collection2.GetId().empty(), false);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), nullptr);
    collection2.SetEmbeddingFunction(embeddingFunction);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, CanGetOrCreateCollectionWithMetadataAndEmbeddingFunction)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    Collection collection = client->GetOrCreateCollection("test_collection", metadata, embeddingFunction);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 2);
    EXPECT_EQ(collection.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), embeddingFunction);

    Collection collection2 = client->GetCollection("test_collection", embeddingFunction);
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 2);
    EXPECT_EQ(collection2.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection2.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection2.GetId().empty(), false);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, GetOrCreateCollectionDoesNotThrowExceptionIfCollectionAlreadyExists)
{
    Collection collection = client->CreateCollection("test_collection");

    EXPECT_NO_THROW(client->GetOrCreateCollection("test_collection"));
}

TEST_F(ClientTest, GetOrCreateCollectionThrowsExceptionIfInvalidNameProvided)
{
    EXPECT_THROW(client->GetOrCreateCollection("te"), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, CanGetOrCreateCollectionWithMetadataAndEmbeddingFunctionIfCollectionAlreadyExists)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    client->CreateCollection("test_collection", metadata);

    Collection collection = client->GetOrCreateCollection("test_collection", metadata, embeddingFunction);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 2);
    EXPECT_EQ(collection.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), embeddingFunction);

    Collection collection2 = client->GetCollection("test_collection", embeddingFunction);
    EXPECT_EQ(collection2.GetName(), "test_collection");
    EXPECT_EQ(collection2.GetMetadata().size(), 2);
    EXPECT_EQ(collection2.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection2.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection2.GetId().empty(), false);
    EXPECT_EQ(collection2.GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, CanGetCollections)
{
    std::vector<Collection> collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 0);

    Collection collection = client->CreateCollection("test_collection");
    collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection");
    EXPECT_EQ(collections[0].GetMetadata().size(), 0);
    EXPECT_EQ(collections[0].GetId().empty(), false);
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), nullptr);

    Collection collection2 = client->CreateCollection("test_collection2");
    collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 2);

    client->DeleteCollection(collection);
    collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection2");
    EXPECT_EQ(collections[0].GetMetadata().size(), 0);
    EXPECT_EQ(collections[0].GetId().empty(), false);
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), nullptr);

    client->DeleteCollection(collection2);
    collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 0);
}

TEST_F(ClientTest, CanGetCollectionsWithMetadata)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    Collection collection = client->CreateCollection("test_collection", metadata);

    std::vector<Collection> collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection");
    EXPECT_EQ(collections[0].GetMetadata().size(), 2);
    EXPECT_EQ(collections[0].GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collections[0].GetMetadata().at("key2"), "value2");
}

TEST_F(ClientTest, CanGetCollectionsWithEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);

    std::vector<Collection> collections = client->GetCollections(embeddingFunction);
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection");
    EXPECT_EQ(collections[0].GetMetadata().size(), 0);
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, CanGetCollectionsWithMetadataAndEmbeddingFunction)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");
    Collection collection = client->CreateCollection("test_collection", metadata, embeddingFunction);

    std::vector<Collection> collections = client->GetCollections(embeddingFunction);
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection");
    EXPECT_EQ(collections[0].GetMetadata().size(), 2);
    EXPECT_EQ(collections[0].GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collections[0].GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), embeddingFunction);
}

TEST_F(ClientTest, CanUpdateCollection)
{
    Collection collection = client->CreateCollection("test_collection");
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 0);
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), nullptr);

    Collection updatedCollection = client->UpdateCollection("test_collection", "test_collection_updated");
    EXPECT_EQ(updatedCollection.GetName(), "test_collection_updated");
    EXPECT_EQ(updatedCollection.GetMetadata().size(), 0);
    EXPECT_EQ(updatedCollection.GetId(), collection.GetId());
    EXPECT_EQ(updatedCollection.GetEmbeddingFunction(), nullptr);

    Collection collection2 = client->GetCollection("test_collection_updated");
    EXPECT_EQ(collection2.GetName(), "test_collection_updated");
    EXPECT_EQ(collection2.GetMetadata().size(), 0);
    EXPECT_EQ(collection2.GetId(), collection.GetId());
    EXPECT_EQ(collection2.GetEmbeddingFunction(), nullptr);

    EXPECT_THROW(client->GetCollection("test_collection"), ChromaNotFoundException);

    std::vector<Collection> collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection_updated");
    EXPECT_EQ(collections[0].GetMetadata().size(), 0);
    EXPECT_EQ(collections[0].GetId(), collection.GetId());
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), nullptr);
}

TEST_F(ClientTest, CanUpdateCollectionWithMetadata)
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    Collection collection = client->CreateCollection("test_collection", metadata);
    EXPECT_EQ(collection.GetName(), "test_collection");
    EXPECT_EQ(collection.GetMetadata().size(), 2);
    EXPECT_EQ(collection.GetMetadata().at("key1"), "value1");
    EXPECT_EQ(collection.GetMetadata().at("key2"), "value2");
    EXPECT_EQ(collection.GetId().empty(), false);
    EXPECT_EQ(collection.GetEmbeddingFunction(), nullptr);

    std::unordered_map<std::string, std::string> newMetadata = { {"key3", "value3"}, {"key4", "value4"} };
    Collection updatedCollection = client->UpdateCollection("test_collection", "test_collection_updated", newMetadata);
    EXPECT_EQ(updatedCollection.GetName(), "test_collection_updated");
    EXPECT_EQ(updatedCollection.GetMetadata().size(), 2);
    EXPECT_EQ(updatedCollection.GetMetadata().at("key3"), "value3");
    EXPECT_EQ(updatedCollection.GetMetadata().at("key4"), "value4");
    EXPECT_EQ(updatedCollection.GetId(), collection.GetId());
    EXPECT_EQ(updatedCollection.GetEmbeddingFunction(), nullptr);

    Collection collection2 = client->GetCollection("test_collection_updated");
    EXPECT_EQ(collection2.GetName(), "test_collection_updated");
    EXPECT_EQ(collection2.GetMetadata().size(), 2);
    EXPECT_EQ(collection2.GetMetadata().at("key3"), "value3");
    EXPECT_EQ(collection2.GetMetadata().at("key4"), "value4");
    EXPECT_EQ(collection2.GetId(), collection.GetId());
    EXPECT_EQ(collection2.GetEmbeddingFunction(), nullptr);

    EXPECT_THROW(client->GetCollection("test_collection"), ChromaNotFoundException);

    std::vector<Collection> collections = client->GetCollections();
    EXPECT_EQ(collections.size(), 1);
    EXPECT_EQ(collections[0].GetName(), "test_collection_updated");
    EXPECT_EQ(collections[0].GetMetadata().size(), 2);
    EXPECT_EQ(collections[0].GetMetadata().at("key3"), "value3");
    EXPECT_EQ(collections[0].GetMetadata().at("key4"), "value4");
    EXPECT_EQ(collections[0].GetId(), collection.GetId());
    EXPECT_EQ(collections[0].GetEmbeddingFunction(), nullptr);
}

TEST_F(ClientTest, UpdateCollectionThrowsExceptionIfCollectionDoesNotExist)
{
    Collection collection = client->CreateCollection("test_collection");

    EXPECT_THROW(client->UpdateCollection("test_collection2", "test_collection_updated"), ChromaNotFoundException);
}

TEST_F(ClientTest, UpdateCollectionThrowsExceptionIfInvalidNameProvided)
{
    EXPECT_THROW(client->UpdateCollection("test_collection", "te"), ChromaNotFoundException);
}

TEST_F(ClientTest, CanDeleteCollection)
{
    Collection collection = client->CreateCollection("test_collection");

    EXPECT_EQ(client->GetCollectionCount(), 1);

    client->DeleteCollection(collection);

    EXPECT_EQ(client->GetCollectionCount(), 0);
}

TEST_F(ClientTest, DeleteCollectionThrowsExceptionIfCollectionAlreadyDeleted)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(client->DeleteCollection(collection), ChromaNotFoundException);
}

TEST_F(ClientTest, CanGetCollectionCount)
{
    size_t count = client->GetCollectionCount();
    EXPECT_EQ(count, 0);

    Collection collection = client->CreateCollection("test_collection");
    count = client->GetCollectionCount();
    EXPECT_EQ(count, 1);

    Collection collection2 = client->CreateCollection("test_collection2");
    count = client->GetCollectionCount();
    EXPECT_EQ(count, 2);

    client->DeleteCollection(collection);
    count = client->GetCollectionCount();
    EXPECT_EQ(count, 1);

    client->DeleteCollection(collection2);
    count = client->GetCollectionCount();
    EXPECT_EQ(count, 0);
}

TEST_F(ClientTest, CanCheckIfCollectionExists)
{
    EXPECT_FALSE(client->CollectionExists("test_collection"));

    Collection collection = client->CreateCollection("test_collection");

    EXPECT_TRUE(client->CollectionExists("test_collection"));

    client->DeleteCollection(collection);

    EXPECT_FALSE(client->CollectionExists("test_collection"));
}

TEST_F(ClientTest, CanAddEmbeddingsWithEmbeddingsWithoutDocumentsWithoutMetadatas)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };

    client->AddEmbeddings(collection, ids, embeddings, {}, {});

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 1.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 2.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 3.0);
    EXPECT_TRUE(queryResponse[0].document == nullptr);
    EXPECT_TRUE(queryResponse[0].metadata == nullptr);

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 6.0);
    EXPECT_TRUE(queryResponse[1].document == nullptr);
    EXPECT_TRUE(queryResponse[1].metadata == nullptr);

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[2].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(2), 9.0);
    EXPECT_TRUE(queryResponse[2].document == nullptr);
    EXPECT_TRUE(queryResponse[2].metadata == nullptr);
}

TEST_F(ClientTest, CanAddEmbeddingsWithEmbeddingsWithDocumentsWithoutMetadatas)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };

    client->AddEmbeddings(collection, ids, embeddings, {}, documents);

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 1.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 2.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 3.0);
    EXPECT_EQ(*queryResponse[0].document, "Document1");
    EXPECT_TRUE(queryResponse[0].metadata == nullptr);

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[1].document, "Document2");
    EXPECT_TRUE(queryResponse[1].metadata == nullptr);

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[2].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(2), 9.0);
    EXPECT_EQ(*queryResponse[2].document, "Document3");
    EXPECT_TRUE(queryResponse[2].metadata == nullptr);
}

TEST_F(ClientTest, CanAddEmbeddingsWithEmbeddingsWithoutDocumentsWithMetadatas)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key2", "value2"} }, { {"key3", "value3"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, {});

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 1.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 2.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 3.0);
    EXPECT_TRUE(queryResponse[0].document == nullptr);
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value1");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 6.0);
    EXPECT_TRUE(queryResponse[1].document == nullptr);
    EXPECT_EQ(queryResponse[1].metadata->size(), 1);
    EXPECT_EQ(queryResponse[1].metadata->at("key2"), "value2");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[2].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(2), 9.0);
    EXPECT_TRUE(queryResponse[2].document == nullptr);
    EXPECT_EQ(queryResponse[2].metadata->size(), 1);
    EXPECT_EQ(queryResponse[2].metadata->at("key3"), "value3");
}

TEST_F(ClientTest, CanAddEmbeddingsWithEmbeddingsWithDocumentsWithMetadatas)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key2", "value2"} }, { {"key3", "value3"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 1.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 2.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 3.0);
    EXPECT_EQ(*queryResponse[0].document, "Document1");
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value1");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[1].document, "Document2");
    EXPECT_EQ(queryResponse[1].metadata->size(), 1);
    EXPECT_EQ(queryResponse[1].metadata->at("key2"), "value2");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[2].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(2), 9.0);
    EXPECT_EQ(*queryResponse[2].document, "Document3");
    EXPECT_EQ(queryResponse[2].metadata->size(), 1);
    EXPECT_EQ(queryResponse[2].metadata->at("key3"), "value3");
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfCollectionDoesNotExist)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, {}), ChromaNotFoundException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfNotEnoughIdsProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, {}), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfNotEnoughEmbeddingsProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, {}), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfNotEnoughDocumentsProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { "Document1", "Document2" };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, documents), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfNotEnoughMetadatasProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key2", "value2"} } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, metadatas, {}), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfInvalidEmbeddingsSizeProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, {}), ChromaDimensionalityException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfInvalidEmbeddingsProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, {}), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfInvalidMetadatasProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key2", "value2"} } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, metadatas, {}), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfInvalidDocumentsProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, documents), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfIDsAreEmpty)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = {};
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 } };
    std::vector<std::string> documents = { "Document1" };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, documents), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfIDsAreEmptyStrings)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 } };
    std::vector<std::string> documents = { "Document1" };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, documents), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfIDsAreNotUnique)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID1" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
    std::vector<std::string> documents = { "Document1", "Document2" };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, {}, documents), ChromaUniqueConstraintException);
}

TEST_F(ClientTest, AddEmbeddingsThrowsExceptionIfNoDocumentsAndNoEmbeddingsAndNoEmbeddingFunctionProvided)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, {}, {}, {}), ChromaInvalidArgumentException);
}


TEST_F(ClientTest, CanGetEmbeddingsWithWhere)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where = { {"key1", "value2"}};
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);
    EXPECT_EQ(queryResponse.size(), 1);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2");
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhere$eq)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where = { {"key1", { {"$eq", "value1"} }} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);
    EXPECT_EQ(queryResponse.size(), 1);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 1.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 2.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 3.0);
    EXPECT_EQ(*queryResponse[0].document, "Document1");
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value1");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhere$ne)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where = { {"key1", { {"$ne", "value1"} }} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);
    EXPECT_EQ(queryResponse.size(), 2);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2");
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");

    EXPECT_EQ(queryResponse[1].id, "ID3");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 9.0);
    EXPECT_EQ(*queryResponse[1].document, "Document3");
    EXPECT_EQ(queryResponse[1].metadata->size(), 1);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "value3");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhere$in)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where = { {"key1", { {"$in", { "value1", "value3" }} }} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);
    EXPECT_EQ(queryResponse.size(), 2);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 1.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 2.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 3.0);
    EXPECT_EQ(*queryResponse[0].document, "Document1");
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value1");

    EXPECT_EQ(queryResponse[1].id, "ID3");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 9.0);
    EXPECT_EQ(*queryResponse[1].document, "Document3");
    EXPECT_EQ(queryResponse[1].metadata->size(), 1);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "value3");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhere$nin)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where = { {"key1", { {"$nin", { "value1", "value3" }} }} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);
    EXPECT_EQ(queryResponse.size(), 1);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2");
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhere$and)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where = { {"$and", { {{"key1", "value2"}}, {{"key2", "value3"}} }} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);
    EXPECT_EQ(queryResponse.size(), 1);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2");
    EXPECT_EQ(queryResponse[0].metadata->size(), 2);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");
    EXPECT_EQ(queryResponse[0].metadata->at("key2"), "value3");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhere$or)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where = { {"$or", { {{"key1", "value2"}}, {{"key2", "value4"}} }} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);
    EXPECT_EQ(queryResponse.size(), 2);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2");
    EXPECT_EQ(queryResponse[0].metadata->size(), 2);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");
    EXPECT_EQ(queryResponse[0].metadata->at("key2"), "value3");

    EXPECT_EQ(queryResponse[1].id, "ID3");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 9.0);
    EXPECT_EQ(*queryResponse[1].document, "Document3");
    EXPECT_EQ(queryResponse[1].metadata->size(), 2);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "value3");
    EXPECT_EQ(queryResponse[1].metadata->at("key2"), "value4");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhereDocument$contains)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where_document = { {"$contains", "Document2"} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, where_document, {});
    EXPECT_EQ(queryResponse.size(), 1);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2");
    EXPECT_EQ(queryResponse[0].metadata->size(), 2);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");
    EXPECT_EQ(queryResponse[0].metadata->at("key2"), "value3");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhereDocument$contains$and)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2Document3" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where_document = { {"$and", { {{"$contains", "Document2"}}, {{"$contains", "Document3"}} }} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, where_document, {});
    EXPECT_EQ(queryResponse.size(), 1);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2Document3");
    EXPECT_EQ(queryResponse[0].metadata->size(), 2);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");
    EXPECT_EQ(queryResponse[0].metadata->at("key2"), "value3");
}

TEST_F(ClientTest, CanGetEmbeddingsWithWhereDocument$contains$or)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where_document = { {"$or", { {{"$contains", "Document2"}}, {{"$contains", "Document3"}} }} };
    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, where_document, {});
    EXPECT_EQ(queryResponse.size(), 2);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2");
    EXPECT_EQ(queryResponse[0].metadata->size(), 2);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");
    EXPECT_EQ(queryResponse[0].metadata->at("key2"), "value3");

    EXPECT_EQ(queryResponse[1].id, "ID3");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 9.0);
    EXPECT_EQ(*queryResponse[1].document, "Document3");
    EXPECT_EQ(queryResponse[1].metadata->size(), 2);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "value3");
    EXPECT_EQ(queryResponse[1].metadata->at("key2"), "value4");
}

TEST_F(ClientTest, ThrowsIfCollectionDoesNotExistForGet)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, {}), ChromaNotFoundException);
}

TEST_F(ClientTest, CanGetEmbeddingCount)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    EXPECT_EQ(client->GetEmbeddingCount(collection), 4);
}

TEST_F(ClientTest, CanUpdateEmbeddingsOnlyDocumentsAndMetadatas)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    std::vector<std::string> new_documents = { "NewDocument1", "NewDocument2", "NewDocument3", "NewDocument4" };
    std::vector<std::unordered_map<std::string, std::string>> new_metadatas = { { {"key1", "NewValue1"}, {"key2", "NewValue2"} }, { {"key1", "NewValue2"}, {"key2", "NewValue3"} }, { {"key1", "NewValue3"}, {"key2", "NewValue4"} }, { {"key1", "NewValue4"}, {"key2", "NewValue5"} } };

    client->UpdateEmbeddings(collection, ids, {}, new_metadatas, new_documents);

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 4);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 1.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 2.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 3.0);
    EXPECT_EQ(*queryResponse[0].document, "NewDocument1");
    EXPECT_EQ(queryResponse[0].metadata->size(), 2);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "NewValue1");
    EXPECT_EQ(queryResponse[0].metadata->at("key2"), "NewValue2");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[1].document, "NewDocument2");
    EXPECT_EQ(queryResponse[1].metadata->size(), 2);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "NewValue2");
    EXPECT_EQ(queryResponse[1].metadata->at("key2"), "NewValue3");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[2].embeddings->at(0), 7.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(1), 8.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(2), 9.0);
    EXPECT_EQ(*queryResponse[2].document, "NewDocument3");
    EXPECT_EQ(queryResponse[2].metadata->size(), 2);
    EXPECT_EQ(queryResponse[2].metadata->at("key1"), "NewValue3");
    EXPECT_EQ(queryResponse[2].metadata->at("key2"), "NewValue4");

    EXPECT_EQ(queryResponse[3].id, "ID4");
    EXPECT_EQ(queryResponse[3].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[3].embeddings->at(0), 10.0);
    EXPECT_EQ(queryResponse[3].embeddings->at(1), 11.0);
    EXPECT_EQ(queryResponse[3].embeddings->at(2), 12.0);
    EXPECT_EQ(*queryResponse[3].document, "NewDocument4");
    EXPECT_EQ(queryResponse[3].metadata->size(), 2);
    EXPECT_EQ(queryResponse[3].metadata->at("key1"), "NewValue4");
    EXPECT_EQ(queryResponse[3].metadata->at("key2"), "NewValue5");
}

TEST_F(ClientTest, CanUpdateEmbeddingVectors)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    std::vector<std::vector<double>> new_embeddings = { { 13.0, 14.0, 15.0 }, { 16.0, 17.0, 18.0 }, { 19.0, 20.0, 21.0 }, { 22.0, 23.0, 24.0 } };

    client->UpdateEmbeddings(collection, ids, new_embeddings, {}, {});

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 4);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 13.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 14.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 15.0);
    EXPECT_EQ(*queryResponse[0].document, "Document1");
    EXPECT_EQ(queryResponse[0].metadata->size(), 2);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value1");
    EXPECT_EQ(queryResponse[0].metadata->at("key2"), "value2");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 16.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 17.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 18.0);
    EXPECT_EQ(*queryResponse[1].document, "Document2");
    EXPECT_EQ(queryResponse[1].metadata->size(), 2);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "value2");
    EXPECT_EQ(queryResponse[1].metadata->at("key2"), "value3");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[2].embeddings->at(0), 19.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(1), 20.0);
    EXPECT_EQ(queryResponse[2].embeddings->at(2), 21.0);
    EXPECT_EQ(*queryResponse[2].document, "Document3");
    EXPECT_EQ(queryResponse[2].metadata->size(), 2);
    EXPECT_EQ(queryResponse[2].metadata->at("key1"), "value3");
    EXPECT_EQ(queryResponse[2].metadata->at("key2"), "value4");

    EXPECT_EQ(queryResponse[3].id, "ID4");
    EXPECT_EQ(queryResponse[3].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[3].embeddings->at(0), 22.0);
    EXPECT_EQ(queryResponse[3].embeddings->at(1), 23.0);
    EXPECT_EQ(queryResponse[3].embeddings->at(2), 24.0);
    EXPECT_EQ(*queryResponse[3].document, "Document4");
    EXPECT_EQ(queryResponse[3].metadata->size(), 2);
    EXPECT_EQ(queryResponse[3].metadata->at("key1"), "value4");
    EXPECT_EQ(queryResponse[3].metadata->at("key2"), "value5");
}

TEST_F(ClientTest, CanDeleteEmbeddings)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    client->DeleteEmbeddings(collection, { "ID1", "ID3" });

    auto queryResponse = client->GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 2);

    EXPECT_EQ(queryResponse[0].id, "ID2");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[0].embeddings->at(0), 4.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(1), 5.0);
    EXPECT_EQ(queryResponse[0].embeddings->at(2), 6.0);
    EXPECT_EQ(*queryResponse[0].document, "Document2");
    EXPECT_EQ(queryResponse[0].metadata->size(), 2);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value2");
    EXPECT_EQ(queryResponse[0].metadata->at("key2"), "value3");

    EXPECT_EQ(queryResponse[1].id, "ID4");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 3);
    EXPECT_EQ(queryResponse[1].embeddings->at(0), 10.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(1), 11.0);
    EXPECT_EQ(queryResponse[1].embeddings->at(2), 12.0);
    EXPECT_EQ(*queryResponse[1].document, "Document4");
    EXPECT_EQ(queryResponse[1].metadata->size(), 2);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "value4");
    EXPECT_EQ(queryResponse[1].metadata->at("key2"), "value5");
}

TEST_F(ClientTest, CanQueryWithEmbeddings)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    auto queryResponse = client->Query(collection, {}, { { 1.0, 2.0, 3.0 } }, 2, { "embeddings", "documents", "metadatas", "distances" });
    EXPECT_EQ(queryResponse.size(), 1);
    EXPECT_EQ(queryResponse[0].embeddings->size(), 2);
    EXPECT_EQ(queryResponse[0].metadatas->size(), 2);
    EXPECT_EQ(queryResponse[0].documents->size(), 2);
    EXPECT_EQ(queryResponse[0].distances->size(), 2);


    auto queryResponse2 = client->Query(collection, {}, { { 1.0, 2.0, 3.0 }, { 7.0, 8.0, 9.0 } }, 2, { "embeddings", "documents", "metadatas", "distances" });
    EXPECT_EQ(queryResponse2.size(), 2);
    EXPECT_EQ(queryResponse2[0].embeddings->size(), 2);
    EXPECT_EQ(queryResponse2[0].metadatas->size(), 2);
    EXPECT_EQ(queryResponse2[0].documents->size(), 2);
    EXPECT_EQ(queryResponse2[0].distances->size(), 2);
    EXPECT_EQ(queryResponse2[1].embeddings->size(), 2);
    EXPECT_EQ(queryResponse2[1].metadatas->size(), 2);
    EXPECT_EQ(queryResponse2[1].documents->size(), 2);
    EXPECT_EQ(queryResponse2[1].distances->size(), 2);
}

TEST_F(ClientTest, ThrowsIfNoDocumentsAndNoEmbeddingsForQuery)
{
    Collection collection = client->CreateCollection("test_collection");

    EXPECT_THROW(client->Query(collection, {}, {}, 2, { "embeddings", "documents", "metadatas", "distances" }), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, ThrowsIfCollectionDoesNotExistForUpdate)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(client->UpdateEmbeddings(collection, { "ID1", "ID2" }, {}, {}, {}), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionDoesNotExistForDelete)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(client->DeleteEmbeddings(collection, { "ID1", "ID3" }), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionDoesNotExistForQuery)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(client->Query(collection, {}, { { 1.0, 2.0, 3.0 } }, 2, { "embeddings", "documents", "metadatas", "distances" }), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionDoesNotExistForGetEmbeddingCount)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(client->GetEmbeddingCount(collection), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionDoesNotExistForAdd)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, metadatas, documents), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionDoesNotExistForUpsert)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    EXPECT_THROW(client->UpsertEmbeddings(collection, ids, embeddings, metadatas, documents), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionWasDeletedForGetId)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(collection.GetId(), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionWasDeletedForGetName)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(collection.GetName(), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionWasDeletedForGetMetadata)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(collection.GetMetadata(), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionWasDeletedForGetEmbeddingFunction)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    EXPECT_THROW(collection.GetEmbeddingFunction(), ChromaNotFoundException);
}

TEST_F(ClientTest, ThrowsIfCollectionWasDeletedForSetEmbeddingFunction)
{
    Collection collection = client->CreateCollection("test_collection");

    client->DeleteCollection(collection);

    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("jina-api-key");

    EXPECT_THROW(collection.SetEmbeddingFunction(embeddingFunction), ChromaNotFoundException);
}

TEST_F(ClientTest, HasDeletedFlagCorrectlySet)
{
    Collection collection = client->CreateCollection("test_collection");

    EXPECT_FALSE(collection.GetIsDeleted());

    client->DeleteCollection(collection);

    EXPECT_TRUE(collection.GetIsDeleted());
}

TEST_F(ClientTest, ThrowsIfDuplicateIdsForAdd)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID2" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, metadatas, documents), ChromaUniqueConstraintException);
}

TEST_F(ClientTest, ThrowsIfDuplicateIdsForUpsert)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID2" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    EXPECT_THROW(client->UpsertEmbeddings(collection, ids, embeddings, metadatas, documents), ChromaUniqueConstraintException);
}

TEST_F(ClientTest, ThrowsIfDuplicateIdsForUpdate)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID2" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    EXPECT_THROW(client->UpdateEmbeddings(collection, ids, embeddings, metadatas, documents), ChromaUniqueConstraintException);
}

TEST_F(ClientTest, ThrowsIfEmptyIdForAdd)
{
    Collection collection = client->CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, embeddings, metadatas, documents), ChromaInvalidArgumentException);
}

TEST_F(ClientTest, CanAddEmbeddingsUsingEmbeddingFunctionWithManualGeneration)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>(jinaApiKey, "jina-embeddings-v3");

    chromadb::Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };
    auto embeddings = embeddingFunction->Generate(documents);

    EXPECT_EQ(embeddings.size(), 3);
    EXPECT_EQ(embeddings[0].size(), 1024);
    EXPECT_EQ(embeddings[1].size(), 1024);
    EXPECT_EQ(embeddings[2].size(), 1024);

    client->AddEmbeddings(collection, ids, embeddings, {}, documents);

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[0].document, "document1");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[1].document, "document2");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[2].document, "document3");
}

TEST_F(ClientTest, CanAddEmbeddingsUsingEmbeddingFunctionWithAutoGeneration)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>(jinaApiKey, "jina-embeddings-v3");

    chromadb::Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };

    client->AddEmbeddings(collection, ids, {}, {}, documents);

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[0].document, "document1");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[1].document, "document2");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[2].document, "document3");
}

TEST_F(ClientTest, CanAddEmbeddingsUsingEmbeddingFunctionWithManuallySettingEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>(jinaApiKey, "jina-embeddings-v3");

    chromadb::Collection collection = client->CreateCollection("test_collection", {});
    collection.SetEmbeddingFunction(embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };

    client->AddEmbeddings(collection, ids, {}, {}, documents);

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[0].document, "document1");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[1].document, "document2");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[2].document, "document3");
}

TEST_F(ClientTest, CanAddEmbeddingsUsingEmbeddingFunctionWithAutoGenerationAndMetadata)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>(jinaApiKey, "jina-embeddings-v3");

    chromadb::Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    client->AddEmbeddings(collection, ids, {}, metadatas, documents);

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[0].document, "document1");
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value1");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[1].document, "document2");
    EXPECT_EQ(queryResponse[1].metadata->size(), 1);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "value2");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[2].document, "document3");
    EXPECT_EQ(queryResponse[2].metadata->size(), 1);
    EXPECT_EQ(queryResponse[2].metadata->at("key1"), "value3");
}

TEST_F(ClientTest, CanAddEmbeddingsUsingEmbeddingFunctionWithManualGenerationAndMetadata)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>(jinaApiKey, "jina-embeddings-v3");

    chromadb::Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };
    auto embeddings = embeddingFunction->Generate(documents);
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    client->AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    auto queryResponse = client->GetEmbeddings(collection, ids, { "embeddings", "documents", "metadatas" }, {}, {});
    EXPECT_EQ(queryResponse.size(), 3);

    EXPECT_EQ(queryResponse[0].id, "ID1");
    EXPECT_EQ(queryResponse[0].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[0].document, "document1");
    EXPECT_EQ(queryResponse[0].metadata->size(), 1);
    EXPECT_EQ(queryResponse[0].metadata->at("key1"), "value1");

    EXPECT_EQ(queryResponse[1].id, "ID2");
    EXPECT_EQ(queryResponse[1].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[1].document, "document2");
    EXPECT_EQ(queryResponse[1].metadata->size(), 1);
    EXPECT_EQ(queryResponse[1].metadata->at("key1"), "value2");

    EXPECT_EQ(queryResponse[2].id, "ID3");
    EXPECT_EQ(queryResponse[2].embeddings->size(), 1024);
    EXPECT_EQ(*queryResponse[2].document, "document3");
    EXPECT_EQ(queryResponse[2].metadata->size(), 1);
    EXPECT_EQ(queryResponse[2].metadata->at("key1"), "value3");
}

TEST_F(ClientTest, ThrowsIfWrongApiKeyForEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>("wrong-api-key", "jina-embeddings-v3");

    chromadb::Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, {}, {}, documents), EmbeddingProviderRequestException);
}

TEST_F(ClientTest, ThrowsIfWrongEmbeddingModelNameForEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>(jinaApiKey, "wrong-embedding-model");

    chromadb::Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, {}, {}, documents), EmbeddingProviderRequestException);
}

TEST_F(ClientTest, ThrowsIfWrongHostForEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>(jinaApiKey, "jina-embeddings-v3", "wrong-host");

    chromadb::Collection collection = client->CreateCollection("test_collection", {}, embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };

    EXPECT_THROW(client->AddEmbeddings(collection, ids, {}, {}, documents), EmbeddingProviderConnectionException);
}

TEST_F(ClientTest, CanGetRequestMetadataFromEmbeddingFunction)
{
    std::shared_ptr<EmbeddingFunction> embeddingFunction = std::make_shared<JinaEmbeddingFunction>(jinaApiKey, "jina-embeddings-v3");
    auto embeddings = embeddingFunction->Generate({ "document1", "document2", "document3" });

    auto requestMetadata = embeddingFunction->GetRequestMetadata();

    EXPECT_EQ(requestMetadata["model"], "jina-embeddings-v3");
    EXPECT_EQ(requestMetadata["object"], "list");
    EXPECT_GT(requestMetadata["usage"]["prompt_tokens"], 0);
    EXPECT_GT(requestMetadata["usage"]["total_tokens"], 0);
}

TEST_F(ClientTest, CanCheckHealth)
{
    EXPECT_TRUE(client->HealthCheck());
}

TEST_F(ClientTest, CanGetUserIdentity)
{
	UserIdentity userIdentity = client->GetUserIdentity();
	EXPECT_EQ(userIdentity.userId, "");
	EXPECT_EQ(userIdentity.tenant, "default_tenant");
	EXPECT_EQ(userIdentity.databases.size(), 1);
	EXPECT_EQ(userIdentity.databases[0], "default_database");
}