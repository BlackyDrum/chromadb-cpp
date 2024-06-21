#include "ChromaDB/Embeddings/CohereEmbeddingFunction.h"

namespace chromadb {

    CohereEmbeddingFunction::CohereEmbeddingFunction(const std::string& apiKey, const std::string& model, const std::string& inputType, const std::string& baseUrl, const std::string& path)
        : EmbeddingFunction(apiKey, model, baseUrl, path), m_InputType(inputType)
    {
    }

    std::vector<std::vector<double>> CohereEmbeddingFunction::Generate(const std::vector<std::string>& documents)
    {
        nlohmann::json body = {
            { "texts", documents },
            { "model", m_Model },
            { "input_type", m_InputType }
        };

        nlohmann::json response = this->Request(body);

        nlohmann::json additionalData;
        additionalData["response_type"] = response["response_type"];
        additionalData["id"] = response["id"];
        additionalData["meta"] = response["meta"];
        m_LastRequestAdditionalMetadata = additionalData;

        return response["embeddings"].get<std::vector<std::vector<double>>>();
    }

} // namespace chromadb