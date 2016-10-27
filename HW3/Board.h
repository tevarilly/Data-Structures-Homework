//Board.h
//Class that intakes a board width and a piece location
//Creates a dynamically allocated array of specified width
//Places the game piece (a Block class object) into the array at the specified location

#ifndef __Board_h_
#define __Board_h_

#include <iostream>
#include <string>
#include "Block.h"

class Board
{
public:
	//Default constructor
	Board();
	Board(int width/*, int loc, char shape, int rot*/);
	void add_block(int loc, char shape, int rot);
	void set_width(int w);
private:
	int max_height;
	int max_width;
	char** game_board;
	char** shape_array;
	int shape_height(Block b);
	int shape_width(Block b);
	void make_shape();
};

#endif