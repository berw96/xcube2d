#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), UI_Toggled(true) {
#pragma region SETUP
	TTF_Font * font = ResourceManager::loadFont("res/fonts/game_over.ttf", 50);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	/*Creates PhysicsObjects with mass and init xy-transform*/
	PO1 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 100.f, Vector2f(800.f, 1000.f));
	PO2 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 200.f, Vector2f(800.f, 300.f), 10.f);
	PO3 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 1000.0f, Vector2f(1100.f, 500.f), 10.f);

	/*Takes the values used in the constructors and saves them for later use.*/
	setResetParameters();
#pragma endregion
}

TestGame::~TestGame() {
	/*Deletes raw pointers - memory management*/
	delete PO1;
	delete PO2;
	delete PO3;
}

void TestGame::setResetParameters() {
	PO1_initTransform	= PO1->getTransform();
	PO2_initTransform	= PO2->getTransform();
	PO3_initTransform	= PO3->getTransform();
	PO1_initSpeed			= PO1->getSpeed();
	PO2_initSpeed			= PO2->getSpeed();
	PO3_initSpeed			= PO3->getSpeed();
	PO1_initVelocity		= PO1->getVelocity();
	PO2_initVelocity		= PO2->getVelocity();
	PO3_initVelocity		= PO3->getVelocity();
	PO1_initForce			= PO1->getForce();
	PO2_initForce			= PO2->getForce();
	PO3_initForce			= PO3->getForce();
}

