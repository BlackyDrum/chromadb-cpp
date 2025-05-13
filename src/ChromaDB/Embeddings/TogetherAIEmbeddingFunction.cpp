#include "ChromaDB/Embeddings/TogetherAIEmbeddingFunction.h"

namespace chromadb {

    TogetherAIEmbeddingFunction::TogetherAIEmbeddingFunction(const std::string& apiKey, const std::string& model)
        : EmbeddingFunction(apiKey, model, "api.together.xyz", "/v1/embeddings")
    {
    }

    std::vector<std::vector<double>> TogetherAIEmbeddingFunction::Generate(const std::vector<std::string>& documents)
    {
        nlohmann::json body = {
            { "input", documents },
            { "model", m_Model }
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
        additionalData["request_id"] = response["request_id"];
        m_LastRequestAdditionalMetadata = additionalData;

        return embeddings;
    }

} // namespace chromadb