#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), box(235.f, 235.f, 30.f, 30.f), gravityToggled(false), speed(0.f, 0.f), force(0.01f ,0.01f), mass(0.1f), acceleration(0.f, 0.f) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 15);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

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
	delete gen;
}

void TestGame::handleKeyEvents() {

	/*Physics engine calculates the appropriate acceleration
	for the player box.*/
	acceleration.x = physics->calculateAcceleration_x(force, mass);
	acceleration.y = physics->calculateAcceleration_y(force, mass);

	velocity.x = speed.x;
	velocity.y = speed.y;

	if (eventSystem->isPressed(Key::UP) &&
		gravityToggled != true) {
		speed.y -= acceleration.y;
	}

	if (eventSystem->isPressed(Key::DOWN) &&
		gravityToggled != true) {
		speed.y += acceleration.y;
	}

	if (eventSystem->isPressed(Key::LEFT)) {
		speed.x -= acceleration.x;
	}

	if (eventSystem->isPressed(Key::RIGHT)) {
		speed.x += acceleration.x;
	}

	if (gravityToggled == true) {
		speed.y += 0.981f;
	}

	/*Resets the player transform and velocity*/
	if (eventSystem->isPressed(Key::R)) {
		box.x = 235.f;
		box.y = 235.f;
		gravityToggled = false;
		speed.x = 0.f;
		speed.y = 0.f;
		velocity = Vector2f(0.f,0.f);
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
	physics->update();

	box.x += velocity.x;
	for (auto line : lines) {
		if (box.intersects(*line)) {
			speed.x = 0.f;
			box.x -= velocity.x;
			break;
		}
	}

	box.y += velocity.y;
	for (auto line : lines) {
		if (box.intersects(*line)) {
			speed.y = 0.f;
			box.y -= velocity.y;
			break;
		}
	}

	/*Calculates the resultant velocity of the player box
	using Pythagorean theorem.*/
	speed_res = physics->calculateResultant(speed);
}

void TestGame::render() {
	gfx->setDrawColor(SDL_COLOR_WHITE);
	for (auto line : lines)
			gfx->drawLine(line->start, line->end);
	

	gfx->setDrawColor(SDL_COLOR_AQUA);
	gfx->drawRect(box);
}

/*
	Renders in-game text for the User Interface.
*/
void TestGame::renderUI() {

	/*WHITE TEXT*/
	gfx->setDrawColor(SDL_COLOR_WHITE);
	std::string x_str = std::to_string(box.x - 185.f);
	std::string y_str = std::to_string(box.y - 185.f);
	std::string speedX_str = std::to_string(speed.x);
	std::string speedY_str = std::to_string(speed.y);
	std::string speedRes_str = std::to_string(speed_res);
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

	/*RED TEXT*/
	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawText("X:", 600, 60);	
	gfx->drawText(x_str, 630, 60);

	/*GREEN TEXT*/
	gfx->setDrawColor(SDL_COLOR_GREEN);
	gfx->drawText("Y:", 600, 90);	
	gfx->drawText(y_str, 630, 90);
}
