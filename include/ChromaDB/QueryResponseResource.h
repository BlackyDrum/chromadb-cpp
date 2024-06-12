#pragma once

namespace chromadb {

	struct QueryResponseResource
	{
		std::vector<std::string> ids;
		std::vector<std::vector<double>> embeddings;
		std::vector<std::unordered_map<std::string, std::string>> metadatas;
		std::vector<std::string> documents;
		std::vector<double> distances;
	};

} // namespace chromadb