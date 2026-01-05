#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "ChromaDB/Exceptions/External/EmbeddingProviderConnectionException.h"
#include "ChromaDB/Exceptions/External/EmbeddingProviderRequestException.h"

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

        nlohmann::json GetRequestMetadata() const { return m_LastRequestAdditionalMetadata; }
    protected:
        std::string m_BaseUrl;
        std::string m_Path;
        std::string m_ApiKey;
        std::string m_Model;

        nlohmann::json m_LastRequestAdditionalMetadata;
    protected:
        nlohmann::json Request(const nlohmann::json& body, bool useSSL = true);
    };

} // namespace chromadb