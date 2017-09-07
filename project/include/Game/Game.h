#pragma once

class Framework;

namespace Game {

	class InputSystem;

	class Game {

	private:
		Framework* mpNetworkFramework;
		InputSystem* mpSystemInput;

	public:
		Game();
		~Game();

		void run();

	protected:

		void startup();
		void runLoop();
		void update(bool &shouldExit);

	};

};
