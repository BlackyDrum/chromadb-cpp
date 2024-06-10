#include "ChromaDB/Client.h"

namespace chromadb {

	Client::Client(const std::string& scheme, const std::string& host, const std::string& port, const std::string& database, const std::string& tenant, const std::string& authToken)
		: m_APIClient(scheme, host, port, authToken), m_Database(database), m_Tenant(tenant)
	{
	}

	std::string Client::GetVersion()
	{
		std::string version = m_APIClient.Get("/version").dump();

		version.erase(std::remove_if(version.begin(), version.end(), [](char c) {
			return c == '"';
		}), version.end());

		return version;
	}

	size_t Client::GetHeartbeat()
	{
		return m_APIClient.Get("/heartbeat")["nanosecond heartbeat"];
	}

} // namespace chromadb