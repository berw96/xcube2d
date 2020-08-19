#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), UI_Toggled(true) {
#pragma region SETUP
	TTF_Font * font = ResourceManager::loadFont("res/fonts/game_over.ttf", 50);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	physics->registerObject(PO1);
	physics->registerObject(PO2);
	physics->registerObject(PO3);

#pragma endregion
}

TestGame::~TestGame() {
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
	// CI517 physics subsystem implementation
	physics->mechanics();
#pragma endregion
}

/*
	Executes once every tick.
*/
void TestGame::update() {
	handleMechanics();
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
