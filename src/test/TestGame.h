#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

#include "TestMazeGenerator.h"

class TestGame : public AbstractGame {
	private:
		Rect box;

		bool gravityToggled;
		float speed_x;
		float speed_y;
		Vector2i velocity;

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