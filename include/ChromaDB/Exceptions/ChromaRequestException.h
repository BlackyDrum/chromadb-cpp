#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

    class ChromaRequestException : public ChromaException
    {
        using ChromaException::ChromaException;
    };

} // namespace chromadb