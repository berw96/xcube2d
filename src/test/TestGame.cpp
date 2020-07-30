#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), UI_Toggled(true) {
#pragma region SETUP
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 12);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	/*Creates PhysicsObjects with mass and init xy-transform*/
	PO1 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 0.1f, Vector2f(235.f, 235.f));
	PO2 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 0.0001f, Vector2f(435.f, 135.f), Rectf(135.f, 135.f, 30.f, 30.f));
	PO3 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 1000.0f, Vector2f(335.f, 335.f), Rectf(335.f, 335.f, 30.f, 30.f));

	/*Takes the values used in the constructors and saves them for later use.*/
	setResetParameters();

	/*Registers PO2 as a child of PO1*/
	PO1->registerChild(*PO2);

	/*Sets all PhysicsObjects as moveable*/
	physics->setMovable(*PO1, true);
	physics->setMovable(*PO2, true);
#pragma endregion
}

TestGame::~TestGame() {
	/*Deletes raw pointers - memory management*/
	delete PO1;
	delete PO2;
	delete PO3;
}

void TestGame::setResetParameters() {
	PO1_initRootTransform = PO1->getRootTransform();
	PO2_initRootTransform = PO2->getRootTransform();
	PO3_initRootTransform = PO3->getRootTransform();
	PO1_initSpeed = PO1->getSpeed();
	PO2_initSpeed = PO2->getSpeed();
	PO3_initSpeed = PO3->getSpeed();
	PO1_initVelocity = PO1->getVelocity();
	PO2_initVelocity = PO2->getVelocity();
	PO3_initVelocity = PO3->getVelocity();
}

