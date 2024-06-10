#pragma once

#include "ChromaDB/APIClient.h"

#include "Http/httplib.h"
#include "Json/json.h"

#include <string>

namespace chromadb {

	class Client
	{
	public:
		Client(const std::string& scheme, const std::string& host, const std::string& port, const std::string& database = "default_database", const std::string& tenant = "default_tenant", const std::string& authToken = "");
	private:
		APIClient m_APIClient;

		std::string m_Database;
		std::string m_Tenant;
	};

} // namespace chromadb