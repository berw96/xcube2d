#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), UI_Toggled(true) {
#pragma region SETUP
	TTF_Font * font = ResourceManager::loadFont("res/fonts/game_over.ttf", 50);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	/*Creates PhysicsObjects with mass and init xy-transform*/
	PO1 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 0.1f, Vector2f(235.f, 235.f));
	PO2 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 0.2f, Vector2f(320.f, 300.f), Rectf(135.f, 135.f, 30.f, 30.f));
	PO3 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 1000.0f, Vector2f(1100.f, 500.f), Rectf(335.f, 335.f, 30.f, 30.f));
	floor = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 1000.0f, Vector2f(960.f, 1000.f), Rectf(335.f, 335.f, 1000.f, 30.f));

	/*Sets PhysicsObjects as moveable*/
	physics->setMovable(*PO1, true);
	physics->setMovable(*PO2, true);
	//physics->setMovable(*PO3, true);

	/*Takes the values used in the constructors and saves them for later use.*/
	setResetParameters();
#pragma endregion
}

TestGame::~TestGame() {
	/*Deletes raw pointers - memory management*/
	delete PO1;
	delete PO2;
	delete PO3;
	delete floor;
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
	PO1_initForce = PO1->getForce();
	PO2_initForce = PO2->getForce();
	PO3_initForce = PO3->getForce();
}

