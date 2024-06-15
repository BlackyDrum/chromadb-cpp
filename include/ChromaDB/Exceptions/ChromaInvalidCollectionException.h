#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

	class ChromaInvalidCollectionException : public ChromaException
	{
		using ChromaException::ChromaException;
	};

} // namespace chromadb