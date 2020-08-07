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

		void setResetParameters();
		void handleKeyEvents();
		void handleMechanics();
		void handleGravitation();
		void reset();
		void update();
		void renderGeometry();
		void renderUI();
	public:
		TestGame();
		~TestGame();

		//PhysicsObject initial values
		Vector2f PO1_initTransform;
		Vector2f PO2_initTransform;
		Vector2f PO3_initTransform;
		Vector2f PO1_initSpeed;
		Vector2f PO2_initSpeed;
		Vector2f PO3_initSpeed;
		Vector2f PO1_initVelocity;
		Vector2f PO2_initVelocity;
		Vector2f PO3_initVelocity;
		Vector2f PO1_initForce;
		Vector2f PO2_initForce;
		Vector2f PO3_initForce;
};

#endif