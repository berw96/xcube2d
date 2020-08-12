#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), UI_Toggled(true) {
#pragma region SETUP
	TTF_Font * font = ResourceManager::loadFont("res/fonts/game_over.ttf", 50);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	/*Creates PhysicsObjects with mass and init xy-transform*/
	PO1 = new PhysicsObject(Point2(0.f, 0.f), 10.f, Vector2f(800.f, 1000.f));
	PO2 = new PhysicsObject(Point2(0.f, 0.f), 20.f, Vector2f(800.f, 300.f), 10.f);
	PO3 = new PhysicsObject(Point2(0.f, 0.f), 50.0f, Vector2f(1100.f, 500.f), 60.f, "Sun");

	PO1->setTag("Earth");
	PO2->setTag("Moon");
#pragma endregion
}

TestGame::~TestGame() {
	/*Deletes raw pointers - memory management*/
	delete PO1;
	delete PO2;
	delete PO3;
}

void TestGame::handleKeyEvents() {
#pragma region EVENTS
	/*User can add a boost to a selected PhysicsObject*/
	float boost = 0.005f;
	if (eventSystem->isPressed(Key::UP)) {
		PO1->setBoostForce_Y(-boost);
	}
	if (eventSystem->isPressed(Key::DOWN)) {
		PO1->setBoostForce_Y(boost);
	}
	if (eventSystem->isPressed(Key::LEFT)) {
		PO1->setBoostForce_X(-boost);
	}
	if (eventSystem->isPressed(Key::RIGHT)) {
		PO1->setBoostForce_X(boost);
	}

	/*Toggles UI (ON)*/
	if (eventSystem->isPressed(Key::U)) {
		if (UI_Toggled != true) {
			UI_Toggled = true;
		}
	}
	
	/*Toggles UI (OFF)*/
	if (eventSystem->isPressed(Key::I)) {
		if (UI_Toggled != false) {
			UI_Toggled = false;
		}
	}
#pragma endregion
}

void TestGame::handleMechanics() {
#pragma region MECHANICS
	physics->mechanics(*PO1);
	physics->mechanics(*PO2);
	physics->mechanics(*PO3);
#pragma endregion
}

void TestGame::handleForces() {
#pragma region GRAVITATION
	/*Physics engine calculates the gravitational force that
	exists between the gravitating PhysicsObjects*/

	/*BUGGED - FLINGS GAME OBJECTS AWAY WHEN THEY'RE CLOSE ENOUGH*/
	/*physics->calculateGravitationalForce(*PO3, *PO1);
	physics->calculateGravitationalForce(*PO3, *PO2);
	physics->calculateGravitationalForce(*PO2, *PO3);*/

	/*BUGGED - DOESN'T WORK DUE TO FAULTY PO VECTOR REGISTRATION*/
	physics->calculateNetForce(*PO1, *PO2, *PO3);
	physics->calculateNetForce(*PO2, *PO1, *PO3);
	physics->calculateNetForce(*PO3, *PO2, *PO1);
#pragma endregion
}

/*
	Executes once every tick.
*/
void TestGame::update() {
	handleMechanics();
	handleForces();
}

/*
	Renders geometry.
*/
void TestGame::renderGeometry() {
#pragma region GEOMETRY
	gfx->setDrawColor(SDL_COLOR_WHITE);
	gfx->drawCircle(PO1->getCenter(), PO1->getRadius());
	gfx->drawCircle(PO2->getCenter(), PO2->getRadius());
	gfx->drawCircle(PO3->getCenter(), PO3->getRadius());
#pragma endregion
}

/*
	Renders in-game text for the User Interface.
*/
void TestGame::renderUI() {
	std::string title = "ci517 Game Engine Fundamentals, 19/20: Student ID-17802815";
	gfx->drawText(title, 0.f, 0.f);

#pragma region USER_INTERFACE
	/*If the UI is toggled it will display the physical fields of the PhysicsObjects*/
	if (UI_Toggled == true) {
		gfx->drawPhysicsUI(*PO1);
		gfx->drawPhysicsUI(*PO2);
		gfx->drawPhysicsUI(*PO3);
	}
#pragma endregion
}
