#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

    class OpenAIEmbeddingFunction : public EmbeddingFunction
    {
    public:
        /*
        * @brief Construct a new OpenAIEmbeddingFunction object
        * 
        * @param apiKey The API key
        * @param model The model to use (optional)
        * @param dimensions The number of dimensions of the embeddings (optional)
        */
        OpenAIEmbeddingFunction(const std::string& apiKey, const std::string& model = "text-embedding-3-small", size_t dimensions = 1536);

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
        size_t m_Dimensions;
    };

} // namespace chromadb