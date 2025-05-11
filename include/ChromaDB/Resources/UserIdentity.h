#pragma once

#include <vector>
#include <string>

namespace chromadb {

    struct UserIdentity
    {
        const std::vector<std::string> databases;
        const std::string tenant;
        const std::string userId;

        UserIdentity(const std::vector<std::string>& databases, const std::string& tenant, const std::string& userId)
            : databases(databases), tenant(tenant), userId(userId)
        {
        }
    };

} // namespace chromadb
