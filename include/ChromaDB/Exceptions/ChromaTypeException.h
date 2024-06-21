#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

    class ChromaTypeException : public ChromaException
    {
        using ChromaException::ChromaException;
    };

} // namespace chromadb