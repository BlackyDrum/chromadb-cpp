#pragma once

#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

    class TogetherAIEmbeddingFunction : public EmbeddingFunction
    {
    public:
        /*
        * @brief Construct a new TogetherAIEmbeddingFunction object
        *
        * @param apiKey The API key
        * @param model The model to use (optional)
        */
        TogetherAIEmbeddingFunction(const std::string& apiKey, const std::string& model = "togethercomputer/m2-bert-80M-8k-retrieval");

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