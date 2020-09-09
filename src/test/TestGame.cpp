#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), UI_Toggled(true) {
#pragma region SETUP
	physics->registerObject(PO1);
	physics->registerObject(PO2);
	physics->registerObject(PO3);
#pragma endregion
}

TestGame::~TestGame() {
}

void TestGame::handleKeyEvents() {
#pragma region EVENTS
	float boost = 0.005f;
	if (eventSystem->isPressed(Key::UP))
		PO1->setBoostForce_Y(-boost);
	if (eventSystem->isPressed(Key::DOWN))
		PO1->setBoostForce_Y(boost);
	if (eventSystem->isPressed(Key::LEFT))
		PO1->setBoostForce_X(-boost);
	if (eventSystem->isPressed(Key::RIGHT))
		PO1->setBoostForce_X(boost);

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
	physics->mechanics();
}

/*
	Executes once every tick.
*/
void TestGame::update() {
	handleMechanics();
	handleKeyEvents();
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
