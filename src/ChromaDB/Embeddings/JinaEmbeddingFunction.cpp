#include "ChromaDB/Embeddings/JinaEmbeddingFunction.h"

namespace chromadb {

    JinaEmbeddingFunction::JinaEmbeddingFunction(const std::string& apiKey, const std::string& model)
        : EmbeddingFunction(apiKey, model, "api.jina.ai", "/v1/embeddings")
    {
    }

    std::vector<std::vector<double>> JinaEmbeddingFunction::Generate(const std::vector<std::string>& documents)
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
        additionalData["usage"] = response["usage"];
        m_LastRequestAdditionalMetadata = additionalData;

        return embeddings;
    }

} // namespace chromadb