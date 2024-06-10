#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "Json/json.h"
#include "Http/httplib.h"

#include "ChromaDB/Exception.h"

#include <string>
#include <vector>

namespace chromadb {

	class EmbeddingFunction
	{
	public:
		EmbeddingFunction(const std::string& apiKey, const std::string& model, const std::string& baseUrl, const std::string& path);

		virtual std::vector<std::vector<float>> Generate(const std::vector<std::string>& documents) = 0;

		virtual nlohmann::json Request(const nlohmann::json& body) = 0;
	protected:
		std::string m_BaseUrl;
		std::string m_Path;
		std::string m_ApiKey;
		std::string m_Model;
	};

} // namespace chromadb