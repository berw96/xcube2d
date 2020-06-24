#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

#include "TestMazeGenerator.h"

class TestGame : public AbstractGame {
	private:
		Rectf box;

		bool gravityToggled;
		float speed_res;
		Vector2f force;
		Vector2f acceleration;
		Vector2f velocity;
		Vector2f speed;
		float mass;
		

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