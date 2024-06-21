#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

    class VoyageAIEmbeddingFunction : public EmbeddingFunction
    {
    public:
        /*
        * @brief Construct a new VoyageAIEmbeddingFunction object
        *
        * @param apiKey The API key
        * @param model The model to use (optional)
        * @param inputType The input type (optional)
        * @param baseUrl The base URL of the server (optional)
        * @param path The path of the endpoint (optional)
        */
        VoyageAIEmbeddingFunction(const std::string& apiKey, const std::string& model = "voyage-2", const std::string& inputType = "document", const std::string& baseUrl = "api.voyageai.com", const std::string& path = "/v1/embeddings");

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

    private:
        std::string m_InputType;
    };

} // namespace chromadb