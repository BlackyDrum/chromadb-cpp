#pragma once

namespace chromadb {

    struct QueryResponseResource
    {
        std::shared_ptr<const std::vector<std::string>> ids;
        std::shared_ptr<const std::vector<std::vector<double>>> embeddings;
        std::shared_ptr<const std::vector<std::unordered_map<std::string, std::string>>> metadatas;
        std::shared_ptr<const std::vector<std::string>> documents;
        std::shared_ptr<const std::vector<double>> distances;
    };

} // namespace chromadb