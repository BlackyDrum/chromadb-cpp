#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

    class EmbeddingProviderConnectionException : public ChromaException
    {
        using ChromaException::ChromaException;
    };

} // namespace chromadb