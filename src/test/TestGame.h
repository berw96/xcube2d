#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"
#include "TestMazeGenerator.h"
#include "../engine/AI.h"

struct GameKey {
	Point2 pos;
	bool alive;
};

class TestGame : public AbstractGame { //TestGame is derived from AbstractGame via inheritance
	private:
		Rect box;
		Rect light;

		Vector2i velocity;

		MazeGenerator* gen;
		Timer* timer_idleCountdown_adr;

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