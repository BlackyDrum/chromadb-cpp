#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

    class EmbeddingProviderRequestException : public ChromaException
    {
        using ChromaException::ChromaException;
    };

} // namespace chromadb