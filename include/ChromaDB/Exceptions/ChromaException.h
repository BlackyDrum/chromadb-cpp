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

} // namespace chromadb