#pragma once

namespace chromadb {

	struct EmbeddingResource
	{
		std::string id;
		std::vector<double> embeddings;
		std::unordered_map<std::string, std::string> metadata;
		std::string document;
	};

} // namespace chromadb
