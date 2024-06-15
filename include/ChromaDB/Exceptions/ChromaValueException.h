#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

	class ChromaValueException : public ChromaException
	{
		using ChromaException::ChromaException;
	};

} // namespace chromadb