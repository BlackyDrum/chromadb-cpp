#include "ChromaDB/Client.h"

namespace chromadb {

	Client::Client(const std::string& scheme, const std::string& host, const std::string& port, const std::string& database, const std::string& tenant, const std::string& authToken)
		: m_APIClient(scheme, host, port, authToken), m_Database(database), m_Tenant(tenant)
	{
		this->Initialize();
	}

	void Client::Initialize()
	{
		try
		{
			m_APIClient.Get("/tenants/" + m_Tenant);
		}
		catch (ChromaException)
		{
			this->CreateTenant();
		}

		try
		{
			m_APIClient.Get("/databases/" + m_Database + "?tenant=" + m_Tenant);
		}
		catch (ChromaException)
		{
			this->CreateDatabase();
		}
	}

	void Client::CreateTenant()
	{
		nlohmann::json json = {
			{ "name", m_Tenant }
		};

		m_APIClient.Post("/tenants", json);
	}

	void Client::CreateDatabase()
	{
		nlohmann::json json = {
			{ "name", m_Database }
		};

		m_APIClient.Post("/databases?tenant=" + m_Tenant, json);
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