void TestGame::handleKeyEvents() {
#pragma region EVENTS
	/*Moves the PO1 PhysicsObject via keypresses. It is in
	a state of de facto movement at all times, but will only
	move when a force is applied to it*/
	//acceleraton sets the speed
	/*PO1 controls*/
	if (eventSystem->isPressed(Key::UP)) {
		PO1->incrementForce_Y(-0.0001f);
	}
	if (eventSystem->isPressed(Key::DOWN)) {
		PO1->incrementForce_Y(0.0001f);
	}
	if (eventSystem->isPressed(Key::LEFT)) {
		PO1->incrementForce_X(-0.0001f);
	}
	if (eventSystem->isPressed(Key::RIGHT)) {
		PO1->incrementForce_X(0.0001f);
	}

	/*PO2 controls*/
	if (eventSystem->isPressed(Key::W)) {
		PO2->incrementForce_Y(-0.0001f);
	}
	if (eventSystem->isPressed(Key::S)) {
		PO2->incrementForce_Y(0.0001f);
	}
	if (eventSystem->isPressed(Key::A)) {
		PO2->incrementForce_X(-0.0001f);
	}
	if (eventSystem->isPressed(Key::D)) {
		PO2->incrementForce_X(0.0001f);
	}
	
	/*PO3 controls*/
	if (eventSystem->isPressed(Key::T)) {
		PO3->incrementForce_Y(-0.0001f);
	}
	if (eventSystem->isPressed(Key::G)) {
		PO3->incrementForce_Y(0.0001f);
	}
	if (eventSystem->isPressed(Key::F)) {
		PO3->incrementForce_X(-0.0001f);
	}
	if (eventSystem->isPressed(Key::H)) {
		PO3->incrementForce_X(0.0001f);
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
	physics->mechanics(*floor);
#pragma endregion
}

void TestGame::handleCollisions() {
#pragma region COLLISION
	//PhysicsObject / PhysicsObject collisions
	physics->collision(*PO1, *PO2);
	physics->collision(*PO1, *PO3);
	physics->collision(*PO2, *PO3);
	physics->collision(*PO1, *floor);
#pragma endregion
}

void TestGame::handleGravitation() {
#pragma region GRAVITATION
	/*Physics engine calculates the gravitational force that
	exists between the gravitating PhysicsObjects*/
	PO3->setForce_X(physics->calculuateGravitationalForce(*PO2, *PO3).x);
	PO3->setForce_Y(physics->calculuateGravitationalForce(*PO2, *PO3).y);
#pragma endregion
}

void TestGame::reset() {
#pragma region RESET
	PO1->setRootTransform(PO1_initRootTransform);
	PO1->setSpeed(PO1_initSpeed);
	PO1->setVelocity(PO1_initVelocity);
	PO1->setForce(PO1_initForce);

	PO2->setRootTransform(PO2_initRootTransform);
	PO2->setSpeed(PO2_initSpeed);
	PO2->setVelocity(PO2_initVelocity);
	PO2->setForce(PO2_initForce);

	PO3->setRootTransform(PO3_initRootTransform);
	PO3->setSpeed(PO3_initSpeed);
	PO3->setVelocity(PO3_initVelocity);
	PO3->setForce(PO3_initForce);
#pragma endregion
}

/*
	Executes once every tick.
*/
void TestGame::update() {
	physics->update();
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

	gfx->setDrawColor(SDL_COLOR_WHITE);
	gfx->drawCircle(Point2(floor->getRootTransform().x, floor->getRootTransform().y), 5.0f);
	gfx->drawRect(floor->getCollider());
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
		gfx->drawText(PO1_tag, PO1->getCollider().x, PO1->getCollider().y - 30.f);
		gfx->drawText("X:", PO1->getCollider().x + 50.f, PO1->getCollider().y - 30.f);
		gfx->drawText(std::to_string(PO1->getCollider().x), PO1->getCollider().x + 90.f, PO1->getCollider().y - 30.f);
		gfx->drawText("Y:", PO1->getCollider().x + 50.f, PO1->getCollider().y);
		gfx->drawText(std::to_string(PO1->getCollider().y), PO1->getCollider().x + 90.f, PO1->getCollider().y);
		gfx->drawText("F:", PO1->getCollider().x + 50.f, PO1->getCollider().y + 30.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO1->getForce())), PO1->getCollider().x + 90.f, PO1->getCollider().y + 30.f);
		gfx->drawText("M:", PO1->getCollider().x + 50.f, PO1->getCollider().y + 60.f);
		gfx->drawText(std::to_string(PO1->getMass()), PO1->getCollider().x + 90.f, PO1->getCollider().y + 60.f);
		gfx->drawText("A:", PO1->getCollider().x + 50.f, PO1->getCollider().y + 90.f);
		gfx->drawText(std::to_string(PO1->getAcceleration().x), PO1->getCollider().x + 90.f, PO1->getCollider().y + 90.f);
		gfx->drawText("V:", PO1->getCollider().x + 50.f, PO1->getCollider().y + 120.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO1->getSpeed())), PO1->getCollider().x + 90.f, PO1->getCollider().y + 120.f);
		gfx->drawText("MV:", PO1->getCollider().x + 50.f, PO1->getCollider().y  + 150.f);
		gfx->drawText(std::to_string(physics->calculateResultant(physics->calculateMomentum(*PO1))), PO1->getCollider().x + 90.f, PO1->getCollider().y + 150.f);

		/*RED TEXT*/
		gfx->setDrawColor(SDL_COLOR_RED);
		gfx->drawText(PO2_tag, PO2->getCollider().x, PO2->getCollider().y - 30.f);
		gfx->drawText("X:", PO2->getCollider().x + 50.f, PO2->getCollider().y - 30.f);
		gfx->drawText(std::to_string(PO2->getCollider().x), PO2->getCollider().x + 90.f, PO2->getCollider().y - 30.f);
		gfx->drawText("Y:", PO2->getCollider().x + 50.f, PO2->getCollider().y);
		gfx->drawText(std::to_string(PO2->getCollider().y), PO2->getCollider().x + 90.f, PO2->getCollider().y);
		gfx->drawText("F:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 30.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO2->getForce())), PO2->getCollider().x + 90.f, PO2->getCollider().y + 30.f);
		gfx->drawText("M:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 60.f);
		gfx->drawText(std::to_string(PO2->getMass()), PO2->getCollider().x + 90.f, PO2->getCollider().y + 60.f);
		gfx->drawText("A:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 90.f);
		gfx->drawText(std::to_string(PO2->getAcceleration().x), PO2->getCollider().x + 90.f, PO2->getCollider().y + 90.f);
		gfx->drawText("V:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 120.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO2->getSpeed())), PO2->getCollider().x + 90.f, PO2->getCollider().y + 120.f);
		gfx->drawText("MV:", PO2->getCollider().x + 50.f, PO2->getCollider().y + 150.f);
		gfx->drawText(std::to_string(physics->calculateResultant(physics->calculateMomentum(*PO2))), PO2->getCollider().x + 90.f, PO2->getCollider().y + 150.f);

		/*GREEN TEXT*/
		gfx->setDrawColor(SDL_COLOR_GREEN);
		gfx->drawText(PO3_tag, PO3->getCollider().x, PO3->getCollider().y - 30.f);
		gfx->drawText("X:", PO3->getCollider().x + 50.f, PO3->getCollider().y - 30.f);
		gfx->drawText(std::to_string(PO3->getCollider().x), PO3->getCollider().x + 90.f, PO3->getCollider().y - 30.f);
		gfx->drawText("Y:", PO3->getCollider().x + 50.f, PO3->getCollider().y);
		gfx->drawText(std::to_string(PO3->getCollider().y), PO3->getCollider().x + 90.f, PO3->getCollider().y);
		gfx->drawText("F:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 30.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO3->getForce())), PO3->getCollider().x + 90.f, PO3->getCollider().y + 30.f);
		gfx->drawText("M:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 60.f);
		gfx->drawText(std::to_string(PO3->getMass()), PO3->getCollider().x + 90.f, PO3->getCollider().y + 60.f);
		gfx->drawText("A:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 90.f);
		gfx->drawText(std::to_string(PO3->getAcceleration().x), PO3->getCollider().x + 90.f, PO3->getCollider().y + 90.f);
		gfx->drawText("V:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 120.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO3->getSpeed())), PO3->getCollider().x + 90.f, PO3->getCollider().y + 120.f);
		gfx->drawText("MV:", PO3->getCollider().x + 50.f, PO3->getCollider().y + 150.f);
		gfx->drawText(std::to_string(physics->calculateResultant(physics->calculateMomentum(*PO3))), PO3->getCollider().x + 90.f, PO3->getCollider().y + 150.f);
	}
#pragma endregion
}
