#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

class TestGame : public AbstractGame {
	private:
		//Make Physics Objects
		std::shared_ptr<PhysicsObject> PO1{ new PhysicsObject(Point2(0.f, 0.f), 3.f, Vector2f(1800.f, 1000.f), 5.f, "Almas CI-517") };
		std::shared_ptr<PhysicsObject> PO2{ new PhysicsObject(Point2(0.f, 0.f), 20.f, Vector2f(800.f, 300.f), 2.f, "Baimag CI-601") };
		std::shared_ptr<PhysicsObject> PO3{ new PhysicsObject(Point2(0.f, 0.f), 200.0f, Vector2f(1100.f, 300.f), 10.f, "Ambetov CI-007") };
		
		bool UI_Toggled;

		void handleKeyEvents();
		void handleMechanics();
		void update();
		void renderGeometry();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif