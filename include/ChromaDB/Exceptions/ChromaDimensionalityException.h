#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

	class ChromaDimensionalityException : public ChromaException
	{
		using ChromaException::ChromaException;
	};

} // namespace chromadb