#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

    class JinaEmbeddingFunction : public EmbeddingFunction
    {
    public:
        /*
        * @brief Construct a new JinaEmbeddingFunction object
        * 
        * @param apiKey The API key
        * @param model The model to use (optional)
        * @param baseUrl The base URL of the server (optional)
        * @param path The path of the endpoint (optional)
        */
        JinaEmbeddingFunction(const std::string& apiKey, const std::string& model = "jina-embeddings-v2-base-en", const std::string& baseUrl = "api.jina.ai", const std::string& path = "/v1/embeddings");

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