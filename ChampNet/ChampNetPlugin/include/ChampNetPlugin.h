#ifndef _CHAMPNET_PLUGIN_PLUGIN_H
#define _CHAMPNET_PLUGIN_PLUGIN_H

// defines CHAMPNET_PLUGIN_SYMTAG
#include "lib.h"

// tell compiler to link as if all function are C not C++
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	namespace ChampNetPlugin {
		
		// Create the framework
		CHAMPNET_PLUGIN_SYMTAG int Create();
		// Clean the framework
		CHAMPNET_PLUGIN_SYMTAG int Destroy();

		// TEMPORARY
		CHAMPNET_PLUGIN_SYMTAG int GetRandom();

	};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _CHAMPNET_PLUGIN_PLUGIN_H