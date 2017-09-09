/*
	Dustin Yost 0984932
	EGP-405-02
	Project 1
	09/21/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#pragma once

class Framework;

// Author: Dustin Yost
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
