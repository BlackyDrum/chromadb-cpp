#include "ChromaDB/Embeddings/LocalOllamaEmbeddingFunction.h"

namespace chromadb {
LocalOllamaEmbeddingFunction::LocalOllamaEmbeddingFunction(
    const std::string& address, const std::string& model)
    : EmbeddingFunction("", model, address, "/api/embed") {}

std::vector<std::vector<double>> LocalOllamaEmbeddingFunction::Generate(
    const std::vector<std::string>& documents) {
  nlohmann::json body = {{"input", documents}, {"model", m_Model}};

  nlohmann::json response = this->Request(body);

  std::vector<std::vector<double>> embeddings;
  for (const auto& obj : response["embeddings"]) {
    embeddings.emplace_back(obj.get<std::vector<double>>());
  }

  nlohmann::json additionalData;
  additionalData["model"] = response["model"];
  m_LastRequestAdditionalMetadata = additionalData;

  return embeddings;
}

nlohmann::json LocalOllamaEmbeddingFunction::Request(
    const nlohmann::json& body) {
  // Don't use SSL
  httplib::Client client(m_BaseUrl);

  httplib::Headers headers = {
      {"Content-Type", "application/json;charset=utf-8"}};

  httplib::Result res =
      client.Post(m_Path, headers, body.dump(), "application/json");
  if (res) {
    if (res->status == httplib::OK_200) return nlohmann::json::parse(res->body);

    throw EmbeddingProviderRequestException(res->body);
  }

  throw EmbeddingProviderConnectionException(httplib::to_string(res.error()));
}

}  // namespace chromadb
