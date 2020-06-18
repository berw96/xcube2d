#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), box(185, 185, 30, 30), light(0, 0, 800, 800) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 15);
	gfx->useFont(font);
	gfx->setVerticalSync(true);
	
	/*Generates a 1x1 maze with a 400x400 perimeter*/
	gen = new MazeGenerator(1, 1);
	gen->generateMaze(0, 0);
	int dist = 400;

	for (int i = 0; i < gen->y; ++i) {

		int y = i * dist;

		for (int j = 0; j < gen->x; ++j) {

			int x = j * dist;

			if ((gen->maze[j][i] & 1) == 0) {
				lines.push_back(std::make_shared<Line2i>(Point2(x, y), Point2(x+dist, y)));
			}

			if ((gen->maze[j][i] & 8) == 0) {
				lines.push_back(std::make_shared<Line2i>(Point2(x, y), Point2(x, y+dist)));
			}
		}

		lines.push_back(std::make_shared<Line2i>(Point2(gen->x*dist, y), Point2(gen->x*dist, y + dist)));
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

	int speed = 3;

	if (eventSystem->isPressed(Key::UP)) {
		velocity.y = -speed;
	}

	if (eventSystem->isPressed(Key::DOWN)) {
		velocity.y = speed;
	}

	if (eventSystem->isPressed(Key::LEFT)) {
		velocity.x = -speed;
	}

	if (eventSystem->isPressed(Key::RIGHT)) {
		velocity.x = speed;
	}

	if (eventSystem->isPressed(Key::R)) {
		box.x = 185;
		box.y = 185;
	}

	if (eventSystem->isPressed(Key::G)) {
		std::cout << "Gravity toggled" << std::endl;
	}
}

void TestGame::update() {
	box.x += velocity.x;
	for (auto line : lines) {
		if (box.intersects(*line)) {
			box.x -= velocity.x;
			break;
		}
	}

	box.y += velocity.y;
	for (auto line : lines) {
		if (box.intersects(*line)) {
			box.y -= velocity.y;
			break;
		}
	}

	velocity = Vector2i(0, 0);
}

void TestGame::render() {
	gfx->setDrawColor(SDL_COLOR_WHITE);
	for (auto line : lines)
		if (light.intersects(*line))
			gfx->drawLine(line->start, line->end);
	

	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawRect(box);
}

/*
	Renders in-game text for the User Interface.
*/
void TestGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_WHITE);

	std::string x_str = std::to_string(box.x - 185);
	std::string y_str = std::to_string(box.y - 185);
	std::string gravityToggle_str;

	gfx->drawText("Gravity:", 500, 30);
	gfx->drawText("X:", 500, 60);		
	gfx->drawText(x_str, 530, 60);		
	gfx->drawText("Y:", 500, 90);		
	gfx->drawText(y_str, 530, 90);		
}