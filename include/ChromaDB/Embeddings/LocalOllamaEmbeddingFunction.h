#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

    class LocalOllamaEmbeddingFunction : public EmbeddingFunction
    {
    public:
        /*
        * @brief Construct a new LocalOllamaEmbeddingFunction object
        *
        * @param address The local Ollama server address (optional)
        * @param model The model to use (optional)
        */
        LocalOllamaEmbeddingFunction(const std::string& address = "localhost:11434", const std::string& model = "deepseek-r1:14b");

        /*
        * @brief Generate the embeddings of the documents
        *
        * @param documents The documents to generate the embeddings
        *
        * @return std::vector<std::vector<double>> The embeddings of the documents
        *
        * @throw ChromaException if something goes wrong
        */
        std::vector<std::vector<double>> Generate(const std::vector<std::string>& documents);
    };

} // namespace chromadb