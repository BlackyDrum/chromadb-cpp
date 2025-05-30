#include "ChromaDB/Embeddings/VoyageAIEmbeddingFunction.h"

namespace chromadb {

    VoyageAIEmbeddingFunction::VoyageAIEmbeddingFunction(const std::string& apiKey, const std::string& model, const std::string& inputType)
        : EmbeddingFunction(apiKey, model, "api.voyageai.com", "/v1/embeddings"), m_InputType(inputType)
    {
    }

    std::vector<std::vector<double>> VoyageAIEmbeddingFunction::Generate(const std::vector<std::string>& documents)
    {
        nlohmann::json body = {
            { "input", documents },
            { "model", m_Model },
            { "input_type", m_InputType }
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