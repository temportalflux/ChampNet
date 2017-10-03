#include "ChampNet.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

namespace ChampNet {

	Network::Network() {

	}

	int Network::init() {
		srand((unsigned int)time(NULL));
		return 0;
	}

	int Network::getRandom() {
		return rand();
	}

}
