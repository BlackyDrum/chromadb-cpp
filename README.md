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
