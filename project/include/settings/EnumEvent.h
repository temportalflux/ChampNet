#ifndef ENUM_MESSAGES_H
#define ENUM_MESSAGES_H

namespace Modules {

	namespace Messages {

		enum EventType
		{
			NONE = -1,

			INPUT_KEY_PRESSED,
			INPUT_KEY_RELEASED,
			INPUT_MOUSE_PRESSED,
			INPUT_MOUSE_RELEASED,

			WINDOW_CLOSE

		};

	};

};

#endif