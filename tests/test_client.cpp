#include <gtest/gtest.h>
#include "ChromaDB/ChromaDB.h"

TEST(ClientTest, Initialization) {
    chromadb::Client client("http", "localhost", "8080");
    EXPECT_EQ(client.GetTenant(), "default_tenant");
    EXPECT_EQ(client.GetDatabase(), "default_database");
}
