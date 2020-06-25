#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

#include "TestMazeGenerator.h"

class TestGame : public AbstractGame {
	private:

		bool gravityToggled;
		float speed_res;

		MazeGenerator * gen;
		PhysicsObject* player;

		std::vector<std::shared_ptr<Line2i>> lines;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif