#pragma once

#include <string>

namespace chromadb {

    class Utils
    {
    public:
        template<typename T>
        static std::string Join(const T& container, const std::string& delimiter)
        {
            std::ostringstream oss;
            for (auto it = container.begin(); it != container.end(); ++it)
            {
                if (it != container.begin())
                    oss << delimiter;

                oss << *it;
            }

            return oss.str();
        }
    private:
            Utils() = default;
    };

} // namespace chromadb