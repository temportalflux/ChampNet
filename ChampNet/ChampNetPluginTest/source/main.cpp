
#include <iostream>

#include "Server.h"

using namespace std;

int main() {
	cout << "Starting ChampNet server...\n";

	Server *pServer = new Server();

	while (pServer->isRunning())
	{
		pServer->update();
	}

	delete pServer;
	pServer = NULL;

	system("pause");
	return 0;
}
