//1780 2815
#include "AI.h"

AI::AI() {
	init_randomNumber_val(0, 100);
}

AI::~AI() {}


int AI::int_randomMovement_val(int int_randomNumber_val, int speed) {
	if (int_randomNumber_val > 50 &&
		int_randomNumber_val <= 75) {
		//Move EAST
		std::cout << "Move EAST. ";
		return speed;
	}

	if (int_randomNumber_val > 75 &&
		int_randomNumber_val < 100) {
		//Move WEST
		std::cout << "Move WEST. ";
		return -speed;
	}

	if (int_randomNumber_val > 0 &&
		int_randomNumber_val <= 25) {
		//Move NORTH
		std::cout << "Move NORTH. ";
		return -speed;
	}

	if (int_randomNumber_val > 25 &&
		int_randomNumber_val <= 50) {
		//Move SOUTH
		std::cout << "Move SOUTH. ";
		return speed;
	}
}

int AI::get_randomNumber_val() {
	return int_randomNumber_val;
}

void AI::init_randomNumber_val(int int_minimum_val, int int_maximum_val) {
	int_randomNumber_val = { getRandom(int_minimum_val, int_maximum_val) };
}

Vector2i AI::vector2i_getVelocity_val() { return vector2i_velocity_val; }

std::vector<Dir> AI::dir_getMemory_adr() {
	return dir_memory_adr;
}

void AI::dir_updateMemory_adr(std::vector<Dir> dir_newDirections_adr){
	dir_memory_adr = dir_newDirections_adr; //sets the valid directions which the AI can go in for the current tile.
}

//1780 2815
