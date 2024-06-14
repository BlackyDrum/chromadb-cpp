#include <gtest/gtest.h>
#include "ChromaDB/ChromaDB.h"

using namespace chromadb;

class ClientTest : public ::testing::Test {
protected:
    Client* client;

    void SetUp() override {
        client = new Client("http", "localhost", "8080", "test_database", "test_tenant", "authToken");
    }

    void TearDown() override {
        client->DeleteCollections();

        delete client;
    }
};

TEST_F(ClientTest, ConstructorInitializesCorrectly)
{
    EXPECT_EQ(client->GetDatabase(), "test_database");
    EXPECT_EQ(client->GetTenant(), "test_tenant");
}