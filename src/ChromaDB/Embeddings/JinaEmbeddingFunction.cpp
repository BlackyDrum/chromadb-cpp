#include "ChromaDB/Embeddings/JinaEmbeddingFunction.h"

namespace chromadb {

	JinaEmbeddingFunction::JinaEmbeddingFunction(const std::string& apiKey, const std::string& model, const std::string& baseUrl, const std::string& path)
		: EmbeddingFunction(apiKey, model, baseUrl, path)
	{
	}

	std::vector<std::vector<double>> JinaEmbeddingFunction::Generate(const std::vector<std::string>& documents)
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

	nlohmann::json JinaEmbeddingFunction::Request(const nlohmann::json& body)
	{
		httplib::SSLClient sslClient(m_BaseUrl);

		httplib::Headers headers = {
			{ "Content-Type", "application/json" },
			{ "Authorization", "Bearer " + m_ApiKey }
		};

		httplib::Result res = sslClient.Post(m_Path, headers, body.dump(), "application/json");
		if (res)
		{
			if (res->status == httplib::OK_200)
			{
				return nlohmann::json::parse(res->body);
			}
			
			throw ChromaException(res->body);
		}

		throw ChromaException(httplib::to_string(res.error()));
	}

} // namespace chromadb