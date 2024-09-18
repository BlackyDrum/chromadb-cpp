#include "ChromaDB/Exceptions/ChromaException.h"
#include "ChromaDB/Exceptions/ChromaAuthorizationException.h"
#include "ChromaDB/Exceptions/ChromaConnectionException.h"
#include "ChromaDB/Exceptions/ChromaDimensionalityException.h"
#include "ChromaDB/Exceptions/ChromaInvalidArgumentException.h"
#include "ChromaDB/Exceptions/ChromaInvalidCollectionException.h"
#include "ChromaDB/Exceptions/ChromaNotFoundException.h"
#include "ChromaDB/Exceptions/ChromaRequestException.h"
#include "ChromaDB/Exceptions/ChromaTypeException.h"
#include "ChromaDB/Exceptions/ChromaUniqueConstraintException.h"
#include "ChromaDB/Exceptions/ChromaValueException.h"

namespace chromadb {

    void ChromaException::throwSpecific(const std::string& message, const std::string& type)
    {
        if (type == "NotFoundError") throw ChromaNotFoundException(message);
        if (type == "AuthError") throw ChromaAuthorizationException(message);
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
        if (message.find("AuthError") != std::string::npos || message.find("Forbidden") != std::string::npos) return "AuthError";
        if (message.find("UniqueConstraintError") != std::string::npos) return "UniqueConstraintError";
        if (message.find("ValueError") != std::string::npos) return "ValueError";
        if (message.find("dimensionality") != std::string::npos) return "DimensionalityError";

        return "UnknownError";
    }

} // namespace chromadb