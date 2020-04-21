#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"
#include "TestMazeGenerator.h"

struct GameKey {
	Point2 pos;
	bool alive;
};

class TestGame : public AbstractGame {
	private:
		Rect box;
		Rect light;

		Vector2i velocity;

		MazeGenerator * gen;
		Timer * timer;

		std::vector<std::shared_ptr<Line2i>> lines;
		std::vector<std::shared_ptr<GameKey>> points;

		/* GAMEPLAY */
		int score, keys, lives, random_number;
		bool gameWon, is_Playing;
		Uint32 time_limit;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
		TestGame();
		~TestGame();
};

#endif