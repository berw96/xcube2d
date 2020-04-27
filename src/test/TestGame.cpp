#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), score(0), lives(3), keys(5), gameWon(false), bool_isPlaying_val(true), uint32_timeLimit_val(SECOND * 5), box(5, 5, 15, 15), light(0, 0, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_HEIGHT) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	gen = new MazeGenerator(10, 10); //generate a 10 X 10 template for the maze.
	gen->generateMaze(0, 0);

	timer_idleCountdown_adr = new Timer();
	timer_idleCountdown_adr->reset();

	ai_artificialIntelligence_adr = new AI();
	ai_artificialIntelligence_adr->init_randomNumber_val(0, 100);

	/*
	The lines of code below set up the game map using the MazeGenerator.
	It is here that the proverbial tiles can be generate too which the AI 
	can use for its pathfinding behaviour. Provide/find an attribute for each
	tile which the AI can use to recognise the following:
	
		• Which tile it is on. 
		• Which directions on that tile it can go in.
	*/
	int dist = 40;

	for (int i = 0; i < gen->y; ++i) {

		int y = i * dist;

		for (int j = 0; j < gen->x; ++j) {

			int x = j * dist;

			//THESE RECOGNIZE THE WALLS - use just the North (N) and West (W) walls, you can determine the East and South.
			//&n (e.g. & 1) represents the dir.bit of the direction.
			//How logical 'and' works for bits 0100 & 0001 ~ truth table, think logically not mathematically.
			/*
			& 1 = North
			& 2 = South
			& 4 = East
			& 8 = West
			*/
			//Top wall - N
			if ((gen->maze[j][i] & 1) == 0) {
				lines.push_back(std::make_shared<Line2i>(Point2(x, y), Point2(x+dist, y)));
			}
			//Left wall - W
			if ((gen->maze[j][i] & 8) == 0) {
				lines.push_back(std::make_shared<Line2i>(Point2(x, y), Point2(x, y+dist)));
			}//iteratate over all the tiles of the maze using this as a reference.

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
	int speed = 2;							//player character speed
	int dist = 40;							//maze coordinate scaling
	if (eventSystem->isPressed(Key::W) || 
		eventSystem->isPressed(Key::UP)) {
		velocity.y = -speed;
		bool_isPlaying_val = true;
		timer_idleCountdown_adr->reset();
	}

	if (eventSystem->isPressed(Key::S) || 
		eventSystem->isPressed(Key::DOWN)) {
		velocity.y = speed;
		bool_isPlaying_val = true;
		timer_idleCountdown_adr->reset();
	}

	if (eventSystem->isPressed(Key::A) || 
		eventSystem->isPressed(Key::LEFT)) {
		velocity.x = -speed;
		bool_isPlaying_val = true;
		timer_idleCountdown_adr->reset();
	}

	if (eventSystem->isPressed(Key::D) || 
		eventSystem->isPressed(Key::RIGHT)) {
		velocity.x = speed;
		bool_isPlaying_val = true;
		timer_idleCountdown_adr->reset();
	}
	
	//Artificial Intelligence
	/*if the timer exceeds the time limit, AI behaviour is invoked to move the player character.*/
	timer_idleCountdown_adr->incrementTime();
	if (timer_idleCountdown_adr->getTime() >= uint32_timeLimit_val) {
		bool_isPlaying_val = false;
		bool bool_hasChosenDirection_val = false;

		if (bool_isPlaying_val == false) {
			//scans the entire maze tile by tile
			
			for (int i = 0; i < gen->y; ++i) {
				int y = i * dist; //scaling y
				for (int j = 0; j < gen->x; ++j) {
					int x = j * dist; //scaling x

					Point2 point2_tile_val = Point2(x + dist/2, y + dist/2); //creates a point at i and j.
					//interpret integer as a Point2, this way the contains() methods will receive the correct datatype.

					//Tile recognition goes here, the basis of the AI is to tell it what it CANNOT DO
					if(box.contains(point2_tile_val)){ //A.I. recognizes when it's on a tile via player box

						//If player contains tile (it does at this point), return the valid directions which this tile has.
						//Isolate the range of choices - get all the valid directions which the tile allows;
						ai_artificialIntelligence_adr->void_randomMovement_val(ai_artificialIntelligence_adr->get_randomNumber_val());
					}
					
				}
			}
			
		}

	}//uint32_timeLimit_val is used as a buffer for the time variable to maintain the same return value for getElapsed();
	//Artificial Intelligence
}

void TestGame::update() {
	box.x += velocity.x;
	for (auto line : lines) {
		if (box.intersects(*line)) {
			box.x -= velocity.x;
			break;
		}
	}//Collision Detection (HORIZONTAL)

	box.y += velocity.y;
	for (auto line : lines) {
		if (box.intersects(*line)) {
			box.y -= velocity.y;
			break;
		}
	}//Collision Detection (VERTICAL)

	for (auto key : points) {
		if (key->alive && box.contains(key->pos)) { //player overlaps a collectable.
			score += 200;
			key->alive = false;
			keys--;
		}
	}//Collision Detection (POINT)

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