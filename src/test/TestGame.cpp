#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), score(0), lives(3), keys(5), gameWon(false), bool_isPlaying_val(true), uint32_timeLimit_val(SECOND * 5), box(5, 5, 20, 20), light(0, 0, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_HEIGHT) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	gen = new MazeGenerator(10, 10); //generate a 10 X 10 template for the maze.
	gen->generateMaze(0, 0);

	timer_idleCountdown_adr = new Timer();
	timer_idleCountdown_adr->reset();

	ai->init_randomNumber_val(0, 100);

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

		if (bool_isPlaying_val == false) {
			//scans the entire maze tile by tile
			for (int i = 0; i < gen->y; ++i) {
				int y = i * dist; //scaling y
				for (int j = 0; j < gen->x; ++j) {
					int x = j * dist; //scaling x

					/*
					Creates a point at i and j as a 'tile'.
					The point is continuously initialized until it is contained by the player box, such the player
					box is 'on the tile'.
					
					After it is decided that the player is on a tile, only then is that tile's possible directions
					assessed and used to invoke an 'appropriate' movement on the player box. This means the AI ONLY
					MAKES DECISIONS BASED ON WHERE IT IS, RATHER THAN WHERE IT NEEDS TO GO. While the AI's current
					tile plays a part in its next movement, it should make decisions based on DIFFERENCE in tile.

					LOGIC:
						The AI currently has no memory implemented so it doesn't recognize the following:
							• Where it has been (Which tiles it has already encountered).
							• Which tile(s) are identical to the previous one (not changing the direction of movement
							just from encountering another tile).
							• What it has already done on each tile (which value of velocity had already been
							used at that tile to make it move).

						To overcome the problem arising from the lack of memory a list of tiles which the player
						box has overlapped should be created. Each time the AI overlaps a tile, that tile can be sent
						as a parameter to a function in the AI subsystem which saves it to a Point2 variable and
						compares it to the list of tiles which the player has touched using a search algorithm. 
						Each tile could have an int ID assigned to it in correspondence to the index at which it
						resides in the maze via the embedded for-loop. This way each tile will be unique.
						
						If the tile is already contained in the list then it has already been encountered by the AI.
						The AI will then check which (valid) directions (values of velocity) it has gone in on that
						tile. This would enable the AI to determine which NEW direction it should go in when on the tile.
						If the tile is not in the list then it will be added to it and build upon the AI's memory.

						Consistency in movement can be achieved by comparing the current tile with the previous tile.
						
						**TRY INVOKING DEFAULT BEHAVIOUR FOR EACH TYPE OF TILE. The A.I. could use it's memory to
						learn an effective route through the maze. (e.g. By default on a straight, go up).**

						The valid directions which can be referenced by the AI can be stored in a std::vector<Dir>
						variable, whose contents can be adapted to reflect the tile which the AI is currently on"

						std::vector<Dir> dir_vector_adr = {}; //no contents

						Below are the logical checks which the AI can conduct to see which valid directions is needs
						to assign as contents to the vector for the tile it is currently on:

						//N
						if((gen->maze[i][j] & 1) == 0){
							dir_vector_adr = {N};
						}

						//W
						if((gen->maze[i][j] & 8) == 0){
							dir_vector_adr = {W};
						}

						//N, W
						if((gen->maze[i][j] & 1) == 0 &&
							(gen->maze[i][j] & 8) == 0){
							dir_vector_adr = {N, W};
						}

						//N, E
						if((gen->maze[i][j] & 1) == 0 &&
							(gen->maze[i][j] & 8) != 0){
							dir_vector_adr = {N, E};
						}

						//S, W
						if((gen->maze[i][j] & 1) != 0 &&
							(gen->maze[i][j] & 8) == 0){
							dir_vector_adr = {S, W};
						}
					*/
					Point2 point2_tile_val = Point2(x + dist/2, y + dist/2);
					//interpret integer as a Point2, this way the contains() methods will receive the correct datatype.

					//Tile recognition goes here, the basis of the AI is to tell it what it CANNOT DO
					if(box.contains(point2_tile_val)){ //A.I. recognizes when it's on a tile via player box

						std::cout << "RNG: " << ai->get_randomNumber_val() << std::endl;

						if ((gen->maze[i][j] & 1) == 0) {
							if (ai->get_randomNumber_val() > 0 &&
								ai->get_randomNumber_val() <= 50) {
								velocity.y = -speed;
							}
							if (ai->get_randomNumber_val() > 50 &&
								ai->get_randomNumber_val() <= 100) {
								velocity.y = speed;
							}
						}

						if ((gen->maze[i][j] & 8) == 0) {
							if (ai->get_randomNumber_val() > 0 &&
								ai->get_randomNumber_val() <= 50) {
								velocity.x = -speed;
							}
							if (ai->get_randomNumber_val() > 50 &&
								ai->get_randomNumber_val() <= 100) {
								velocity.x = speed;
							}
						}
						/*If player contains tile (it does at this point), return the valid directions which this tile has.
						Isolate the range of choices - get all the valid directions which the tile allows;

						Each tile is a point (Point2)
						*/
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

	velocity = Vector2i(0, 0); //velocity.x and velocity.y

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