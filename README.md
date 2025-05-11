## ChromaDB C++

**Easily interact with ChromaDB Vector Database in C++**

## Description

[ChromaDB](https://www.trychroma.com/) is an open-source vector database designed for managing and querying high-dimensional vector data efficiently.

ChromaDB C++ lets you easily interact with the ChromaDB Vector Database:

- **Collection Management**: Create, retrieve, update, and delete collections
- **Embedding Management**: Add, get, update, upsert, and delete embeddings
- **Querying**: Perform advanced searches on collections using documents or embeddings
- **Authorization Handling**: Connect securely with authorization tokens

## Example

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
	chromadb::Client client("http", "localhost", "8080");

	std::cout << client.GetVersion() << std::endl;
	std::cout << client.GetHeartbeat() << std::endl;

	chromadb::Collection collection = client.CreateCollection("test_collection");

	std::cout << collection.GetId() << std::endl;

	std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
	std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
	std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

	client.AddEmbeddings(collection, ids, embeddings, metadatas);

	std::vector<chromadb::QueryResponseResource> queryResponse = client.Query(collection, {}, { { 1.0, 2.0, 3.0 } }, 2);

	std::cout << queryResponse[0].ids[0] << std::endl; // ID1
	std::cout << queryResponse[0].metadatas->at(0).at("key1") << std::endl; // value1

	client.DeleteCollection(collection);
}
```

## Running ChromaDB

To use this library, you need to have ChromaDB running either locally or in the cloud.

To run ChromaDB in a docker container, you can use the following command:

```bash
docker container run -p 8080:8000 chromadb/chroma
```

Alternatively, you can use a `docker-compose` file:

```yml
services:
  chromadb:
    image: "chromadb/chroma"
    ports:
      - "8080:8000"
    volumes:
      - chroma-data:/chroma/chroma

volumes:
  chroma-data:
    driver: local
```

Run it with:

```bash
docker-compose up
```

Once running, you can access ChromaDB at: http://localhost:8080

## Installation

This guide will help you build and install the ChromaDB library and run an example project on both Linux and Windows systems.

### Prerequisites

- CMake (version 3.10 or higher)
- A C++ compiler (supporting C++20)
- OpenSSL development libraries

### Building ChromaDB Library

Make sure to run the following command to initialize and update the submodules on both Linux and Windows if not done already:

```bash
git submodule update --init --recursive
```

**Linux**

1. Create a build directory

```bash
mkdir build
cd build
```

2. Run CMake

```bash
cmake ..
```

3. Build the project

```bash
make
```

4. Install the library

```bash
sudo make install
```

**Windows**

1. Create a build directory

```bash
mkdir build
cd build
```

2. Run CMake

```bash
cmake ..
```

3. Build the project

```bash
cmake --build . --config Release
```

4. Install the library (you may need administrator privileges)

```bash
cmake --install .
```

### Running the example project

**Linux**

1. Navigate to the examples directory

```bash
cd examples
```

2. Create a build directory

```bash
mkdir build
cd build
```

3. Run CMake

```bash
cmake ..
```

4. Build the example project

```bash
make
```

5. Run the example

```
./ExampleProject
```

**Windows**

1. Navigate to the examples directory

```bash
cd examples
```

2. Create a build directory

```bash
mkdir build
cd build
```

3. Run CMake

```bash
cmake ..
```

4. Open the generated Visual Studio solution and build it
   - Open `ExampleProject.sln` in Visual Studio
   - Build the project (ensure you build in `Release` mode)

### Notes

- On Windows, ensure that the `chromadb.dll` is copied to the output directory where the `ExampleProject` executable resides. This is handled by the CMake script with a post-build command.
- You may need to adjust the `CMAKE_PREFIX_PATH` in the examples `CMakeLists.txt` if the library and include paths for ChromaDB are different on your system.
- If you encounter any issues with finding the OpenSSL libraries, ensure the `OPENSSL_ROOT_DIR` environment variable is set to the correct path of your OpenSSL installation. You can download OpenSSL pre-built binaries [here](https://github.com/CristiFati/Prebuilt-Binaries/tree/master/OpenSSL/v3.3).

## Testing

To run tests included in the ChromaDB project:

```bash
cd build
ctest -C Release
```

This will execute the tests defined in the `tests` directory, using Google Test.

## Usage

> To use ChromaDB in your project, simply include the header file "ChromaDB/ChromaDB.h". This header provides access to the full suite of functionalities offered by the ChromaDB C++ library.

### Connecting to ChromaDB

To connect to a ChromaDB server, you need to create an instance of the `Client` class. You can specify the connection details such as the scheme, host, port, database, tenant, and an authorization token.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    // Define connection parameters
    std::string scheme = "http";
    std::string host = "localhost";
    std::string port = "8080";
    std::string database = "my_database";
    std::string tenant = "my_tenant";
    std::string authToken = "my_auth_token";

    // Create a ChromaDB client
    chromadb::Client client(scheme, host, port, database, tenant, authToken);
}
```

**Parameters**

- **scheme**: The protocol to use (`http` or `https`).
- **host**: The hostname or IP address of the ChromaDB server.
- **port**: The port number on which the ChromaDB server is running.
- **database**: (Optional) The database to use (defaults to `default_database`). If it does not exist, it will be created.
- **tenant**: (Optional) The tenant to use (defaults to `default_tenant`). If it does not exist, it will be created.
- **authToken**: (Optional) The authorization token to use.

### Authentication

ChromaDB supports static token-based authentication. To use it, start the Chroma server with the necessary environment variables as specified in the [documentation](https://docs.trychroma.com/deployment/auth#static-api-token-authentication).
For Docker Compose, you can use the `env_file` option or pass the environment variables directly as shown below:

```yml
services:
  chroma-with-auth:
    image: "chromadb/chroma"
    ports:
      - "8081:8000"
    volumes:
      - chroma-data:/chroma/chroma
    env_file:
      - .env
    # Alternatively, you can specify the environment variables directly
    environment:
      CHROMA_SERVER_AUTHN_CREDENTIALS: "authToken"
      CHROMA_SERVER_AUTHN_PROVIDER: "chromadb.auth.token_authn.TokenAuthenticationServerProvider"

volumes:
  chroma-data:
    driver: local
```

### Create a Collection

To create a new collection in ChromaDB, use the `CreateCollection` method. This method allows you to specify the name of the collection, optional metadata, and an optional embedding function.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::unordered_map<std::string, std::string> metadata = { {"key1", "value1"}, {"key2", "value2"} };
    std::shared_ptr<chromadb::EmbeddingFunction> embeddingFunction = std::make_shared<chromadb::OpenAIEmbeddingFunction>("openai-api-key");

    chromadb::Collection collection = client.CreateCollection("test_collection", metadata, embeddingFunction);
}
```

**Parameters**

- **name**: The name of the collection to create.
- **metadata**: (Optional) A map of metadata key-value pairs for the collection.
- **embeddingFunction**: (Optional) A shared pointer to an embedding function for the collection.

### Get a Collection

To retrieve an existing collection in ChromaDB, use the `GetCollection` method. This method allows you to specify the name of the collection and an optional embedding function.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    chromadb::Collection collection = client.GetCollection("test_collection");
    std::cout << "Collection name: " << collection.GetName() << std::endl;
    std::cout << "Collection id: " << collection.GetId() << std::endl;

    std::cout << "Collection metadata: " << std::endl;
    for (const auto& [key, value] : collection.GetMetadata())
	std::cout << key << ": " << value << std::endl;
}
```

**Parameters**

- **name**: The name of the collection to retrieve.
- **embeddingFunction**: (Optional) A shared pointer to an embedding function for the collection.

### Get or Create a Collection

To retrieve an existing collection or create a new one in ChromaDB, use the `GetOrCreateCollection` method. This method allows you to specify the name of the collection, optional metadata, and an optional embedding function.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    chromadb::Collection collection = client.GetOrCreateCollection("test_collection");
}
```

**Parameters**

- **name**: The name of the collection to retrieve.
- **metadata**: (Optional) A map of metadata key-value pairs to associate with the collection.
- **embeddingFunction**: (Optional) A shared pointer to an embedding function for the collection.

### Get all Collections

To retrieve all existing collections in ChromaDB, use the `GetCollections` method. This method allows you to specify an optional embedding function that applies to all collections.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::vector<chromadb::Collection> collections = client.GetCollections();

    for (chromadb::Collection& collection : collections)
        std::cout << "Collection name: " << collection.GetName() << std::endl;
}
```

**Parameters**

- **embeddingFunction**: (Optional) A shared pointer to an embedding function for the collections.

### Get Collection Count

To get the total number of collections in ChromaDB, use the `GetCollectionCount` method.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    size_t collectionCount = client.GetCollectionCount();
    std::cout << "Collection count: " << collectionCount << std::endl;
}
```

### Check if Collection exists

To check if a collection exists, use the `CollectionExists` method.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    bool exists = client.CollectionExists("test_collection");
}
```

