#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "ChromaDB/Exceptions/Exception.h"

#include "Http/httplib.h"
#include "Json/json.h"

#include <string>

namespace chromadb {

	class ChromaApiClient
	{
	public:
		/*
		* @brief Construct a new ChromaApiClient object
		* 
		* @param scheme The scheme of the server (http or https)
		* @param host The host of the server
		* @param port The port of the server
		* @param authToken The authorization token to use
		*/
		ChromaApiClient(const std::string& scheme, const std::string& host, const std::string& port, const std::string& authToken);

		/*
		* @brief Make a GET request to the server
		* 
		* @param endpoint The endpoint to make the request to
		* 
		* @return nlohmann::json The response from the server
		* 
		* @throw ChromaRequestException if the request fails
		* @throw ChromaConnectionException if the connection to the server fails
		*/
		nlohmann::json Get(const std::string& endpoint);

		/*
		* @brief Make a POST request to the server
		* 
		* @param endpoint The endpoint to make the request to
		* @param body The body of the request
		* 
		* @return nlohmann::json The response from the server
		* 
		* @throw ChromaRequestException if the request fails
		* @throw ChromaConnectionException if the connection to the server fails
		*/
		nlohmann::json Post(const std::string& endpoint, const nlohmann::json& body);

		/*
		* @brief Make a PUT request to the server
		* 
		* @param endpoint The endpoint to make the request to
		* @param body The body of the request
		* 
		* @return nlohmann::json The response from the server
		* 
		* @throw ChromaRequestException if the request fails
		* @throw ChromaConnectionException if the connection to the server fails
		*/
		nlohmann::json Put(const std::string& endpoint, const nlohmann::json& body);

		/*
		* @brief Make a DELETE request to the server
		* 
		* @param endpoint The endpoint to make the request to
		* 
		* @return nlohmann::json The response from the server
		* 
		* @throw ChromaRequestException if the request fails
		* @throw ChromaConnectionException if the connection to the server fails
		*/
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