void TestGame::handleKeyEvents() {
#pragma region EVENTS
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

	/*Resets the session*/
	if (eventSystem->isPressed(Key::R)) {
		reset();
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

void TestGame::handleGravitation() {
#pragma region GRAVITATION
	/*Physics engine calculates the gravitational force that
	exists between the gravitating PhysicsObjects*/
	PO1->setForce(physics->calculateGravitationalForce(*PO3, *PO1));
	PO2->setForce(physics->calculateGravitationalForce(*PO3, *PO2));
	PO3->setForce(physics->calculateGravitationalForce(*PO2, *PO3));
#pragma endregion
}

void TestGame::reset() {
#pragma region RESET
	PO1->setTransform(PO1_initTransform);
	PO1->setSpeed(PO1_initSpeed);
	PO1->setVelocity(PO1_initVelocity);
	PO1->setForce(PO1_initForce);

	PO2->setTransform(PO2_initTransform);
	PO2->setSpeed(PO2_initSpeed);
	PO2->setVelocity(PO2_initVelocity);
	PO2->setForce(PO2_initForce);

	PO3->setTransform(PO3_initTransform);
	PO3->setSpeed(PO3_initSpeed);
	PO3->setVelocity(PO3_initVelocity);
	PO3->setForce(PO3_initForce);
#pragma endregion
}

/*
	Executes once every tick.
*/
void TestGame::update() {
	handleMechanics();
	handleGravitation();
}

/*
	Renders geometry.
*/
void TestGame::renderGeometry() {
#pragma region GEOMETRY
	gfx->setDrawColor(SDL_COLOR_AQUA);
	gfx->drawCircle(PO1->getCenter(), PO1->getRadius());

	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawCircle(PO2->getCenter(), PO2->getRadius());

	gfx->setDrawColor(SDL_COLOR_GREEN);
	gfx->drawCircle(PO3->getCenter(), PO3->getRadius());
#pragma endregion
}

/*
	Renders in-game text for the User Interface.
*/
void TestGame::renderUI() {
	std::string PO1_tag = "PO1";
	std::string PO2_tag = "PO2";
	std::string PO3_tag = "PO3";
	std::string title = "ci517 Game Engine Fundamentals, 19/20: Student ID-17802815";
	gfx->setDrawColor(SDL_COLOR_WHITE);
	gfx->drawText(title, 0.f, 0.f);

#pragma region USER_INTERFACE
	/*If the UI is toggled it will display the physical fields of the PhysicsObjects*/
	if (UI_Toggled == true) {

		/*AQUA TEXT*/
		gfx->setDrawColor(SDL_COLOR_AQUA);
		gfx->drawText(PO1_tag, PO1->getCenter().x, PO1->getCenter().y - 30.f);
		gfx->drawText("X:", PO1->getCenter().x + 50.f, PO1->getCenter().y - 30.f);
		gfx->drawText(std::to_string(PO1->getTransform().x), PO1->getCenter().x + 90.f, PO1->getCenter().y - 30.f);
		gfx->drawText("Y:", PO1->getCenter().x + 50.f, PO1->getCenter().y);
		gfx->drawText(std::to_string(PO1->getTransform().y), PO1->getCenter().x + 90.f, PO1->getCenter().y);
		gfx->drawText("F:", PO1->getCenter().x + 50.f, PO1->getCenter().y + 30.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO1->getForce())), PO1->getCenter().x + 90.f, PO1->getCenter().y + 30.f);
		gfx->drawText("M:", PO1->getCenter().x + 50.f, PO1->getCenter().y + 60.f);
		gfx->drawText(std::to_string(PO1->getMass()), PO1->getCenter().x + 90.f, PO1->getCenter().y + 60.f);
		gfx->drawText("A:", PO1->getCenter().x + 50.f, PO1->getCenter().y + 90.f);
		gfx->drawText(std::to_string(PO1->getAcceleration().x), PO1->getCenter().x + 90.f, PO1->getCenter().y + 90.f);
		gfx->drawText("V:", PO1->getCenter().x + 50.f, PO1->getCenter().y + 120.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO1->getSpeed())), PO1->getCenter().x + 90.f, PO1->getCenter().y + 120.f);
		gfx->drawText("MV:", PO1->getCenter().x + 50.f, PO1->getCenter().y  + 150.f);
		gfx->drawText(std::to_string(physics->calculateResultant(physics->calculateMomentum(*PO1))), PO1->getCenter().x + 90.f, PO1->getCenter().y + 150.f);

		/*RED TEXT*/
		gfx->setDrawColor(SDL_COLOR_RED);
		gfx->drawText(PO2_tag, PO2->getCenter().x, PO2->getCenter().y - 30.f);
		gfx->drawText("X:", PO2->getCenter().x + 50.f, PO2->getCenter().y - 30.f);
		gfx->drawText(std::to_string(PO2->getTransform().x), PO2->getCenter().x + 90.f, PO2->getCenter().y - 30.f);
		gfx->drawText("Y:", PO2->getCenter().x + 50.f, PO2->getCenter().y);
		gfx->drawText(std::to_string(PO2->getTransform().y), PO2->getCenter().x + 90.f, PO2->getCenter().y);
		gfx->drawText("F:", PO2->getCenter().x + 50.f, PO2->getCenter().y + 30.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO2->getForce())), PO2->getCenter().x + 90.f, PO2->getCenter().y + 30.f);
		gfx->drawText("M:", PO2->getCenter().x + 50.f, PO2->getCenter().y + 60.f);
		gfx->drawText(std::to_string(PO2->getMass()), PO2->getCenter().x + 90.f, PO2->getCenter().y + 60.f);
		gfx->drawText("A:", PO2->getCenter().x + 50.f, PO2->getCenter().y + 90.f);
		gfx->drawText(std::to_string(PO2->getAcceleration().x), PO2->getCenter().x + 90.f, PO2->getCenter().y + 90.f);
		gfx->drawText("V:", PO2->getCenter().x + 50.f, PO2->getCenter().y + 120.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO2->getSpeed())), PO2->getCenter().x + 90.f, PO2->getCenter().y + 120.f);
		gfx->drawText("MV:", PO2->getCenter().x + 50.f, PO2->getCenter().y + 150.f);
		gfx->drawText(std::to_string(physics->calculateResultant(physics->calculateMomentum(*PO2))), PO2->getCenter().x + 90.f, PO2->getCenter().y + 150.f);

		/*GREEN TEXT*/
		gfx->setDrawColor(SDL_COLOR_GREEN);
		gfx->drawText(PO3_tag, PO3->getCenter().x, PO3->getCenter().y - 30.f);
		gfx->drawText("X:", PO3->getCenter().x + 50.f, PO3->getCenter().y - 30.f);
		gfx->drawText(std::to_string(PO3->getTransform().x), PO3->getCenter().x + 90.f, PO3->getCenter().y - 30.f);
		gfx->drawText("Y:", PO3->getCenter().x + 50.f, PO3->getCenter().y);
		gfx->drawText(std::to_string(PO3->getTransform().y), PO3->getCenter().x + 90.f, PO3->getCenter().y);
		gfx->drawText("F:", PO3->getCenter().x + 50.f, PO3->getCenter().y + 30.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO3->getForce())), PO3->getCenter().x + 90.f, PO3->getCenter().y + 30.f);
		gfx->drawText("M:", PO3->getCenter().x + 50.f, PO3->getCenter().y + 60.f);
		gfx->drawText(std::to_string(PO3->getMass()), PO3->getCenter().x + 90.f, PO3->getCenter().y + 60.f);
		gfx->drawText("A:", PO3->getCenter().x + 50.f, PO3->getCenter().y + 90.f);
		gfx->drawText(std::to_string(PO3->getAcceleration().x), PO3->getCenter().x + 90.f, PO3->getCenter().y + 90.f);
		gfx->drawText("V:", PO3->getCenter().x + 50.f, PO3->getCenter().y + 120.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO3->getSpeed())), PO3->getCenter().x + 90.f, PO3->getCenter().y + 120.f);
		gfx->drawText("MV:", PO3->getCenter().x + 50.f, PO3->getCenter().y + 150.f);
		gfx->drawText(std::to_string(physics->calculateResultant(physics->calculateMomentum(*PO3))), PO3->getCenter().x + 90.f, PO3->getCenter().y + 150.f);
	}
#pragma endregion
}
