#pragma once

namespace chromadb {

    struct EmbeddingResource
    {
        const std::string id;
        const std::shared_ptr<const std::vector<double>> embeddings;
        const std::shared_ptr<const std::unordered_map<std::string, std::string>> metadata;
        const std::shared_ptr<const std::string> document;

        EmbeddingResource(const std::string& id, std::shared_ptr<const std::vector<double>> embeddings = nullptr, std::shared_ptr<const std::unordered_map<std::string, std::string>> metadata = nullptr, std::shared_ptr<const std::string> document = nullptr)
            : id(id), embeddings(embeddings), metadata(metadata), document(document) 
        {
        }
    };

} // namespace chromadb
