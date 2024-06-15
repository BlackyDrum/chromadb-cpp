#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

	class ChromaInvalidArgumentException : public ChromaException
	{
		using ChromaException::ChromaException;
	};

} // namespace chromadb