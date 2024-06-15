#pragma once

#include <exception>
#include <string>

namespace chromadb {

    class ChromaException : public std::exception
    {
    public:
        explicit ChromaException(const std::string& message)
            : m_Message(message)
        {
        }

        const char* what() const noexcept override { return m_Message.c_str(); }

        static void throwSpecific(const std::string& message, const std::string& type);

        static std::string inferTypeFromMessage(const std::string& message);

    private:
        std::string m_Message;
    };

    class ChromaNotFoundException : public ChromaException {
    public:
        using ChromaException::ChromaException;
    };

    class ChromaConnectionException : public ChromaException {
    public:
        using ChromaException::ChromaException;
    };

    class ChromaRequestException : public ChromaException {
    public:
		using ChromaException::ChromaException;
	};

    class ChromaAuthorizationException : public ChromaException {
    public:
        using ChromaException::ChromaException;
    };

    class ChromaValueException : public ChromaException {
    public:
        using ChromaException::ChromaException;
    };

    class ChromaUniqueConstraintException : public ChromaException {
    public:
        using ChromaException::ChromaException;
    };

    class ChromaDimensionalityException : public ChromaException {
    public:
        using ChromaException::ChromaException;
    };

    class ChromaInvalidCollectionException : public ChromaException {
    public:
        using ChromaException::ChromaException;
    };

    class ChromaTypeException : public ChromaException {
    public:
        using ChromaException::ChromaException;
    };

    class ChromaInvalidArgumentException : public ChromaException {
    public:
		using ChromaException::ChromaException;
	};

} // namespace chromadb