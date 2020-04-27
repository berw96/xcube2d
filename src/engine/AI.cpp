//1780 2815
#include "AI.h"

AI::AI() {
	init_randomNumber_val(0, 100);
}

AI::~AI() {}


void AI::void_randomMovement_val(int int_randomNumber_val) {

	if (int_randomNumber_val > 0 &&
		int_randomNumber_val <= 25) {
		//Move NORTH
		std::cout << "Move NORTH. ";
	}

	if (int_randomNumber_val > 25 &&
		int_randomNumber_val <= 50) {
		//Move SOUTH
		std::cout << "Move SOUTH. ";
	}

	if (int_randomNumber_val > 50 &&
		int_randomNumber_val <= 75) {
		//Move EAST
		std::cout << "Move EAST. ";
	}

	if (int_randomNumber_val > 75 &&
		int_randomNumber_val < 100) {
		//Move WEST
		std::cout << "Move WEST. ";
	}
}

int AI::get_randomNumber_val() {
	return int_randomNumber_val;
}

void AI::init_randomNumber_val(int int_minimum_val, int int_maximum_val) {
	int_randomNumber_val = { getRandom(int_minimum_val, int_maximum_val) };
}

//1780 2815
