#include "ChampNetPlugin.h"
#include "ChampNet.h"

namespace ChampNetPlugin {

	CHAMPNET_PLUGIN_SYMTAG ChampNet::Network *INSTANCE = 0;

	int Create() {
		if (INSTANCE == 0) {
			INSTANCE = new ChampNet::Network();
			return INSTANCE->init();
		}
		return 0;
	}

	int Destroy() {
		if (INSTANCE != 0) {
			delete INSTANCE;
			INSTANCE = 0;
			return 0;
		}
		return 1;
	}

	int GetRandom() {
		return INSTANCE->getRandom();
	}

}
