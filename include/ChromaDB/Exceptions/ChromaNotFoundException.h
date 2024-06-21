#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

    class ChromaNotFoundException : public ChromaException
    {
        using ChromaException::ChromaException;
    };

} // namespace chromadb