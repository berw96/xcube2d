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
	int int_randomMovement_val(int int_randomNumber_val, int speed);

	int get_randomNumber_val();
	void init_randomNumber_val(int int_minimum_val, int int_maximum_val);

	Point2 point2_getCurrentTile_val();
	void point2_getCurrentTile_val(Point2 point2_currentTile_val);

	Vector2i vector2i_getVelocity_val();

	std::vector<Point2> vectorpoint2_getForbiddenTiles_val();
	void vectorpoint2_setForbiddenTile_val(Point2 point2_forbiddenTile_val);

	std::vector<Dir> dir_getMemory_adr();
	void dir_updateMemory_adr(std::vector<Dir> dir_newDirections_adr);

private:
	int int_randomNumber_val;
	Point2 point2_currentTile_val;
	Vector2i vector2i_velocity_val;
	std::vector<Dir> dir_memory_adr; //memory
};



#endif
//1780 2815
