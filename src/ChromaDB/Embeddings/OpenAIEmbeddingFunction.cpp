#include "ChromaDB/Embeddings/OpenAIEmbeddingFunction.h"

namespace chromadb {

    OpenAIEmbeddingFunction::OpenAIEmbeddingFunction(const std::string& apiKey, const std::string& model, size_t dimensions)
        : EmbeddingFunction(apiKey, model, "api.openai.com", "/v1/embeddings"), m_Dimensions(dimensions)
    {
    }

    std::vector<std::vector<double>> OpenAIEmbeddingFunction::Generate(const std::vector<std::string>& documents)
    {
        nlohmann::json body = {
            { "input", documents },
            { "model", m_Model },
            { "dimensions", m_Dimensions },
        };

        nlohmann::json response = this->Request(body);

        std::vector<std::vector<double>> embeddings;
        for (const auto& obj : response["data"])
        {
            embeddings.emplace_back(obj["embedding"].get<std::vector<double>>());
        }

        nlohmann::json additionalData;
        additionalData["model"] = response["model"];
        additionalData["object"] = response["object"];
        additionalData["usage"] = response["usage"];
        m_LastRequestAdditionalMetadata = additionalData;

        return embeddings;
    }

} // namespace chromadb