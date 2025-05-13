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
        */
        VoyageAIEmbeddingFunction(const std::string& apiKey, const std::string& model = "voyage-2", const std::string& inputType = "document");

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