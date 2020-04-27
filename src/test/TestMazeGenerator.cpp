#include "TestMazeGenerator.h"

Dir::Dir() {} //prototype constructor

Dir::Dir(const Dir & other) : bit(other.bit), dx(other.dx), dy(other.dy),
opposite(other.opposite) {}

MazeGenerator::MazeGenerator(const int & x, const int & y) : x(x), y(y) {
	/*Binary bits are integers preceeded by the binary 'and' (&) operator.
	They are used in bitwise expressions
	
		|= is the bitwise version of +=
		& is the bitwise version of ==

	Behind the scenes, integers are represented as binary numbers, and bitwise
	operators allow access to these binary attributes.
	*/
	N.bit = 1; //0001
	N.dx = 0;
	N.dy = -1;
	N.opposite = &S;

	S.bit = 2; //0010
	S.dx = 0;
	S.dy = 1;
	S.opposite = &N;

	E.bit = 4; //0100
	E.dx = 1;
	E.dy = 0;
	E.opposite = &W;

	W.bit = 8; //1000
	W.dx = -1;
	W.dy = 0;
	W.opposite = &E;

	maze = new int*[x];

	for (int i = 0; i < x; ++i)
		maze[i] = new int[y];

	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			maze[i][j] = 0;	//creates a blank canvas for the maze walls to be drawn on.
		} 
	}
}//generates the dimensions for the maze using the x and y parameters passed to it from the caller in TestGame.cpp

MazeGenerator::~MazeGenerator() {
	for (int i = 0; i < x; ++i)
		delete[] maze[i];
	delete[] maze;
}

//COULD BE USEFUL FOR A.I. PATHFINDING
void MazeGenerator::generateMaze(int cx, int cy) {
	Dir * dirs = new Dir[4] {N, S, E, W};	//possible directions which a single tile allows.

	for (int i = 0; i < 4; ++i) {	//0, 1, 2 and 3 - north (N), south (S), east (E) and west (W).
		int index;
		do {
			index = getRandom(0, 3); //0, 1 or 2 - only three directions in dirs are allowed (N, S, E).
		} 
		while (index == i);
		/*if the random value of index equals the iterated value of 'i', run the loop body again.
		This ensures that the value of index is unique and inequal to the value of 'i'.*/

		Dir tmp = dirs[i];
		dirs[i] = dirs[index];
		dirs[index] = tmp;
		/*Once a unique value for index is assigned, create a temporary Direction array and store the
		direction at slot 'i' in the dirs array. Swap the directions over.*/
	}

	for (int i = 0; i < 4; ++i) { //0 to 3, the possible directions which any one tile can allow.
		Dir dir = dirs[i]; //determines the direction(s) in which the player can move when on the tile.
		int nx = cx + dir.dx;
		int ny = cy + dir.dy;
		if (between(nx, x) && between(ny, y) && (maze[nx][ny] == 0)) {
			maze[cx][cy] |= dir.bit; //same as += but in the context of binary
			maze[nx][ny] |= dir.opposite->bit;
			generateMaze(nx, ny); //recursive call
		}
	}
	//COULD BE USEFUL FOR A.I. PATHFINDING
}//Fills the maze with TILES which enable different directions of movement
