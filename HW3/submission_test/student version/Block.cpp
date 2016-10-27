//Block.cpp
//The block class takes in a shape type (character) and an orientation (integer)
//It can rotate the default shape type to the desired orientation and allows the user
//	to access the resuling 2 dimensional array in which the shape is stored
//This class also provides the maximum height and width of the shape, after rotation,
//	which makes it easier to place in the game board

#include <iostream>
#include <string>
#include <utility>
#include "Block.h"

//Constructor
Block::Block(char new_shape, int degrees)
{
	//Allocate space in memory for a shape (stored as a 2D array)
	shape = new char*[4];
	for(int i=0;i<4;i++)
	{
		shape[i] = new char[4];
	}
	
	//Call functions to operate on shape
	set_shape(new_shape);
	rotate(degrees);
	shape_shift();
	shape_height();
	shape_width();
}

//Function that prints the shape
void Block::print_shape()
{
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
    	{
      		std::cout << shape[i][j] << " "; 
      	}
    	std::cout << "\n";
  	}
}

//Function that takes a valid shape character as an argument
//	and assigns shape[] to one of the defined global shape variables
//	that may be found in shapes.h
int Block::set_shape(char new_shape)
{
	//If shape entered is 'I' 
	if(new_shape=='I')
	{	
		//Assign rod shape from global variables
		for (unsigned int i=0; i<4; i++)
		{
			for (unsigned int j=0; j<4; j++)
			{
				shape[i][j] = ROD[i][j];
			}
		}
	}
	//If shape entered is 'O' 
	else if(new_shape=='O')
	{
		//Assign sqr shape from global variables
		for (unsigned int i=0; i<4; i++)
		{
			for (unsigned int j=0; j<4; j++)
			{
				shape[i][j] = SQR[i][j];
			}
		}
	}
	//If shape entered is 'T' 
	else if(new_shape=='T')
	{
		//Assign tee shape from global variables
		for (unsigned int i=0; i<4; i++)
		{
			for (unsigned int j=0; j<4; j++)
			{
				shape[i][j] = TEE[i][j];
			}
		}		
	}
	//If shape entered is 'Z' 
	else if(new_shape=='Z')
	{
		//Assign zee shape from global variables
		for (unsigned int i=0; i<4; i++)
		{
			for (unsigned int j=0; j<4; j++)
			{
				shape[i][j] = ZEE[i][j];
			}
		}
	}
	//If shape entered is 'S' 
	else if(new_shape=='S')
	{
		//Assign ess shape from global variables
		for (unsigned int i=0; i<4; i++)
		{
			for (unsigned int j=0; j<4; j++)
			{
				shape[i][j] = ESS[i][j];
			}
		}	
	}
	//If shape entered is 'L' 
	else if(new_shape=='L')
	{
		//Assign ell shape from global variables
		for (unsigned int i=0; i<4; i++)
		{
			for (unsigned int j=0; j<4; j++)
			{
				shape[i][j] = ELL[i][j];
			}
		}	
	}
	//If shape entered is 'J' 
	else if(new_shape=='J')
	{
		//Assign jay shape from global variables
		for (unsigned int i=0; i<4; i++)
		{
			for (unsigned int j=0; j<4; j++)
			{
				shape[i][j] = JAY[i][j];
			}
		}
	}
	else
	{
		return 1;
	}
return 0;
}

//Function that rotates shape by the desired number of degrees
int Block::rotate(int degrees)
{
	//Make temporary buffers for storing rotation
	char buffer1[4][4];
	char buffer2[4][4];
	char buffer3[4][4];

	//If you want to rotate by 90, 180, or 270 degrees
	if(degrees == 90 || degrees == 180 || degrees == 270)
	{
		//Rotate the shape array once (by 90 degrees) for all 3 cases
		for (int i=0; i<4; i++)
		{
			for (int j=0, k=3; j<4; j++, k--)
			{
				buffer1[i][j] = shape[k][i];
			}
		}

		//If you want to rotate by 180 or 270 degrees
		if(degrees == 180 || degrees == 270)
		{
			//Rotate the shape array once more (by 90 degrees)
			for (int i=0; i<4; i++)
			{
				for (int j=0, k=3; j<4; j++, k--)
				{
					buffer2[i][j] = buffer1[k][i];
				}
			}

			//If you want to rotate by 270 degrees
			if(degrees == 270)
			{
				//Rotate the shape array a third time (by 90 degrees)
				for (int i=0; i<4; i++)
				{
					for (int j=0, k=3; j<4; j++, k--)
					{
						buffer3[i][j] = buffer2[k][i];
					}
				}
			}
		}

		//If degrees entered was 90, put the 90 degree rotated buffer into shape
		if(degrees == 90)
		{
			for (int i=0; i<4; i++)
			{
				for (int j=0; j<4; j++)
				{
					shape[i][j] = buffer1[i][j];
				}
			}
		}

		//If degrees entered was 180, put the 180 degree rotated buffer into shape
		if(degrees == 180)
		{
			for (int i=0; i<4; i++)
			{
				for (int j=0; j<4; j++)
				{
					shape[i][j] = buffer2[i][j];
				}
			}
		}

		//If degrees entered was 270, put the 270 degree rotated buffer into shape
		if(degrees == 270)
		{
			for (int i=0; i<4; i++)
			{
				for (int j=0; j<4; j++)
				{
					shape[i][j] = buffer3[i][j];
				}
			}
		}

	return 0;
	}

	else
	{
	return 1;
	}
}

//Function that removes empty rows above shape (effectively shifting it to upper left corner of the shape array)
//Any empty rows above the shape result from the rotation function
void Block::shape_shift()
{
	//Buffer temporarily stores non-empty rows from the shape array
	//No particular reason for calling it buffer4. Used buffers 1, 2, and 3 previously
	char buffer4[4][4];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			//Fill buffer with spaces for now
			buffer4[i][j]=' ';
		}
	}

	//For each location in shape array
	for(int i=0,b=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			//If you find a character that isn't a space
			if(shape[i][j] != ' ')
			{
				//Go through row i (in which you found a character)
				for(int k=0; k<4; k++)
				{
					//And put the values from row i into the buffer
					buffer4[b][k] = shape[i][k];
				}
				//The b counter ensures you add the values in row i to the topmost row in buffer
				b++;
				break;
			}
		}
	}

	//Put contents of buffer into dynamically allocated memory
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			shape[i][j] = buffer4[i][j];
		}
	}
}

//Function that counts how many "boxes" high the shape is => will use this to determine grid height later
void Block::shape_height()
{
	height = 0;
	bool done = false;

	//For every row in shape, starting at bottom
	for(int i=3; i>=0 && done == false; i--)
	{
		//For every character in the row
		for(int j=0; j<4 && done == false; j++)
		{
			//The first time you encounter a character that isn't a space
			if(shape[i][j] != ' ')
			{
				//Calculate how many rows high the shape is
				height = 4-(4-(i+1));

				//And then stop looping
				done = true;
			}
		}
	}
}

void Block::shape_width()
{
	width = 0;
	bool done = false;

	//For every column in shape, starting at end
	for(int j=3; j>=0 && done == false; j--)
	{
		//For every row in the column, starting at bottom
		for(int i=3; i>=0 && done == false; i--)
		{
			//The first time you encounter a character that isn't a space
			if(shape[i][j] != ' ')
			{
				//Calculate how wide the shape is
				width = 4-(4-(j+1));

				//And then stop looping
				done = true;
			}
		}
	}
}

//Allows the private shape array to be accessed
//Need to use a buffer for this because dynamic memory is weird
//Basically passes the buffer pointer by reference so you don't need toreturn anything
void Block::get_shape(char** buffer)
{
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
    	{
      		buffer[i][j]=shape[i][j];
    	}
  	}
}

//Allows user to get the maximum height of the shape
int Block::get_shape_height()
{
	shape_height();
	return height;
}

//Allows user to get the maximum width of the shape
int Block::get_shape_width()
{
	shape_width();
	return width;
}

//Deletes dynamically allocated memory to avoid leaks
void Block::destroy()
{
	for(int i=0; i<4; i++)
	{
		delete [] shape[i];
	}

	delete [] shape;
}