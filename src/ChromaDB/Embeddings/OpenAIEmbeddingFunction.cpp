#include "ChromaDB/Embeddings/OpenAIEmbeddingFunction.h"

namespace chromadb {

	OpenAIEmbeddingFunction::OpenAIEmbeddingFunction(const std::string& apiKey, const std::string& model, const std::string& baseUrl, const std::string& path)
		: EmbeddingFunction(apiKey, model, baseUrl, path)
	{
	}

	std::vector<std::vector<double>> OpenAIEmbeddingFunction::Generate(const std::vector<std::string>& documents)
	{
		nlohmann::json body = {
			{ "input", documents },
			{ "model", m_Model }
		};

		nlohmann::json response = this->Request(body);

		std::vector<std::vector<double>> embeddings;
		for (const auto& obj : response["data"])
		{
			embeddings.push_back(obj["embedding"]);
		}

		return embeddings;
	}

} // namespace chromadb