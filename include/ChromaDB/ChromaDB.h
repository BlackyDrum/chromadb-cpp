/***********************************************************************************
* 
*   ChromaDB C++ - A C++ client for the ChromaDB API
* 
*     _____ _                               _____  ____         _____
*    / ____| |                             |  __ \|  _ \       / ____|_     _
*   | |    | |__  _ __ ___  _ __ ___   __ _| |  | | |_) |     | |   _| |_ _| |_
*   | |    | '_ \| '__/ _ \| '_ ` _ \ / _` | |  | |  _ <      | |  |_   _|_   _|
*   | |____| | | | | | (_) | | | | | | (_| | |__| | |_) |     | |____|_|   |_|
*    \_____|_| |_|_|  \___/|_| |_| |_|\__,_|_____/|____/       \_____|
*
*   MIT License
*
*   Copyright (c) BlackyDrum
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
* 
***********************************************************************************/

#pragma once

#include "ChromaDB/Client/Client.h"
#include "ChromaDB/Resources/Collection.h"

#include "ChromaDB/Resources/QueryResponseResource.h"
#include "ChromaDB/Resources/EmbeddingResource.h"

#include "ChromaDB/Resources/UserIdentity.h"

#include "ChromaDB/Exceptions/ChromaException.h"
#include "ChromaDB/Exceptions/ChromaAuthorizationException.h"
#include "ChromaDB/Exceptions/ChromaConnectionException.h"
#include "ChromaDB/Exceptions/ChromaDimensionalityException.h"
#include "ChromaDB/Exceptions/ChromaInvalidArgumentException.h"
#include "ChromaDB/Exceptions/ChromaInvalidCollectionException.h"
#include "ChromaDB/Exceptions/ChromaNotFoundException.h"
#include "ChromaDB/Exceptions/ChromaRequestException.h"
#include "ChromaDB/Exceptions/ChromaTypeException.h"
#include "ChromaDB/Exceptions/ChromaUniqueConstraintException.h"
#include "ChromaDB/Exceptions/ChromaValueException.h"
#include "ChromaDB/Exceptions/External/EmbeddingProviderConnectionException.h"
#include "ChromaDB/Exceptions/External/EmbeddingProviderRequestException.h"

#include "ChromaDB/Embeddings/JinaEmbeddingFunction.h"
#include "ChromaDB/Embeddings/OpenAIEmbeddingFunction.h"
#include "ChromaDB/Embeddings/CohereEmbeddingFunction.h"
#include "ChromaDB/Embeddings/VoyageAIEmbeddingFunction.h"
#include "ChromaDB/Embeddings/TogetherAIEmbeddingFunction.h"
#include "ChromaDB/Embeddings/LocalOllamaEmbeddingFunction.h"
