#pragma once

namespace Game {

	class InputSystem {

	private:

		void *mpConsoleWindow;

	public:
		InputSystem();
		~InputSystem() {};

		void update(bool &shouldExit);

	};

};
