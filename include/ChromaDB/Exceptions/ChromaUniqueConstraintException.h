#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

    class ChromaUniqueConstraintException : public ChromaException
    {
        using ChromaException::ChromaException;
    };

} // namespace chromadb