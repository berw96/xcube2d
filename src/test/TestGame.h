#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

#include "TestMazeGenerator.h"

class TestGame : public AbstractGame {
	private:
		PhysicsObject* PO1;
		PhysicsObject* PO2;
		PhysicsObject* PO3;

		bool UI_Toggled;

		void handleKeyEvents();
		void handleMechanics();
		void handleCollisions();
		void handleGravitation();
		void reset();
		void update();
		void render();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif