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

        static void ThrowSpecific(const std::string& message, const std::string& type);

    private:
        std::string m_Message;
    };

} // namespace chromadb