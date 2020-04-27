//1780 2815
#ifndef _AI_
#define _AI_
#include "../test/TestMazeGenerator.h"
#include "GameMath.h"
#include <vector>


class AI {
public:
	AI();
	~AI();
	void void_randomMovement_val(int int_randomNumber_val);

	int get_randomNumber_val();
	void init_randomNumber_val(int int_minimum_val, int int_maximum_val);

private:
	int int_randomNumber_val;
	Point2 point2_currentTile_val;
	std::vector<Point2> vectorpoint2_forbiddenTiles_val;
};



#endif
//1780 2815
