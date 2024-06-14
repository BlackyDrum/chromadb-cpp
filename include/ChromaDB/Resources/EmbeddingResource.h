#pragma once

namespace chromadb {

    struct EmbeddingResource
    {
        const std::string id;
        std::shared_ptr<const std::vector<double>> embeddings;
        std::shared_ptr<const std::unordered_map<std::string, std::string>> metadata;
        std::shared_ptr<const std::string> document;

        EmbeddingResource(const std::string& id, std::shared_ptr<const std::vector<double>> embeddings, std::shared_ptr<const std::unordered_map<std::string, std::string>> metadata, std::shared_ptr<const std::string> document)
            : id(id), embeddings(embeddings), metadata(metadata), document(document) 
        {
        }
    };

} // namespace chromadb