**Parameters**

- **name**: The name of the collection.

### Update a Collection

To update an existing collection in ChromaDB, use the `UpdateCollection` method. This method allows you to change the name and metadata of a collection.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::string newName = "test_collection_updated";
    std::unordered_map<std::string, std::string> newMetadata = { {"key3", "value3"}, {"key4", "value4"} };
    chromadb::Collection updatedCollection = client.UpdateCollection("test_collection", newName, newMetadata);

    std::cout << updatedCollection.GetName() << std::endl; // "test_collection_updated"
}
```

**Parameters**

- **oldName**: The old name of the collection to update.
- **newName**: The new name for the collection.
- **newMetadata**: (Optional) A map of new metadata key-value pairs for the collection.

### Delete a Collection

To delete an existing collection in ChromaDB, use the `DeleteCollection` method. This method allows you to specify the collection you want to remove.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    chromadb::Collection collection = client.CreateCollection("test_collection");
    client.DeleteCollection(collection);

    collection.GetName(); // Throws exception
}
```

**Parameters**

- **collection**: The collection to delete.

### Add Embeddings to a Collection

To add embeddings to an existing collection in ChromaDB, use the `AddEmbeddings` method.
This method allows you to specify the collection, the IDs of the embeddings, and optionally, the embeddings themselves, metadata, and documents associated with the embeddings.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key2", "value2"} }, { {"key3", "value3"} } };

    client.AddEmbeddings(collection, ids, embeddings, metadatas, {});
}
```

**Parameters**

- **collection**: The collection to which embeddings will be added.
- **ids**: The unique IDs of the embeddings.
- **embeddings**: (Optional) A vector of embeddings.
- **metadata**: (Optional) A vector of metadata key-value pairs associated with the embeddings.
- **documents**: (Optional) A vector of documents associated with the embeddings

> If you do not provide embeddings, you must provide documents along with an embedding function.

### Add Embeddings with Embedding Function

When adding embeddings to a collection in ChromaDB, you can utilize an embedding function to generate embeddings from documents.
You need to either call `embeddingFunction->Generate()` to generate embeddings before manually passing them to `AddEmbeddings`, or simply pass an embedding function directly to a collection, and the embeddings will be generated automatically.

**Manual Generation of Embeddings**

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::OpenAIEmbeddingFunction> embeddingFunction = std::make_shared<chromadb::OpenAIEmbeddingFunction>("openai-api-key");

    chromadb::Collection collection = client.GetCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };
    auto embeddings = embeddingFunction->Generate(documents);

    client.AddEmbeddings(collection, ids, embeddings, {}, documents);
}
```

**Automatic Generation of Embeddings**

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::OpenAIEmbeddingFunction> embeddingFunction = std::make_shared<chromadb::OpenAIEmbeddingFunction>("openai-api-key");

    chromadb::Collection collection = client.GetCollection("test_collection", embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::string> documents = { "document1", "document2", "document3" };

    client.AddEmbeddings(collection, ids, {}, {}, documents);
}
```

We currently support `JinaEmbeddingFunction`, `OpenAIEmbeddingFunction`, `CohereEmbeddingFunction`, `VoyageAIEmbeddingFunction` and `TogetherAIEmbeddingFunction` for this purpose.

**JinaEmbeddingFunction**

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::JinaEmbeddingFunction> jinaEmbeddingFunction = std::make_shared<chromadb::JinaEmbeddingFunction>("jina-api-key");
}
```

**Parameters**

- **apiKey**: The API key to access the API.
- **model**: (Optional) The model to use for generating embeddings. Defaults to `jina-embeddings-v2-base-en`.
- **baseUrl**: (Optional) The base URL of the API server. Defaults to `api.jina.ai`.
- **path**: (Optional) The path of the endpoint for generating embeddings. Defaults to `/v1/embeddings`.

