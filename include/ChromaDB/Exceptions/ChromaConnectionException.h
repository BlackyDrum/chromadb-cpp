#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

    class ChromaConnectionException : public ChromaException
    {
        using ChromaException::ChromaException;
    };

} // namespace chromadb