#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "Exception.h"

#include "Http/httplib.h"
#include "Json/json.h"

#include <string>

namespace chromadb {

	class APIClient
	{
	public:
		APIClient(const std::string& scheme, const std::string& host, const std::string& port, const std::string& authToken);

		nlohmann::json Get(const std::string& endpoint);

		nlohmann::json Post(const std::string& endpoint, const nlohmann::json& body);

		nlohmann::json Put(const std::string& endpoint, const nlohmann::json& body);

		nlohmann::json Delete(const std::string& endpoint);
	private:
		std::string m_Scheme;
		std::string m_Host;
		std::string m_Port;
		std::string m_AuthToken;
		std::string m_BaseUrl;

		const std::string m_Prefix = "/api/v1";
	};

} // namespace chromadb