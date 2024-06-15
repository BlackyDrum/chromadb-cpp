#pragma once

#include "ChromaDB/Exceptions/ChromaException.h"

namespace chromadb {

	class ChromaAuthorizationException : public ChromaException
	{
		using ChromaException::ChromaException;
	};

} // namespace chromadb