#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), UI_Toggled(true) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 12);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

<<<<<<< HEAD
<<<<<<< HEAD
	/*Creates a PO1 PhysicsObject with a mass and init xy-transform*/
	PO1 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 0.1f, Vector2f(235.f, 235.f));
	PO2 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 0.4f, Vector2f(135.f, 135.f), Rectf(135.f, 135.f, 30.f, 30.f));
	PO3 = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 200.0f, Vector2f(335.f, 335.f), Rectf(335.f, 335.f, 30.f, 30.f));
=======
=======
>>>>>>> parent of 6d5bfae... 190820-14:56
	/*Creates PhysicsObjects with mass and init xy-transform*/

	physics->registerObject(PO1);
	physics->registerObject(PO2);
	physics->registerObject(PO3);

#pragma endregion
>>>>>>> parent of 6d5bfae... 190820-14:56
}

TestGame::~TestGame() {
	/*Deletes raw pointers*/
	delete PO1;
	delete PO2;
	delete PO3;
}

void TestGame::handleKeyEvents() {

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

<<<<<<< HEAD
	/*Resets the session*/
	if (eventSystem->isPressed(Key::R)) {
		PO1->setRootTransform(Vector2f(235.f, 235.f));
		PO1->setSpeed(Vector2f(0.f, 0.f));
		PO1->setVelocity(Vector2f(0.f, 0.f));
		PO2->setRootTransform(Vector2f(135.f, 135.f));
		PO2->setSpeed(Vector2f(0.f, 0.f));
		PO2->setVelocity(Vector2f(0.f, 0.f));
		PO3->setRootTransform(Vector2f(335.f, 335.f));
		PO3->setSpeed(Vector2f(0.f, 0.f));
		PO3->setVelocity(Vector2f(0.f, 0.f));
	}
=======
void TestGame::handleMechanics() {
#pragma region MECHANICS
	physics->mechanics();
#pragma endregion
>>>>>>> parent of 6d5bfae... 190820-14:56
}

/*
	Executes once every tick.
*/
void TestGame::update() {
	
	/*Calculates the resultant velocity of the PO1
	using Pythagorean theorem.*/
	speed_res = physics->calculateResultant(PO1->getSpeed());

	//speed sets the magnitude of velocity
	PO1->setVelocity_X(PO1->getSpeed().x);
	PO1->setVelocity_Y(PO1->getSpeed().y);
	PO2->setVelocity_X(PO2->getSpeed().x);
	PO2->setVelocity_Y(PO2->getSpeed().y);
	PO3->setVelocity_X(PO3->getSpeed().x);
	PO3->setVelocity_Y(PO3->getSpeed().y);

	/*Physics engine calculates the appropriate acceleration
	for each PhysicsObject using its mass and set force vector.*/
	PO1->setAcceleration_X(physics->calculateAcceleration_x(PO1->getForce(), PO1->getMass()));
	PO1->setAcceleration_Y(physics->calculateAcceleration_y(PO1->getForce(), PO1->getMass()));
	PO2->setAcceleration_X(physics->calculateAcceleration_x(PO2->getForce(), PO2->getMass()));
	PO2->setAcceleration_Y(physics->calculateAcceleration_y(PO2->getForce(), PO2->getMass()));
	PO3->setAcceleration_X(physics->calculateAcceleration_x(PO3->getForce(), PO3->getMass()));
	PO3->setAcceleration_Y(physics->calculateAcceleration_y(PO3->getForce(), PO3->getMass()));

	//velocity moves PhysicsObjects
	PO1->setRootTransform_X(PO1->getVelocity().x);
	PO1->setRootTransform_Y(PO1->getVelocity().y);
	PO2->setRootTransform_X(PO2->getVelocity().x);
	PO2->setRootTransform_Y(PO2->getVelocity().y);
	PO3->setRootTransform_X(PO3->getVelocity().x);
	PO3->setRootTransform_Y(PO3->getVelocity().y);

	//collider follows PhysicsObject (implicit child)
	PO1->setColliderTransform_X(PO1->getRootTransform());
	PO1->setColliderTransform_Y(PO1->getRootTransform());
	PO2->setColliderTransform_X(PO2->getRootTransform());
	PO2->setColliderTransform_Y(PO2->getRootTransform());
	PO3->setColliderTransform_X(PO3->getRootTransform());
	PO3->setColliderTransform_Y(PO3->getRootTransform());

	//PhysicsObject / PhysicsObject collisions
	/*if (PO1->getCollider().intersects(PO2->getCollider())) {
		PO1->setRootTransform(Vector2f(PO1->getRootTransform().x - PO1->getVelocity().x, PO1->getRootTransform().y));
		PO2->setSpeed_X(PO1->getSpeed().x + PO2->getSpeed().x);
	}
	
	if (PO1->getCollider().intersects(PO2->getCollider())) {
		PO1->setRootTransform(Vector2f(PO1->getRootTransform().x, PO1->getRootTransform().y - PO1->getVelocity().y));
		PO2->setSpeed_Y(PO1->getSpeed().y + PO2->getSpeed().y);
	}
	
	if (PO1->getCollider().intersects(PO3->getCollider())) {
		PO1->setRootTransform(Vector2f(PO1->getRootTransform().x - PO1->getVelocity().x, PO1->getRootTransform().y));
		PO3->setSpeed_X(PO1->getSpeed().x + PO3->getSpeed().x);
	}
	
	if (PO1->getCollider().intersects(PO3->getCollider())) {
		PO1->setRootTransform(Vector2f(PO1->getRootTransform().x, PO1->getRootTransform().y - PO1->getVelocity().y));
		PO3->setSpeed_Y(PO1->getSpeed().y + PO3->getSpeed().y);
	}
	
	if (PO2->getCollider().intersects(PO1->getCollider())) {
		PO2->setRootTransform(Vector2f(PO2->getRootTransform().x - PO2->getVelocity().x, PO2->getRootTransform().y));
		PO1->setSpeed_X(PO2->getSpeed().x + PO1->getSpeed().x);
	}
	
	if (PO2->getCollider().intersects(PO1->getCollider())) {
		PO2->setRootTransform(Vector2f(PO2->getRootTransform().x, PO2->getRootTransform().y - PO2->getVelocity().y));
		PO1->setSpeed_Y(PO2->getSpeed().y + PO1->getSpeed().y);
	}
	
	if (PO2->getCollider().intersects(PO3->getCollider())) {
		PO2->setRootTransform(Vector2f(PO2->getRootTransform().x - PO2->getVelocity().x, PO2->getRootTransform().y));
		PO3->setSpeed_X(PO2->getSpeed().x + PO3->getSpeed().x);
	}
	
	if (PO2->getCollider().intersects(PO3->getCollider())) {
		PO2->setRootTransform(Vector2f(PO2->getRootTransform().x, PO2->getRootTransform().y - PO2->getVelocity().y));
		PO3->setSpeed_Y(PO2->getSpeed().y + PO3->getSpeed().y);
	}
	
	if (PO3->getCollider().intersects(PO1->getCollider())) {
		PO3->setRootTransform(Vector2f(PO3->getRootTransform().x - PO3->getVelocity().x, PO3->getRootTransform().y));
		PO1->setSpeed_X(PO3->getSpeed().x + PO1->getSpeed().x);
	}
	
	if (PO3->getCollider().intersects(PO1->getCollider())) {
		PO3->setRootTransform(Vector2f(PO3->getRootTransform().x, PO3->getRootTransform().y - PO3->getVelocity().y));
		PO1->setSpeed_Y(PO3->getSpeed().y + PO1->getSpeed().y);
	}*/

	//Gravitation
	PO2->moveTo(PO3->getRootTransform(), physics->calculuateGravitationalForce(*PO2, *PO3));
	PO3->moveTo(PO2->getRootTransform(), physics->calculuateGravitationalForce(*PO2, *PO3));
}

/*
	Renders geometry.
*/
void TestGame::render() {
	gfx->setDrawColor(SDL_COLOR_AQUA);
	gfx->drawRect(PO1->getCollider());
	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawRect(PO2->getCollider());
	gfx->setDrawColor(SDL_COLOR_GREEN);
	gfx->drawRect(PO3->getCollider());
	gfx->setDrawColor(SDL_COLOR_WHITE);
	gfx->drawCircle(Point2(PO3->getRootTransform().x, PO3->getRootTransform().y), 1.f);
}

/*
	Renders in-game text for the User Interface.
*/
void TestGame::renderUI() {
	std::string PO1_tag = "PO1";
	std::string PO2_tag = "PO2";
	std::string PO3_tag = "PO3";

	/*If the UI is toggled it will display the physical fields of the PhysicsObjects*/
	if (UI_Toggled == true) {

		/*AQUA TEXT*/
		gfx->setDrawColor(SDL_COLOR_AQUA);
		gfx->drawText(PO1_tag, PO1->getRootTransform().x, PO1->getRootTransform().y - 20.f);
		gfx->drawText("X:", PO1->getRootTransform().x + 50.f, PO1->getRootTransform().y - 15.f);
		gfx->drawText(std::to_string(PO1->getRootTransform().x), PO1->getRootTransform().x + 70.f, PO1->getRootTransform().y - 15.f);
		gfx->drawText("Y:", PO1->getRootTransform().x + 50.f, PO1->getRootTransform().y);
		gfx->drawText(std::to_string(PO1->getRootTransform().y), PO1->getRootTransform().x + 70.f, PO1->getRootTransform().y);
		gfx->drawText("F:", PO1->getRootTransform().x + 50.f, PO1->getRootTransform().y + 15.f);
		gfx->drawText(std::to_string(PO1->getForce().x), PO1->getRootTransform().x + 70.f, PO1->getRootTransform().y + 15.f);
		gfx->drawText("M:", PO1->getRootTransform().x + 50.f, PO1->getRootTransform().y + 30.f);
		gfx->drawText(std::to_string(PO1->getMass()), PO1->getRootTransform().x + 70.f, PO1->getRootTransform().y + 30.f);
		gfx->drawText("A:", PO1->getRootTransform().x + 50.f, PO1->getRootTransform().y + 45.f);
		gfx->drawText(std::to_string(PO1->getAcceleration().x), PO1->getRootTransform().x + 70.f, PO1->getRootTransform().y + 45.f);
		gfx->drawText("V:", PO1->getRootTransform().x + 50.f, PO1->getRootTransform().y + 60.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO1->getSpeed())), PO1->getRootTransform().x + 70.f, PO1->getRootTransform().y + 60.f);

		/*RED TEXT*/
		gfx->setDrawColor(SDL_COLOR_RED);
		gfx->drawText(PO2_tag, PO2->getRootTransform().x, PO2->getRootTransform().y - 20.f);
		gfx->drawText("X:", PO2->getRootTransform().x + 50.f, PO2->getRootTransform().y - 15.f);
		gfx->drawText(std::to_string(PO2->getRootTransform().x), PO2->getRootTransform().x + 70.f, PO2->getRootTransform().y - 15.f);
		gfx->drawText("Y:", PO2->getRootTransform().x + 50.f, PO2->getRootTransform().y);
		gfx->drawText(std::to_string(PO2->getRootTransform().y), PO2->getRootTransform().x + 70.f, PO2->getRootTransform().y);
		gfx->drawText("F:", PO2->getRootTransform().x + 50.f, PO2->getRootTransform().y + 15.f);
		gfx->drawText(std::to_string(PO2->getForce().x), PO2->getRootTransform().x + 70.f, PO2->getRootTransform().y + 15.f);
		gfx->drawText("M:", PO2->getRootTransform().x + 50.f, PO2->getRootTransform().y + 30.f);
		gfx->drawText(std::to_string(PO2->getMass()), PO2->getRootTransform().x + 70.f, PO2->getRootTransform().y + 30.f);
		gfx->drawText("A:", PO2->getRootTransform().x + 50.f, PO2->getRootTransform().y + 45.f);
		gfx->drawText(std::to_string(PO2->getAcceleration().x), PO2->getRootTransform().x + 70.f, PO2->getRootTransform().y + 45.f);
		gfx->drawText("V:", PO2->getRootTransform().x + 50.f, PO2->getRootTransform().y + 60.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO2->getSpeed())), PO2->getRootTransform().x + 70.f, PO2->getRootTransform().y + 60.f);

		/*GREEN TEXT*/
		gfx->setDrawColor(SDL_COLOR_GREEN);
		gfx->drawText(PO3_tag, PO3->getRootTransform().x, PO3->getRootTransform().y - 20.f);
		gfx->drawText("X:", PO3->getRootTransform().x + 50.f, PO3->getRootTransform().y - 15.f);
		gfx->drawText(std::to_string(PO3->getRootTransform().x), PO3->getRootTransform().x + 70.f, PO3->getRootTransform().y - 15.f);
		gfx->drawText("Y:", PO3->getRootTransform().x + 50.f, PO3->getRootTransform().y);
		gfx->drawText(std::to_string(PO3->getRootTransform().y), PO3->getRootTransform().x + 70.f, PO3->getRootTransform().y);
		gfx->drawText("F:", PO3->getRootTransform().x + 50.f, PO3->getRootTransform().y + 15.f);
		gfx->drawText(std::to_string(PO3->getForce().x), PO3->getRootTransform().x + 70.f, PO3->getRootTransform().y + 15.f);
		gfx->drawText("M:", PO3->getRootTransform().x + 50.f, PO3->getRootTransform().y + 30.f);
		gfx->drawText(std::to_string(PO3->getMass()), PO3->getRootTransform().x + 70.f, PO3->getRootTransform().y + 30.f);
		gfx->drawText("A:", PO3->getRootTransform().x + 50.f, PO3->getRootTransform().y + 45.f);
		gfx->drawText(std::to_string(PO3->getAcceleration().x), PO3->getRootTransform().x + 70.f, PO3->getRootTransform().y + 45.f);
		gfx->drawText("V:", PO3->getRootTransform().x + 50.f, PO3->getRootTransform().y + 60.f);
		gfx->drawText(std::to_string(physics->calculateResultant(PO3->getSpeed())), PO3->getRootTransform().x + 70.f, PO3->getRootTransform().y + 60.f);
	}
}
