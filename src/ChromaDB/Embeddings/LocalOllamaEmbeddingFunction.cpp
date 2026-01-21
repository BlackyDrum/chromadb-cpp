#include "ChromaDB/Embeddings/LocalOllamaEmbeddingFunction.h"

namespace chromadb {

    LocalOllamaEmbeddingFunction::LocalOllamaEmbeddingFunction(const std::string& address, const std::string& model)
        : EmbeddingFunction("", model, address, "/api/embed")
    {
    }

    std::vector<std::vector<double>> LocalOllamaEmbeddingFunction::Generate(const std::vector<std::string>& documents)
    {
        nlohmann::json body = {
            { "input", documents },
            { "model", m_Model },
        };

        nlohmann::json response = this->Request(body, false);

        std::vector<std::vector<double>> embeddings;
        for (const auto& obj : response["embeddings"])
        {
            embeddings.emplace_back(obj.get<std::vector<double>>());
        }

        nlohmann::json additionalData;
        additionalData["model"] = response["model"];
        additionalData["load_duration"] = response["load_duration"];
        additionalData["prompt_eval_count"] = response["prompt_eval_count"];
        additionalData["total_duration"] = response["total_duration"];
        m_LastRequestAdditionalMetadata = additionalData;

        return embeddings;
    }

    void LocalOllamaEmbeddingFunction::ConfigureClient(httplib::Client& client) const
    {
        client.set_connection_timeout(10, 0);
        client.set_read_timeout(120, 0);
        client.set_write_timeout(120, 0);
    }

} // namespace chromadb
