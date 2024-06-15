#include "ChromaDB/Exceptions/Exception.h"

namespace chromadb {

    void ChromaException::throwSpecific(const std::string& message, const std::string& type)
    {
        if (type == "NotFoundError") throw ChromaNotFoundException(message);
        if (type == "AuthorizationError") throw ChromaAuthorizationException(message);
        if (type == "ValueError") throw ChromaValueException(message);
        if (type == "UniqueConstraintError") throw ChromaUniqueConstraintException(message);
        if (type == "DimensionalityError") throw ChromaDimensionalityException(message);
        if (type == "InvalidCollection") throw ChromaInvalidCollectionException(message);
        if (type == "TypeError") throw ChromaTypeException(message);

        throw ChromaException(message);
    }

    std::string ChromaException::inferTypeFromMessage(const std::string& message)
    {
        if (message.find("NotFoundError") != std::string::npos) return "NotFoundError";
        if (message.find("AuthorizationError") != std::string::npos || message.find("Forbidden") != std::string::npos) return "AuthorizationError";
        if (message.find("UniqueConstraintError") != std::string::npos) return "UniqueConstraintError";
        if (message.find("ValueError") != std::string::npos) return "ValueError";
        if (message.find("dimensionality") != std::string::npos) return "DimensionalityError";

        return "UnknownError";
    }

} // namespace chromadb