void TestGame::handleKeyEvents() {
#pragma region EVENTS
	/*Moves the PO1 PhysicsObject via keypresses.*/
	//acceleraton sets the speed
	if (eventSystem->isPressed(Key::UP)) {
		PO1->setSpeed_Y(PO1->getSpeed().y - PO1->getAcceleration().y);
	}

	if (eventSystem->isPressed(Key::DOWN)) {
		PO1->setSpeed_Y(PO1->getSpeed().y + PO1->getAcceleration().y);
	}

	if (eventSystem->isPressed(Key::LEFT)) {
		PO1->setSpeed_X(PO1->getSpeed().x - PO1->getAcceleration().x);
	}

	if (eventSystem->isPressed(Key::RIGHT)) {
		PO1->setSpeed_X(PO1->getSpeed().x + PO1->getAcceleration().x);
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

void TestGame::handleCollisions() {
#pragma region COLLISION
	//PhysicsObject / PhysicsObject collisions
	physics->collision(*PO1, *PO2);
	physics->collision(*PO1, *PO3);
	physics->collision(*PO2, *PO3);
#pragma endregion
}

void TestGame::handleGravitation() {
#pragma region GRAVITATION
	/*Physics engine calculates the gravitational force that
	exists between the gravitating PhysicsObjects*/
	PO2->setForce(physics->calculuateGravitationalForce(*PO2, *PO3));

	/*PhysicsObject moves towards the other using
	its own acceleration.*/
	PO2->moveTo(PO3->getRootTransform());

#pragma endregion
}

void TestGame::reset() {
#pragma region RESET
	PO1->setRootTransform(Vector2f(235.f, 235.f));
	PO1->setSpeed(Vector2f(0.f, 0.f));
	PO1->setVelocity(Vector2f(0.f, 0.f));

	PO2->setRootTransform(Vector2f(135.f, 135.f));
	PO2->setSpeed(Vector2f(0.f, 0.f));
	PO2->setVelocity(Vector2f(0.f, 0.f));

	PO3->setRootTransform(Vector2f(335.f, 335.f));
	PO3->setSpeed(Vector2f(0.f, 0.f));
	PO3->setVelocity(Vector2f(0.f, 0.f));
#pragma endregion
}

/*
	Executes once every tick.
*/
void TestGame::update() {
	handleMechanics();
	handleCollisions();
	handleGravitation();
}

/*
	Renders geometry.
*/
void TestGame::renderGeometry() {
#pragma region GEOMETRY
	gfx->setDrawColor(SDL_COLOR_AQUA);
	gfx->drawCircle(Point2(PO1->getRootTransform().x, PO1->getRootTransform().y), 5.0f);
	gfx->drawRect(PO1->getCollider());

	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawCircle(Point2(PO2->getRootTransform().x, PO2->getRootTransform().y), 5.0f);
	gfx->drawRect(PO2->getCollider());

	gfx->setDrawColor(SDL_COLOR_GREEN);
	gfx->drawCircle(Point2(PO3->getRootTransform().x, PO3->getRootTransform().y), 5.0f);
	gfx->drawRect(PO3->getCollider());
#pragma endregion
}

/*
	Renders in-game text for the User Interface.
*/
void TestGame::renderUI() {
	std::string PO1_tag = "PO1";
	std::string PO2_tag = "PO2";
	std::string PO3_tag = "PO3";

#pragma region USER_INTERFACE
	/*If the UI is toggled it will display the physical fields of the PhysicsObjects*/
	if (UI_Toggled == true) {

		/*AQUA TEXT*/
		gfx->setDrawColor(SDL_COLOR_AQUA);
		gfx->drawText(PO1_tag, PO1->getCollider().x, PO1->getCollider().y - 20.f);
		gfx->drawText("X:", PO1->getCollider().x + 50.f, PO1->getCollider().y - 15.f);
		gfx->drawText(std::to_string(PO1->getCollider().x), PO1->getCollider().x + 70.f, PO1->getCollider().y - 15.f);
		gfx->drawText("Y:", PO1->getCollider().x + 50.f, PO1->getCollider().y);
		gfx->drawText(std::to_string(PO1->getCollider().y), PO1->getCollider().x + 70.f, PO1->getCollider().y);
		gfx->drawText("F:", PO1->getCollider().x + 50.f, PO1->getCollider().y + 15.f);
		gfx->drawText(std::to_string(PO1->getForce().x), PO1->getCollider().x + 70.f, PO1->getCollider().y + 15.f);
		gfx->drawText("M:", PO1->getCollider().x + 50.f, PO1->getCollider().y + 30.f);
		gfx->drawText(std::to_string(PO1->getMass()), PO1->getCollider().x + 70.f, PO1->getCollider().y + 30.f);
		gfx->drawText("A:", PO1->getCollider().x + 50.f, PO1->getCollider().y + 45.f);
		gfx->drawText(std::to_string(PO1->getAcceleration().x), PO1->getCollider().x + 70.f, PO1->getCollider().y + 45.f);
		gfx->drawText("V:", PO1->getCollider().x + 50.f, PO1->getCollider().y + 60.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO1->getSpeed())), PO1->getCollider().x + 70.f, PO1->getCollider().y + 60.f);

		/*RED TEXT*/
		gfx->setDrawColor(SDL_COLOR_RED);
		gfx->drawText(PO2_tag, PO2->getCollider().x, PO2->getCollider().y - 20.f);
		gfx->drawText("X:", PO2->getCollider().x + 50.f, PO2->getCollider().y - 15.f);
		gfx->drawText(std::to_string(PO2->getCollider().x), PO2->getCollider().x + 70.f, PO2->getCollider().y - 15.f);
		gfx->drawText("Y:", PO2->getCollider().x + 50.f, PO2->getCollider().y);
		gfx->drawText(std::to_string(PO2->getCollider().y), PO2->getCollider().x + 70.f, PO2->getCollider().y);
		gfx->drawText("F:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 15.f);
		gfx->drawText(std::to_string(PO2->getForce().x), PO2->getCollider().x + 70.f, PO2->getCollider().y + 15.f);
		gfx->drawText("M:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 30.f);
		gfx->drawText(std::to_string(PO2->getMass()), PO2->getCollider().x + 70.f, PO2->getCollider().y + 30.f);
		gfx->drawText("A:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 45.f);
		gfx->drawText(std::to_string(PO2->getAcceleration().x), PO2->getCollider().x + 70.f, PO2->getCollider().y + 45.f);
		gfx->drawText("V:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 60.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO2->getSpeed())), PO2->getCollider().x + 70.f, PO2->getCollider().y + 60.f);

		/*GREEN TEXT*/
		gfx->setDrawColor(SDL_COLOR_GREEN);
		gfx->drawText(PO3_tag, PO3->getCollider().x, PO3->getCollider().y - 20.f);
		gfx->drawText("X:", PO3->getCollider().x + 50.f, PO3->getCollider().y - 15.f);
		gfx->drawText(std::to_string(PO3->getCollider().x), PO3->getCollider().x + 70.f, PO3->getCollider().y - 15.f);
		gfx->drawText("Y:", PO3->getCollider().x + 50.f, PO3->getCollider().y);
		gfx->drawText(std::to_string(PO3->getCollider().y), PO3->getCollider().x + 70.f, PO3->getCollider().y);
		gfx->drawText("F:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 15.f);
		gfx->drawText(std::to_string(PO3->getForce().x), PO3->getCollider().x + 70.f, PO3->getCollider().y + 15.f);
		gfx->drawText("M:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 30.f);
		gfx->drawText(std::to_string(PO3->getMass()), PO3->getCollider().x + 70.f, PO3->getCollider().y + 30.f);
		gfx->drawText("A:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 45.f);
		gfx->drawText(std::to_string(PO3->getAcceleration().x), PO3->getCollider().x + 70.f, PO3->getCollider().y + 45.f);
		gfx->drawText("V:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 60.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO3->getSpeed())), PO3->getCollider().x + 70.f, PO3->getCollider().y + 60.f);
	}
#pragma endregion
}