> Note: You can get started immediately by obtaining a free Jina API Key with 1M Tokens [here](https://jina.ai/embeddings/#apiform)

**OpenAIEmbeddingFunction**

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::OpenAIEmbeddingFunction> openAIEmbeddingFunction = std::make_shared<chromadb::OpenAIEmbeddingFunction>("openai-api-key");
}
```

**Parameters**

- **apiKey**: The API key to access the API.
- **model**: (Optional) The model to use for generating embeddings. Defaults to `text-embedding-3-small`.
- **dimensions**: (Optional) The number of dimensions of the embeddings. Defaults to `1536`.
- **baseUrl**: (Optional) The base URL of the API server. Defaults to `api.openai.com`.
- **path**: (Optional) The path of the endpoint for generating embeddings. Defaults to `/v1/embeddings`.

**CohereEmbeddingFunction**

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::CohereEmbeddingFunction> cohereEmbeddingFunction = std::make_shared<chromadb::CohereEmbeddingFunction>("cohere-api-key");
}
```

**Parameters**

- **apiKey**: The API key to access the API.
- **model**: (Optional) The model to use for generating embeddings. Defaults to `embed-english-v3.0`.
- **inputType**: (Optional) The input type passed to the model. Defaults to `classification`.
- **baseUrl**: (Optional) The base URL of the API server. Defaults to `api.cohere.com`.
- **path**: (Optional) The path of the endpoint for generating embeddings. Defaults to `/v1/embed`.

**VoyageAIEmbeddingFunction**

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::VoyageAIEmbeddingFunction> voyageAIEmbeddingFunction = std::make_shared<chromadb::VoyageAIEmbeddingFunction>("voyageai-api-key");
}
```

**Parameters**

- **apiKey**: The API key to access the API.
- **model**: (Optional) The model to use for generating embeddings. Defaults to `voyage-2`.
- **inputType**: (Optional) The input type passed to the model. Defaults to `document`.
- **baseUrl**: (Optional) The base URL of the API server. Defaults to `api.voyageai.com`.
- **path**: (Optional) The path of the endpoint for generating embeddings. Defaults to `/v1/embeddings`.

**TogetherAIEmbeddingFunction**

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::TogetherAIEmbeddingFunction> togetherAIEmbeddingFunction = std::make_shared<chromadb::TogetherAIEmbeddingFunction>("togetherai-api-key");
}
```

**Parameters**

- **apiKey**: The API key to access the API.
- **model**: (Optional) The model to use for generating embeddings. Defaults to `togethercomputer/m2-bert-80M-8k-retrieval`.
- **baseUrl**: (Optional) The base URL of the API server. Defaults to `api.together.xyz`.
- **path**: (Optional) The path of the endpoint for generating embeddings. Defaults to `/v1/embeddings`.

### Getting Additional Metadata from Embedding Requests

If you generated embeddings using an embedding function that calls a provider (e.g., OpenAI or Jina), you can retrieve additional information about the **most recent** request using the `embeddingFunction->GetRequestMetadata()` method. This function returns specific metadata for a provider.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::JinaEmbeddingFunction> embeddingFunction = std::make_shared<chromadb::JinaEmbeddingFunction>("jina-api-key");

    chromadb::Collection collection = client.GetOrCreateCollection("test_collection", {}, embeddingFunction);

    std::vector<std::string> ids = { "ID1", "ID2" };
    std::vector<std::string> documents = { "document1", "document2" };

    client.AddEmbeddings(collection, ids, {}, {}, documents);

    std::cout << embeddingFunction->GetRequestMetadata() << std::endl;
    // Example output (specific for the Jina API):
    // {
    //     "model": "jina-embeddings-v2-base-en",
    //     "object": "list",
    //     "usage": {
    //         "prompt_tokens": 8,
    //         "total_tokens": 8
    //     }
    // }

    // Access specific fields in the additional metadata
    std::cout << embeddingFunction->GetRequestMetadata()["usage"]["prompt_tokens"] << std::endl; // 8
}

```

### Get Embeddings from a Collection

To retrieve embeddings from an existing collection in ChromaDB, use the `GetEmbeddings` method. This method allows you to specify the collection, optional IDs of the embeddings, and optional filters and fields to include in the result.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    chromadb::Collection collection = client.GetCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2" }, { "Document3" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"} }, { {"key1", "value2"} }, { {"key1", "value3"} } };

    client.AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    auto queryResponse = client.GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, {});

    std::cout << queryResponse.size() << std::endl; // 3
    std::cout << queryResponse[0].id << std::endl; // ID1
    std::cout << queryResponse[0].embeddings->at(0) << std::endl; // 1.0
    std::cout << queryResponse[1].metadata->at("key1") << std::endl; // value2
    std::cout << queryResponse[2].document->c_str() << std::endl; // Document3
}
```

**Parameters**

- **collection**: The collection from which to retrieve embeddings.
- **ids**: (Optional) The IDs of the embeddings to retrieve.
- **include**: (Optional) The fields to include in the result (e.g., "metadatas", "documents", "embeddings").
- **where_document**: (Optional) The where clause for filtering documents.
- **where**: (Optional) The where clause for filtering metadata.

You can also filter the results based on their metadata using a `where` clause:

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    nlohmann::json where = { {"key1", "value2"} };
    auto queryResponse = client.GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);

    std::cout << queryResponse.size() << std::endl; // 1
    std::cout << queryResponse[0].id << std::endl; // ID2
}
```

The where clause must be an array of key-value pairs. The key must be a string, and the value can be a string or a nested objects of valid filter values:

- `$eq`: Equals
- `$ne`: Not equals
- `$in`: In
- `$nin`: Not In
- `$and`: And
- `$or`: Or

> The `$gt`, `$gte`, `$lt`, and `$lte` filters are also officially supported in ChromaDB. However, this C++ driver only supports strings for metadata values, so these operators are not available in this implementation yet.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    nlohmann::json where = { {"key1", { {"$ne", "value1"} }} };
    auto queryResponse = client.GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);

    std::cout << queryResponse.size() << std::endl; // 2
    std::cout << queryResponse[0].id << std::endl; // ID2
    std::cout << queryResponse[1].id << std::endl; // ID3
}
```

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    nlohmann::json where = { {"key1", { {"$nin", { "value1", "value3" }} }} };
    auto queryResponse = client.GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);

    std::cout << queryResponse.size() << std::endl; // 1
    std::cout << queryResponse[0].id << std::endl; // ID2
}
```

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    nlohmann::json where = { { "$or", { { {"key1", "value1"} }, { {"key1", "value3"} } } } };
    auto queryResponse = client.GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, {}, where);

    std::cout << queryResponse.size() << std::endl; // 2
    std::cout << queryResponse[0].id << std::endl; // ID1
    std::cout << queryResponse[1].id << std::endl; // ID3
}
```

The `where_document` filter works similarly to the `where` filter but for filtering documents. It additionally supports the `$contains` filter value for more advanced document filtering:

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    chromadb::Collection collection = client.CreateCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2", "ID3", "ID4" };
    std::vector<std::vector<double>> embeddings = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 }, { 10.0, 11.0, 12.0 } };
    std::vector<std::string> documents = { { "Document1" }, { "Document2Special" }, { "Document3" }, { "Document4" } };
    std::vector<std::unordered_map<std::string, std::string>> metadatas = { { {"key1", "value1"}, {"key2", "value2"} }, { {"key1", "value2"}, {"key2", "value3"} }, { {"key1", "value3"}, {"key2", "value4"} }, { {"key1", "value4"}, {"key2", "value5"} } };

    client.AddEmbeddings(collection, ids, embeddings, metadatas, documents);

    nlohmann::json where_document = { {"$and", { {{"$contains", "Document2"}}, {{"$contains", "Special"}} }} };
    auto queryResponse = client.GetEmbeddings(collection, {}, { "embeddings", "documents", "metadatas" }, where_document, {});

    std::cout << queryResponse.size() << std::endl; // 1
    std::cout << queryResponse[0].id << std::endl; // ID2
    std::cout << queryResponse[0].embeddings->at(0) << std::endl; // 4.0
    std::cout << queryResponse[0].embeddings->at(1) << std::endl; // 5.0
    std::cout << queryResponse[0].embeddings->at(2) << std::endl; // 6.0
    std::cout << *queryResponse[0].document << std::endl; // Document2Special
    std::cout << queryResponse[0].metadata->at("key1") << std::endl; // value2
    std::cout << queryResponse[0].metadata->at("key2") << std::endl; // value3
}
```

> If you need guidance on structuring filters such as the `where` clause, refer to `tests/test_client.cpp` for numerous examples.

### Get Embedding Count from a Collection

To retrieve the count of embeddings from an existing collection in ChromaDB, use the `GetEmbeddingCount` method. This method provides a straightforward way to determine the number of embeddings in a collection.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    chromadb::Collection collection = client.CreateCollection("test_collection");

    std::cout << client.GetEmbeddingCount(collection) << std::endl;
}
```

**Parameters**

- **collection**: The collection from which to retrieve the count of embeddings.

### Update Embeddings in a collection

