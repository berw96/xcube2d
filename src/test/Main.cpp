#include "TestGame.h"

int main(int argc, char * args[]) {

	try {
		TestGame game;							//creates a TestGame instance.
		game.runMainLoop();						//invokes the main game loop on it.
	} catch (EngineException & e) {				//catches an exception 'e' using its reference in memory.
		std::cout << e.what() << std::endl;		//prints the exception to the console.
		getchar();
	}

	return 0;
}