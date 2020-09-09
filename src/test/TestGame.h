#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

class TestGame : public AbstractGame {
	private:
		std::shared_ptr<PhysicsObject> PO1{ new PhysicsObject(Point2(0.f, 0.f), 10.f, Vector2f(800.f, 1000.f), 20.f, "Earth") };
		std::shared_ptr<PhysicsObject> PO2{ new PhysicsObject(Point2(0.f, 0.f), 20.f, Vector2f(800.f, 300.f), 10.f, "Moon") };
		std::shared_ptr<PhysicsObject> PO3{ new PhysicsObject(Point2(0.f, 0.f), 50.0f, Vector2f(1100.f, 500.f), 60.f, "Sun") };
		
		bool UI_Toggled;

		void handleKeyEvents();
		void handleMechanics();
		void update();
		void renderGeometry();
	public:
		TestGame();
		~TestGame();
};

#endif