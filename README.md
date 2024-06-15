## ChromaDB C++
**Easily interact with ChromaDB Vector Database in C++**

## Description
[ChromaDB](https://www.trychroma.com/) is a powerful, high-performance database designed specifically for managing and querying vector embeddings and other high-dimensional data.
Tailored to support machine learning and artificial intelligence applications, ChromaDB offers an efficient and scalable solution for handling
large volumes of complex data, enabling rapid similarity searches and facilitating advanced data analytics.

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

	client.DeleteCollection(collection.GetName());
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
version: '3.9'

services:
    chromadb:
        image: 'chromadb/chroma'
        ports:
            - '8080:8000'
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
- A C++ compiler (supporting C++17)
- OpenSSL development libraries

### Building ChromaDB Library

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
- If you encounter any issues with finding the OpenSSL libraries, ensure the `OPENSSL_ROOT_DIR` environment variable is set to the correct path of your OpenSSL installation.

## Testing
To run tests included in the ChromaDB project:
```bash
cd build
ctest -C Release
```
This will execute the tests defined in the `tests` directory, using Google Test.

## Usage

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
- **database**: The database to use (optional, defaults to `default_database`). If it does not exist, it will be created.
- **tenant**: The tenant to use (optional, defaults to `default_tenant`). If it does not exist, it will be created.
- **authToken**: The authorization token to use (optional).

### Authentication
ChromaDB supports static token-based authentication. To use it, start the Chroma server with the necessary environment variables as specified in the [documentation](https://docs.trychroma.com/deployment/auth#static-api-token-authentication).
For Docker Compose, you can use the `env_file` option or pass the environment variables directly as shown below:

```yml
version: '3.9'

services:
    chroma-with-auth:
        image: 'chromadb/chroma'
        ports:
            - '8081:8000'
        volumes:
            - chroma-data:/chroma/chroma
        env_file: 
            - .env
        # Alternatively, you can specify the environment variables directly
        environment:
            CHROMA_SERVER_AUTHN_CREDENTIALS: 'authToken'
            CHROMA_SERVER_AUTHN_PROVIDER: 'chromadb.auth.token_authn.TokenAuthenticationServerProvider'

volumes:
    chroma-data:
        driver: local

```

### Create a collection
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

### Get a collection
To retrieve an existing collection in ChromaDB, use the `GetCollection` method. This method allows you to specify the name of the collection and an optional embedding function.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    Collection collection = client.GetCollection("test_collection");
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

### Get all collections
To retrieve all existing collections in ChromaDB, use the `GetCollections` method. This method allows you to specify an optional embedding function that applies to all collections.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::vector<Collection> collections = client.GetCollections();

    for (Collection& collection : collections)
        std::cout << "Collection name: " << collection.GetName() << std::endl;
}
```
**Parameters**
- **embeddingFunction**: (Optional) A shared pointer to an embedding function for the collections.

### Get collection count
To get the total number of collections in ChromaDB, use the `GetCollectionCount` method.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    size_t collectionCount = client.GetCollectionCount();
    std::cout << "Collection count: " << collectionCount << std::endl;
}
```

### Update a collection
To update an existing collection in ChromaDB, use the `UpdateCollection` method. This method allows you to change the name and metadata of a collection.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    std::string newName = "test_collection_updated";
    std::unordered_map<std::string, std::string> newMetadata = { {"key3", "value3"}, {"key4", "value4"} };
    Collection updatedCollection = client.UpdateCollection("test_collection", newName, newMetadata);

    std::cout << updatedCollection.GetName() << std::endl; // "test_collection_updated"
}
```
**Parameters**
- **oldName**: The old name of the collection to update.
- **newName**: The new name for the collection.
- **newMetadata**: (Optional) A map of new metadata key-value pairs for the collection.

### Delete a collection
To delete an existing collection in ChromaDB, use the `DeleteCollection` method. This method allows you to specify the name of the collection you want to remove.

```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    client.DeleteCollection("test_collection");
}
```
**Parameters**
- **name**: The name of the collection to delete.

### Delete all collections
To delete all existing collections for the current database, use the ``DeleteCollections`` method.
```cpp
#include "ChromaDB/ChromaDB.h"

int main()
{
    client.DeleteCollections();
}
```
