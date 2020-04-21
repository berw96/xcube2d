#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), score(0), lives(3), keys(5), gameWon(false), is_Playing(true), time_limit(SECOND * 5), box(5, 5, 30, 30), light(0, 0, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_HEIGHT) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	gen = new MazeGenerator(15, 15); //generate a 15 X 15 template for the maze.
	gen->generateMaze(0, 0);

	timer = new Timer();
	timer->reset();

	int dist = 40;

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

			if (keys > 0 && getRandom(0, 100) <= 5) {
				std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
				k->alive = true;
				k->pos = Point2(j*dist + dist/2, i*dist + dist/2);
				points.push_back(k);
				keys--;
			}
		}

		lines.push_back(std::make_shared<Line2i>(Point2(gen->x*dist, y), Point2(gen->x*dist, y + dist)));
	}

	for (int j = 0; j < gen->x; j++) {
		int x = j * dist;
		lines.push_back(std::make_shared<Line2i>(Point2(x, gen->y * dist), Point2(x + dist, gen->y * dist)));
	}

	keys = 5;
}//TestGame()

TestGame::~TestGame() {
	delete gen;
}//~TestGame()

void TestGame::handleKeyEvents() {
	int speed = 3;							//player character speed

	if (eventSystem->isPressed(Key::W) || 
		eventSystem->isPressed(Key::UP)) {
		velocity.y = -speed;
		is_Playing = true;
		timer->reset();
	}

	if (eventSystem->isPressed(Key::S) || 
		eventSystem->isPressed(Key::DOWN)) {
		velocity.y = speed;
		is_Playing = true;
		timer->reset();
	}

	if (eventSystem->isPressed(Key::A) || 
		eventSystem->isPressed(Key::LEFT)) {
		velocity.x = -speed;
		is_Playing = true;
		timer->reset();
	}

	if (eventSystem->isPressed(Key::D) || 
		eventSystem->isPressed(Key::RIGHT)) {
		velocity.x = speed;
		is_Playing = true;
		timer->reset();
	}

	//Artificial Intelligence

	/*if the timer exceeds the time limit, AI behaviour is invoked to move the player character.*/
	timer->incrementTime();
	if (timer->getTime() >= time_limit) {
		is_Playing = false;

		//Random Movement
		random_number = { getRandom(0,100) };

		if (is_Playing == false) {
			if (random_number > 0 &&
				random_number <= 25) {
				velocity.y = -speed;
			}

			if (random_number > 25 &&
				random_number <= 50) {
				velocity.y = speed;
			}

			if (random_number > 50 &&
				random_number <= 75) {
				velocity.x = -speed;
			}

			if (random_number > 75 &&
				random_number < 100) {
				velocity.x = speed;
			}
		}
		//Random Movement

	}//time_limit is used as a buffer for the time variable to maintain the same return value for getElapsed();
	//Artificial Intelligence
}

void TestGame::update() {
	//Collision Detection
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
	//Collision Detection

	for (auto key : points) {
		if (key->alive && box.contains(key->pos)) {
			score += 200;
			key->alive = false;
			keys--;
		}
	}

	velocity = Vector2i(0, 0);

	light.x = box.x - DEFAULT_WINDOW_HEIGHT / 2;
	light.y = box.y - DEFAULT_WINDOW_HEIGHT / 2;

	if (keys == 0) {
		gameWon = true;
	}
}

void TestGame::render() {
	gfx->setDrawColor(SDL_COLOR_WHITE);
	for (auto line : lines)
		if (light.intersects(*line))
			gfx->drawLine(line->start, line->end);
	
	gfx->setDrawColor(SDL_COLOR_GREEN);
	gfx->drawRect(box);

	gfx->setDrawColor(SDL_COLOR_YELLOW);
	for (auto key : points)
		if (key->alive && light.contains(key->pos))
			gfx->drawCircle(key->pos, 2.5f);
}

void TestGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_GREEN);
	std::string scoreStr = std::to_string(score);
	gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);

	if (gameWon)
		gfx->drawText("YOU WON", 250, 500);
}