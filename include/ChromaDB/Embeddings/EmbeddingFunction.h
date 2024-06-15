#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "ChromaDB/Exceptions/ChromaException.h"

#include "Json/json.h"
#include "Http/httplib.h"

#include <string>
#include <vector>

namespace chromadb {

	class EmbeddingFunction
	{
	public:
		EmbeddingFunction(const std::string& apiKey, const std::string& model, const std::string& baseUrl, const std::string& path);

		virtual std::vector<std::vector<double>> Generate(const std::vector<std::string>& documents) = 0;
	protected:
		std::string m_BaseUrl;
		std::string m_Path;
		std::string m_ApiKey;
		std::string m_Model;
	protected:
		nlohmann::json Request(const nlohmann::json& body);
	};

} // namespace chromadb