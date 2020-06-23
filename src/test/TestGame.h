#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

#include "TestMazeGenerator.h"

class TestGame : public AbstractGame {
	private:
		Rectf box;

		bool gravityToggled;
		float speed_x;
		float speed_y;
		float speed_res;
		Vector2f velocity;

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