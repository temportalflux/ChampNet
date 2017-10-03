
#include <iostream>
#include "ChampNetPlugin.h"

using namespace std;

int main() {
	cout << "Starting ChampNet plugin test environment...\n";

	ChampNetPlugin::Create();

	for (int i = 0; i < 10; i++)
		cout << ChampNetPlugin::GetRandom() % 1000 << '\n';

	ChampNetPlugin::Destroy();

	system("pause");
	return 0;
}