To update embeddings in an existing collection in ChromaDB, use the `UpdateEmbeddings` method. This method allows you to specify the collection, the IDs of the embeddings to update, and optionally, the new embeddings, metadata, and documents associated with the embeddings.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    chromadb::Collection collection = client.GetCollection("test_collection");

    std::vector<std::string> ids = { "ID1", "ID2" };
    std::vector<std::string> new_documents = { "NewDocument1", "NewDocument2" };
    std::vector<std::unordered_map<std::string, std::string>> new_metadatas = { { {"key1", "NewValue3"}, {"key2", "NewValue4"} }, { {"key1", "NewValue4"}, {"key2", "NewValue5"} } };

    client.UpdateEmbeddings(collection, ids, {}, new_metadatas, new_documents);
}
```

**Parameters**

- **collection**: The collection in which to update embeddings.
- **ids**: The IDs of the embeddings to update.
- **embeddings**: (Optional) The new embeddings.
- **metadata**: (Optional) The new metadata associated with the embeddings.
- **documents**: (Optional) The new documents associated with the embeddings.

### Delete Embeddings from a Collection

To delete embeddings from an existing collection in ChromaDB, use the `DeleteEmbeddings` method. This method allows you to specify the collection, the IDs of the embeddings to delete, and optionally, the `where_document` and `where` clauses to filter which embeddings to delete based on document or metadata criteria.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    chromadb::Collection collection = client.GetCollection("test_collection");

    client.DeleteEmbeddings(collection, { "ID1", "ID3" });
}
```

**Parameters**

- **collection**: The collection from which to delete embeddings.
- **ids**: The IDs of the embeddings to delete.
- **where_document**: (Optional) The where clause for filtering which documents to delete.
- **where**: (Optional) The where clause for filtering which metadata to delete.

### Query a collection

To query an existing collection in ChromaDB, use the `Query` method. This method allows you to specify the collection, optional query documents, query embeddings, number of results, fields to include in the results, and optional `where_document` and `where` clauses to filter the query based on document or metadata criteria.

> You MUST either provide queryEmbeddings OR queryDocuments. If you provide queryDocuments, you also need to pass an Embedding Function to the collection.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::shared_ptr<chromadb::JinaEmbeddingFunction> embeddingFunction = std::make_shared<chromadb::JinaEmbeddingFunction>("jina-api-key");

    chromadb::Collection collection = client.GetCollection("test_collection", embeddingFunction); // or collection.SetEmbeddingFunction(embeddingFunction);

    auto queryResponse = client.Query(collection, { "This is a query document" }, {}, 3, { "metadatas", "documents", "embeddings", "distances" });

    for (auto& response : queryResponse)
    {
        for (size_t i = 0; i < response.ids.size(); i++)
        {
            std::cout << response.ids[i] << std::endl;
            std::cout << response.documents->at(i).c_str() << std::endl;

            for (const auto& [key, value] : response.metadatas->at(i))
            {
                std::cout << key << ": " << value << std::endl;
            }
        }
    }
}
```

**Parameters**

- **collection**: The collection to query.
- **queryDocuments**: (Optional) The documents to query.
- **queryEmbeddings**: (Optional) The embeddings to query.
- **nResults**: (Optional) The number of results to return. Defaults to 10.
- **include**: (Optional) The fields to include in the results (e.g., "metadatas", "documents", "embeddings", "distances"). Defaults to "metadatas" and "embeddings".
- **where_document**: (Optional) The where clause for filtering documents.
- **where**: (Optional) The where clause for filtering metadata.

### Get version

To get the version of the ChromaDB server, use the `GetVersion` method.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::string version = client.GetVersion();
    std::cout << "ChromaDB version: " << version << std::endl;
}
```

### Get heartbeat

To get the heartbeat of the ChromaDB server, use the `GetHeartbeat` method.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    size_t heartbeat = client.GetHeartbeat();
    std::cout << "ChromaDB heartbeat: " << heartbeat << std::endl;
}
```

### Get User Identity

To get the user identity of the ChromaDB server, use the `GetUserIdentity` method.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    UserIdentity userIdentity = client.GetUserIdentity();
    std::cout << "User Id: " << userIdentity.userId << std::endl;
    std::cout << "User Tenant: " << userIdentity.tenant << std::endl;

    for (const auto& database : userIdentity.databases)
    {
        std::cout << "Database: " << database << std::endl;
    }
}
```

### Reset ChromaDB

To reset ChromaDB, use the `Reset` method. This will delete all collections and entries.

> Resetting may not be allowed by your configuration. To enable it, include `ALLOW_RESET=TRUE` in your environment variables.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    client.Reset();
}
```

## License

This project is licensed under the MIT License.
