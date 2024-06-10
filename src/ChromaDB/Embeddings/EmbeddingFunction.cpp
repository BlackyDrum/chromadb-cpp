#include "ChromaDB/Embeddings/EmbeddingFunction.h"

namespace chromadb {

	EmbeddingFunction::EmbeddingFunction(const std::string& apiKey, const std::string& model, const std::string& baseUrl, const std::string& path)
		: m_BaseUrl(baseUrl), m_Path(path), m_ApiKey(apiKey), m_Model(model)
	{
	}

	nlohmann::json EmbeddingFunction::Request(const nlohmann::json& body)
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