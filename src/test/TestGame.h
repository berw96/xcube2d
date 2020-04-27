#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"
#include "TestMazeGenerator.h"
#include "../engine/AI.h"

struct GameKey {
	Point2 pos;
	bool alive;
};

/*A struct to define what a tile is based off of the mathematics provided
by GameMath.h and TestMazeGenerator.h*/
/*struct Tile {
	Rectangle2 rect2_tileArea_val;
};*/

class TestGame : public AbstractGame {
	private:
		Rect box;
		Rect light;

		Vector2i velocity;

		MazeGenerator* gen;
		Timer* timer_idleCountdown_adr;
		AI* ai_artificialIntelligence_adr;

		std::vector<std::shared_ptr<Line2i>> lines;
		std::vector<std::shared_ptr<GameKey>> points;

		/* GAMEPLAY */
		int score, keys, lives;
		bool gameWon, bool_isPlaying_val;
		Uint32 uint32_timeLimit_val;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif