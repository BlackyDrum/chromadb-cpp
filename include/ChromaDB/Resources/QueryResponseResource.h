#pragma once

namespace chromadb {

    struct QueryResponseResource
    {
        const std::vector<std::string> ids;
        const std::shared_ptr<const std::vector<std::vector<double>>> embeddings;
        const std::shared_ptr<const std::vector<std::unordered_map<std::string, std::string>>> metadatas;
        const std::shared_ptr<const std::vector<std::string>> documents;
        const std::shared_ptr<const std::vector<double>> distances;

        QueryResponseResource(const std::vector<std::string>& ids, const std::shared_ptr<const std::vector<std::vector<double>>>& embeddings = nullptr, std::shared_ptr<const std::vector<std::unordered_map<std::string, std::string>>> metadatas = nullptr, std::shared_ptr<const std::vector<std::string>> documents = nullptr, std::shared_ptr<const std::vector<double>> distances = nullptr)
            : ids(ids), embeddings(embeddings), metadatas(metadatas), documents(documents), distances(distances)
        {
        }
    };

} // namespace chromadb