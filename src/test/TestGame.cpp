#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), gravityToggled(false), force(0.01f ,0.01f) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 15);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	/*Creates a player PhysicsObject with a mass and init xy-transform*/
	player = new PhysicsObject(Point2(0.f, 0.f), 30.f, 30.f, 0.1f, Vector2f(235.f, 235.f));

	/*Generates a 1x1 maze with a 500x500 perimeter*/
	gen = new MazeGenerator(1, 1);
	gen->generateMaze(0, 0);

	int dist = 500;

	for (int i = 0; i < gen->y; ++i) {

		int y = i * dist;

		for (int j = 0; j < gen->x; ++j) {

			int x = j * dist;

			if ((gen->maze[j][i] & 1) == 0) {
				lines.push_back(std::make_shared<Line2i>(Point2(x, y), Point2(x + dist, y)));
			}

			if ((gen->maze[j][i] & 8) == 0) {
				lines.push_back(std::make_shared<Line2i>(Point2(x, y), Point2(x, y + dist)));
			}
		}

		lines.push_back(std::make_shared<Line2i>(Point2(gen->x * dist, y), Point2(gen->x * dist, y + dist)));
	}

	for (int j = 0; j < gen->x; j++) {
		int x = j * dist;
		lines.push_back(std::make_shared<Line2i>(Point2(x, gen->y * dist), Point2(x + dist, gen->y * dist)));
	}
}

TestGame::~TestGame() {
	delete player;
	delete gen;
}

void TestGame::handleKeyEvents() {

	//speed sets the velocity
	player->setVelocity_X(player->getSpeed().x);
	player->setVelocity_Y(player->getSpeed().y);

	/*Physics engine calculates the appropriate acceleration
	for the player.*/
	player->setAcceleration_X(physics->calculateAcceleration_x(force, player->getMass()));
	player->setAcceleration_Y(physics->calculateAcceleration_y(force, player->getMass()));

	//acceleraton sets the speed
	if (eventSystem->isPressed(Key::UP) &&
		gravityToggled != true) {
		player->setSpeed_Y(player->getSpeed().y - player->getAcceleration().y);
	}

	if (eventSystem->isPressed(Key::DOWN) &&
		gravityToggled != true) {
		player->setSpeed_Y(player->getSpeed().y + player->getAcceleration().y);
	}

	if (eventSystem->isPressed(Key::LEFT)) {
		player->setSpeed_X(player->getSpeed().x - player->getAcceleration().x);
	}

	if (eventSystem->isPressed(Key::RIGHT)) {
		player->setSpeed_X(player->getSpeed().x + player->getAcceleration().x);
	}

	if (gravityToggled == true) {
		player->setSpeed_Y(player->getSpeed().y + 0.981f);
	}

	/*Resets the player transform and velocity*/
	if (eventSystem->isPressed(Key::R)) {
		gravityToggled = false;
		player->setRootTransform(Vector2f(235.f, 235.f));
		player->setSpeed(Vector2f(0.f, 0.f));
		player->setVelocity(Vector2f(0.f,0.f));
	}

	/*Toggles gravity (ON)*/
	if (eventSystem->isPressed(Key::G)) {
		if (gravityToggled != true) {
			gravityToggled = true;
		}
	}
	
	/*Toggles gravity (OFF)*/
	if (eventSystem->isPressed(Key::T)) {
		if (gravityToggled != false) {
			gravityToggled = false;
		}
	}
}

void TestGame::update() {
	//physics engine should handle collisions, TestGame handles setup
	physics->update();

	//velocity moves the player
	player->setRootTransform_X(player->getVelocity());
	player->setRootTransform_Y(player->getVelocity());

	//collider follows the player (implicit child)
	player->setColliderTransform_X(player->getRootTransform());
	player->setColliderTransform_Y(player->getRootTransform());

	for (auto line : lines) {
		if (player->getCollider().intersects(*line)) {
			player->setSpeed_X(0.f);
			//box.x -= velocity.x;
			//player->setRootTransform_X();
			break;
		}
	}

	for (auto line : lines) {
		if (player->getCollider().intersects(*line)) {
			player->setSpeed_Y(0.f);
			//box.x -= velocity.x;
			//player->setRootTransform_Y();
			break;
		}
	}
	
	/*Calculates the resultant velocity of the player
	using Pythagorean theorem.*/
	speed_res = physics->calculateResultant(player->getSpeed());
}

void TestGame::render() {
	gfx->setDrawColor(SDL_COLOR_WHITE);
	for (auto line : lines)
			gfx->drawLine(line->start, line->end);
	

	gfx->setDrawColor(SDL_COLOR_AQUA);
	gfx->drawRect(player->getCollider());
}

/*
	Renders in-game text for the User Interface.
*/
void TestGame::renderUI() {

	/*WHITE TEXT*/
	gfx->setDrawColor(SDL_COLOR_WHITE);
	std::string x_str = std::to_string(player->getRootTransform().x - 185.f);
	std::string y_str = std::to_string(player->getRootTransform().y - 185.f);
	std::string speedX_str = std::to_string(player->getSpeed().x);
	std::string speedY_str = std::to_string(player->getSpeed().y);
	std::string speedRes_str = std::to_string(speed_res);
	std::string acceleration_str = std::to_string(player->getAcceleration().x);
	std::string gravityToggle_str;

	if (gravityToggled == true) {
		gravityToggle_str = "ON";
	}
	if (gravityToggled != true) {
		gravityToggle_str = "OFF";
	}

	gfx->drawText("Gravity:", 600, 30);
	gfx->drawText(gravityToggle_str, 660, 30);

	gfx->drawText("V:", 600, 120);
	gfx->drawText(speedRes_str, 630, 120);

	gfx->drawText("A:", 600, 150);
	gfx->drawText(acceleration_str, 630, 150);

	/*RED TEXT*/
	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawText("X:", 600, 60);	
	gfx->drawText(x_str, 630, 60);

	/*GREEN TEXT*/
	gfx->setDrawColor(SDL_COLOR_GREEN);
	gfx->drawText("Y:", 600, 90);	
	gfx->drawText(y_str, 630, 90);
}
