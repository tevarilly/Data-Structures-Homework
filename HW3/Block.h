//Block.h
//The block class takes in a shape type (character) and an orientation (integer)
//It can rotate the default shape type to the desired orientation and allows the user
//	to access the resuling 2 dimensional array in which the shape is stored
//This class also provides the maximum height and width of the shape, after rotation,
//	which makes it easier to place in the game board

#ifndef __Block_h_
#define __Block_h_

#include <iostream>
#include <string>
#include <utility>
#include "shapes.h"

class Block
{
public:
	//Default constructor
	Block(char new_shape, int degrees);

	//Accessors
	void get_shape(char** buffer);
	int get_shape_height();
	int get_shape_width();

	//Other public member functions
	void print_shape();
	void destroy();

private:
	char** shape;
	int height;
	int width;
	int set_shape(char new_shape);
	int rotate(int degrees);
	void shape_shift();
	void shape_height();
	void shape_width();
};

#endif