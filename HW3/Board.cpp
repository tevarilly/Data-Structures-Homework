#include <iostream>
#include <string>
#include "Board.h"

//Default constructor
Board::Board()
{
}

Board::Board(int width/*, int loc, char shape, int rot*/)
{
	//Allocate 'width' number of char*'s in dynamic memory
	//These will be pointers to each row on the game board
	game_board = new char*[width];

	//Get the maxiumum height and width of the game piece for use
	//shape_height();
	//shape_width();

	shape_array = new char*[4];
	for(int i=0; i<4; i++)
	{
		shape_array[i] = new char[4];
	}

}

void Board::set_width(int w)
{
	max_width=w;
}
void Board::add_block(int loc, char shape, int rot)
{
	Block block_to_add(shape,rot); //creates a block to add to the board

	block_to_add.destroy();
}

//Access the maximum height of the shape to be added to board (from Block class)
int Board::shape_height(Block b)
{
	return b.get_shape_height();
}

//Access the maximum width of the shape to be added to the board (from Block class)
int Board::shape_width(Block b)
{
	return b.get_shape_width();
}

//Make an array (for use in this class) of the game piece
//The height of the array is the max height of the ganme piece
//The width of the array is the max width of the game piece
/*
void make_array()
{
	Block testblock(shape,rot);

	testblock.get_shape(shape_array);

}
*/