#include <iostream>

#include "ChromaDB/ChromaDB.h"

int main()
{
	try
	{
		chromadb::Client client("http", "localhost", "8080");

		std::cout << "Version: " << client.GetVersion() << std::endl;
		std::cout << "Database: " << client.GetDatabase() << std::endl;
		std::cout << "Tenant: " << client.GetTenant() << std::endl;
	}
	catch (const chromadb::ChromaException& e)
	{
		std::cerr << e.what() << std::endl;
	}
}