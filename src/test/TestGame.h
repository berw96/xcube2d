#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

class TestGame : public AbstractGame {
	private:
		PhysicsObject* PO1;
		PhysicsObject* PO2;
		PhysicsObject* PO3;

		bool UI_Toggled;

		void handleKeyEvents();
		void handleMechanics();
		void handleForces();
		void update();
		void renderGeometry();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif