#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

    class CohereEmbeddingFunction : public EmbeddingFunction
    {
    public:
        /*
        * @brief Construct a new CohereEmbeddingFunction object
        *
        * @param apiKey The API key
        * @param model The model to use (optional)
        * @param inputType The input type (optional)
        */
        CohereEmbeddingFunction(const std::string& apiKey, const std::string& model = "embed-english-v3.0", const std::string& inputType = "classification");

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