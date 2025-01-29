#include "ChromaDB/Client/ChromaApiClient.h"

namespace chromadb {

    ChromaApiClient::ChromaApiClient(const std::string& scheme, const std::string& host, const std::string& port, const std::string& authToken)
        : m_Scheme(scheme), m_Host(host), m_Port(port), m_AuthToken(authToken)
    {
        m_BaseUrl = std::format("{}://{}:{}", m_Scheme, m_Host, m_Port);
    }

    nlohmann::json ChromaApiClient::Get(const std::string& endpoint)
    {
        httplib::Client client(m_BaseUrl);
        httplib::SSLClient sslClient(m_Host);

        bool https = m_Scheme == "https";

        nlohmann::json headers = {
            { "Authorization", "Bearer " + m_AuthToken },
        };

        auto res = https ? sslClient.Get(m_Prefix + endpoint, headers) : client.Get(m_Prefix + endpoint, headers);
        if (res)
        {
            if (res->status == httplib::OK_200)
                return nlohmann::json::parse(res->body);

            throw ChromaRequestException(res->body);
        }

        throw ChromaConnectionException(httplib::to_string(res.error()));
    }

    nlohmann::json ChromaApiClient::Post(const std::string& endpoint, const nlohmann::json& body)
    {
        httplib::Client client(m_BaseUrl);
        httplib::SSLClient sslClient(m_Host);

        bool https = m_Scheme == "https";

        nlohmann::json headers = {
            { "Content-Type", "application/json" },
            { "Authorization", "Bearer " + m_AuthToken },
        };

        auto res = https ? sslClient.Post(m_Prefix + endpoint, headers, body.dump(), "application/json") : client.Post(m_Prefix + endpoint, headers, body.dump(), "application/json");
        if (res)
        {
            if (res->status == httplib::OK_200 || res->status == httplib::Created_201)
                return nlohmann::json::parse(res->body);

            throw ChromaRequestException(res->body);
        }

        throw ChromaConnectionException(httplib::to_string(res.error()));
    }

    nlohmann::json ChromaApiClient::Put(const std::string& endpoint, const nlohmann::json& body)
    {
        httplib::Client client(m_BaseUrl);
        httplib::SSLClient sslClient(m_Host);

        bool https = m_Scheme == "https";

        nlohmann::json headers = {
            { "Content-Type", "application/json" },
            { "Authorization", "Bearer " + m_AuthToken },
        };

        auto res = https ? sslClient.Put(m_Prefix + endpoint, headers, body.dump(), "application/json") : client.Put(m_Prefix + endpoint, headers, body.dump(), "application/json");
        if (res)
        {
            if (res->status == httplib::OK_200)
                return nlohmann::json::parse(res->body);

            throw ChromaRequestException(res->body);
        }

        throw ChromaConnectionException(httplib::to_string(res.error()));
    }

    nlohmann::json ChromaApiClient::Delete(const std::string& endpoint)
    {
        httplib::Client client(m_BaseUrl);
        httplib::SSLClient sslClient(m_Host);

        bool https = m_Scheme == "https";

        nlohmann::json headers = {
            { "Authorization", "Bearer " + m_AuthToken },
        };

        auto res = https ? sslClient.Delete(m_Prefix + endpoint, headers) : client.Delete(m_Prefix + endpoint, headers);
        if (res)
        {
            if (res->status == httplib::OK_200)
                return nlohmann::json::parse(res->body);

            throw ChromaRequestException(res->body);
        }

        throw ChromaConnectionException(httplib::to_string(res.error()));
    }

} // namespace chromadb