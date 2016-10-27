//Tetris.h
//The tetris class takes user input, retrieves a rotated block template from the block class,
//	and adds the piece to the game board at the desired location
//Once the piece is added, the tetris class may also perform operations (found in the member functions) on the game board
//Some of these operations include adding and deleting columns and removing full rows

#ifndef __tetris_h_
#define __tetris_h_

#include <iostream>
#include <string>
#include "Block.h"

class Tetris
{
public:
	//Default constructor
	Tetris(int w);

	void add_piece(char shape, int rot, int loc);
	void print () const;//done							
	int count_squares();//done
	int remove_full_rows();//done
	void destroy();//done
	void remove_left_column();//done
	void remove_right_column();//done
	void add_left_column();
	void add_right_column();//done

	//Accessors
	int get_max_height() const;//done
	int get_width() const;//done

private:
	int max_height;
	int* heights;
	char** data; //the 2d array of data for the game board
	int width;
	char shape;
	int orientation;
	int location;
	int get_biggest(int begin, int end);//done
	int get_min_height() const; //done
};

#endif