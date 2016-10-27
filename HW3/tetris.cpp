//Tetris.cpp
//The tetris class takes user input, retrieves a rotated block template from the block class,
//	and adds the piece to the game board at the desired location
//Once the piece is added, the tetris class may also perform operations (found in the member functions) on the game board
//Some of these operations include adding and deleting columns and removing full rows

#include <iostream>
#include <string>
#include <cassert>
#include <stdio.h>
#include "tetris.h"

//Default constructor
Tetris::Tetris(int w)
{
	width = w;

	//Allocate space for the game board of width 'width'
	data = new char*[width];

	//Allocate space for an array of heights corresponding to width of game board
	heights = new int[width];

	//Initialize each value in heights to be 0: will be modified as we go
	for(int i=0;i<width;i++)
	{
		heights[i]=0;
	}

	//Initialize max height to be zero: will be modified when find_max_height function is called
	max_height=0;
}

//Add a shape to the game board (data)
void Tetris::add_piece(char shape, int rot, int loc)
{
	//Make a shape in the block class using given parameters (shape and rotation)
	Block new_block(shape,rot);

	//Find the maximum width and height of the shape
	int shape_width = new_block.get_shape_width();
	int shape_height = new_block.get_shape_height();

	//Maximum height of column in range of interest after shape will be added
	int new_height = get_biggest(loc,loc+(shape_width-1))+shape_height;

	//Maximum height of column in range of interest before shape will be added
	int old_height = get_biggest(loc, loc+(shape_width-1));

	//Buffer will temporarily hold the characters in the columns we want to add to
	char* buffer = new char[new_height];

	//Initialize buffer with empty spaces
	for(int i=0; i<new_height; i++)
	{
		buffer[i] = ' ';
	}

	//Allocate space in data for the new shape
	//Look in range the width of the shape covers
	for(int i=loc;i<loc+shape_width;i++)
	{
		//For each column in the range, copy the column to the buffer
		for(int j=0; j<heights[i]; j++)
		{
			buffer[j]=data[i][j];
		}

		//If there are characters in the column
		if(heights[i]>0)
		{
			//Temporarily delete the characters in that column
			delete [] data[i];
		}

		//Re-size the column to the new height
		data[i] = new char[new_height];

		//Update the heights array
		heights[i]=new_height;

		//Put the values from the buffer back into the data column
		for(int j=0; j<heights[i]; j++)
		{
			data[i][j]=buffer[j];
		}

		//Re-initialize all of buffer to be empty spaces
		for(int i=0; i<new_height; i++)
		{
			buffer[i] = ' ';
		}
	}

	//We're done with the buffer
	delete [] buffer;

	//Buffer for the shape array to be added into
	char** shape_array = new char*[4];
	for(int i=0; i<4; i++)
	{
		shape_array[i] = new char[4];
	}

	//Get the array which holds the shape from the block class
	//Put it in your buffer
	new_block.get_shape(shape_array);

	//Writes the characters from shape array into the specified location in data 
	//i and j are coordinates in shape array
	//l and h are coordinates in data
	//Initialize l to be location inputted by user
	for(int i=0,l=loc,h=old_height;i<shape_width;i++)
	{
		for(int j=shape_height-1;j>=0;j--)
		{
			data[l][h]=shape_array[j][i];
			h++;
		}
		h=old_height;
		l++;
	}

	//Delete the allocated memory for the shape array because we're done with it
	for(int i=0; i<4; i++)
	{
		delete [] shape_array[i];
	}
	delete [] shape_array;

	//Call destroy on the block object we made
	//Deletes any dynamically allocated memory from block class now that we're done with it
	new_block.destroy();

	//Delete any rogue spaces from the top of the heights column whenever a new shape is added
	//This just changes the heights array value and makes the next shape "think" there is no space there
 	//For every column in data
	for(int i=0;i<width;i++)
	{
		bool de_hatted = false;

		//For every row in column, starting at the top row
		for(int j=heights[i]-1;j >= 0 && !de_hatted;j--)
		{
			//If you find a space
			if(data[i][j]==' ')
			{
				//Subtract 1 from the corresponding heights array
				heights[i]--;
			}

			//If you found something that wasn't a space
			else
			{
				//Then, congratulations, you have de-hatted the column
				de_hatted = true;
			}
		}
	}
}

//Return the largest column size in a given range
int Tetris::get_biggest(int begin, int end) const
{
	int big = 0;

	//For every value in heights array within given range
	for(int i=begin; i<=end; i++)
	{
		//If the value is greater than big
		if(heights[i]>big)
		{
			//Make big equal to the value
			big = heights[i];
		}
	}

return big;	
}

//Return the width: pretty self-explanatory
int Tetris::get_width() const
{
	return width;
}

//Returns the number of characters in the highest column in data
int Tetris::get_max_height() const
{
	int big = 0;

	//For every value in heights array
	for(int i=0;i<width;i++)
	{
		//If the value is greater than the current value of big
		if(heights[i]>big)
		{
			//Make this value equal to big
			big = heights[i];
		}
	}

return big;
}

//Return how many squares on the grid are filled with characters
int Tetris::count_squares()
{
	int count = 0;

	//For every row and column in data
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<heights[i]; j++)
		{
			//If you find a character that isn't a space
			if(data[i][j] != ' ')
			{
				//Add 1 to the count
				count++;
			}
		}
	}

return count;
}

//Function that deletes rows full of characters when called
int Tetris::remove_full_rows()
{
	int score = 0;
	int count = 0;

	//For every row value, beginning with the smallest height
	for(int j=get_min_height()-1; j>=0; j--)
	{
		count = 0;

		//For every column in data
		for(int i=0; i<width; i++)
		{
			//If you find a character that is not a space
			if(data[i][j] != ' ')
			{
				//Add one to count
				count++;
			}

			//If count equals the width, you have a full row of non-space characters
			//Therefore, carry out deletion of row j
			if(count == width)
			{
				//Delete the jth row in every column
				for(int k=0; k<width; k++)
				{
					//Define a buffer of length j (number of values below full row)
					char* buffer = new char[j];

					//Define another buffer (length = number of values above full row)
					char* buffer2 = new char[heights[k]-j-1];

					//Fill buffer with empty spaces
					for(int l=0; l<j; l++)
					{
						buffer[l] = ' ';
					}

					//Fill buffer2 with empty spaces
					for(int l=0; l<(heights[k]-j-1); l++)
					{
						buffer2[l] = ' ';
					}

					//For every row below full row
					for(int l=0; l<j; l++)
					{
						//Fill buffer with data values
						buffer[l] = data[k][l];
					}

					if(heights[k]>(j+1))
					{
						int buffer2_count = 0;
						//For every row above full row
						for(int l=j+1; l<heights[k]; l++)
						{
							//Fill buffer2 with data values
							buffer2[buffer2_count] = data[k][l];
							buffer2_count++;
						}
					}

					//Temporarily delete data values in the column
					delete [] data[k];

					//Re-allocate space for data column that is one smaller than previous
					// to account for removed row
					data[k] = new char[heights[k]-1];

					//Put values of first buffer back into data
					for(int l=0; l<j; l++)
					{
						data[k][l] = buffer[l];
					}

					if(heights[k]>(j+1))
					{
						int data_count = j;
						//Put values of second buffer back into data
						for(int l=0; l<(heights[k]-j-1); l++)
						{
							data[k][data_count] = buffer2[l];
							data_count++;
						}
					}

					//Delete buffers each time to avoid sizing issues
					delete [] buffer;
					delete [] buffer2;
				}

				//You found a full row! Add 1 to score
				score++;

				for(int k=0; k<width; k++)
				{
					heights[k]--;
				}
			}
		}
	}
	
	bool column_is_blank = false;

	//Get rid of spaces sitting on top of shapes (space hats)
	//Additionally, get rid of spaces in blank columns	

	//For every column in data
	for(int i=0;i<width;i++)
	{
		bool de_hatted = false;

		column_is_blank = true;

		//For every row in each column
		for(int j=0;j<heights[i];j++)
		{
			//If you find a character that isn't a space
			if(data[i][j]!=' ')
			{
				//Then the column is definitely not blank
				column_is_blank = false;
			}
		}

		//If the column_is_blank bool is still true after looking through the column
		if (column_is_blank && heights[i]>0)
		{
			//Make the corresponding heights value zero
			heights[i] = 0;
		}

		//For every row in column, starting at the top row
		for(int j=heights[i]-1;j >= 0 && !de_hatted;j--)
		{
			//If you find a space
			if(data[i][j]==' ')
			{
				//Subtract 1 from the corresponding heights array
				heights[i]--;
			}

			//If you found something that wasn't a space
			else
			{
				//Then, congratulations, you have de-hatted the column
				de_hatted = true;
			}
		}
	}
	
return score;
}

//Function that finds the smallest value in the heights array
int Tetris::get_min_height() const
{
	//Start with the largest height value
	int small = get_max_height();

	//For every value in the heights array
	for(int i=0; i<width; i++)
	{
		//If the value is less than small
		if(heights[i]<small)
		{
			//Make small equal to that value
			small = heights[i];
		}
	}

return small;
}

//Function that removes the right column of the game board
void Tetris::remove_right_column()
{
	char** buffer;

	//Create buffer to temporarily hold ALL of data except for last column
	buffer = new char*[width-1];

	//Allocate space for columns in buffer, ignoring last column
	for(int i=0; i<width-1; i++)
	{
		buffer[i] = new char[heights[i]];
	}

	//Copy all of the information from data (except the last column) into buffer
	for(int i=0; i<width-1; i++)
	{
		for(int j=0; j<heights[i]; j++)
		{
			buffer[i][j] = data[i][j];
		}
	}

	//Delete data
	for(int i=0; i<width; i++)
	{
		delete [] data[i];
	}

	delete [] data;

	//Redefine and reallocate data with new width
	data = new char*[width-1];

	for(int i=0; i<width-1; i++)
	{
		data[i] = new char[heights[i]];
	}

	//Put values in buffer back into data
	for(int i=0; i<width-1; i++)
	{
		for(int j=0; j<heights[i]; j++)
		{
			data[i][j] = buffer[i][j];
		}
	}

	//We're done with buffer, so delete it
	for(int i=0; i<width-1; i++)
	{
		delete [] buffer[i];
	}

	delete [] buffer;

	//Allocate char* to temporarily hold data from heights
	int* heightsbuffer;

	heightsbuffer = new int[width-1];

	//Put all of heights into heightsbuffer, excluding the last column
	for(int i=0; i<width-1; i++)
	{
		heightsbuffer[i] = heights[i];
	}

	//Delete heights array
	delete [] heights;

	//Re-allocate memory for heights array
	heights = new int[width-1];

	//Put values from heightsbuffer back into heights array
	for(int i=0; i<width-1; i++)
	{
		heights[i] = heightsbuffer[i];
	}

	delete [] heightsbuffer;

	width = width -1;
}

//Function that removes the left column of the game board
void Tetris::remove_left_column()
{
	char** buffer;

	//Create buffer to temporarily hold ALL of data except for first column
	buffer = new char*[width-1];

	//Allocate space for columns in buffer, ignoring the first column
	for(int i=0; i<width-1; i++)
	{
		buffer[i] = new char[heights[i+1]];
	}

	int buffercount = 0;

	//Copy all of the information from data (except the first column) into buffer
	for(int i=1; i<width; i++)
	{
		for(int j=0; j<heights[i]; j++)
		{
			buffer[buffercount][j] = data[i][j];
		}

		buffercount++;
	}

	//Delete data
	for(int i=0; i<width; i++)
	{
		delete [] data[i];
	}

	delete [] data;

	//Redefine and reallocate data with new width
	data = new char*[width-1];

	for(int i=0; i<width-1; i++)
	{
		data[i] = new char[heights[i+1]];
	}

	//Put values in buffer back into data
	for(int i=0; i<width-1; i++)
	{
		for(int j=0; j<heights[i+1]; j++)
		{
			data[i][j] = buffer[i][j];
		}
	}

	//We're done with buffer, so delete it
	for(int i=0; i<width-1; i++)
	{
		delete [] buffer[i];
	}

	delete [] buffer;

	//Allocate char* to temporarily hold data from heights
	int* heightsbuffer;

	heightsbuffer = new int[width-1];

	//Put all of heights into heightsbuffer, excluding the first column
	for(int i=0; i<width-1; i++)
	{
		heightsbuffer[i] = heights[i+1];
	}

	//Delete heights array
	delete [] heights;

	//Re-allocate memory for heights array
	heights = new int[width-1];

	//Put values from heightsbuffer back into heights array
	for(int i=0; i<width-1; i++)
	{
		heights[i] = heightsbuffer[i];
	}

	delete [] heightsbuffer;

	width = width -1;
}

//Function that adds a column to the right of the game board
void Tetris::add_right_column()
{
	//Allocate char* to temporarily hold data from heights
	int* heightsbuffer;

	heightsbuffer = new int[width+1];

	//Put all of heights into heightsbuffer, plus one space for new value
	for(int i=0; i<width; i++)
	{
		heightsbuffer[i] = heights[i];
	}

	//New right column value
	heightsbuffer[width] = 0;

	//Delete heights array
	delete [] heights;

	//Re-allocate memory for heights array
	heights = new int[width+1];

	//Put values from heightsbuffer back into heights array
	for(int i=0; i<width+1; i++)
	{
		heights[i] = heightsbuffer[i];
	}

	delete [] heightsbuffer;

	char** buffer;

	//Create buffer to temporarily hold ALL of data plus one more column
	buffer = new char*[width+1];

	//Allocate space for columns in buffer plus one more than width
	for(int i=0; i<width+1; i++)
	{
		//std::cout<<"allocate buffer["<<i<<"] "<<heights[i]<<" chars of data\n";
		buffer[i] = new char[heights[i]];
	}

	//Copy all of the information from data (except the first column) into buffer
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<heights[i]; j++)
		{
			buffer[i][j] = data[i][j];
		}
	}

	//Delete data
	for(int i=0; i<width; i++)
	{
		delete [] data[i];
	}

	delete [] data;


	//Redefine and reallocate data with new width
	data = new char*[width+1];

	for(int i=0; i<width+1; i++)
	{
		data[i] = new char[heights[i]];
	}
	

	//Put values in buffer back into data
	for(int i=0; i<width+1; i++)
	{
		for(int j=0; j<heights[i]; j++)
		{
			data[i][j] = buffer[i][j];
		}
	}

	//We're done with buffer, so delete it
	for(int i=0; i<width+1; i++)
	{
		delete [] buffer[i];
	}

	delete [] buffer;

	width = width +1;
}

//Function that adds a column to the left of the game board
void Tetris::add_left_column()
{
	//Allocate char* to temporarily hold data from heights
	int* heightsbuffer;

	heightsbuffer = new int[width+1];

	//Put all of heights into heightsbuffer, starting at index 1 for buffer
	for(int i=1; i<width+1; i++)
	{
		heightsbuffer[i] = heights[i-1];
	}

	//New left column value
	heightsbuffer[0] = 0;

	//Delete heights array
	delete [] heights;

	//Re-allocate memory for heights array
	heights = new int[width+1];

	//Put values from heightsbuffer back into heights array
	for(int i=0; i<width+1; i++)
	{
		heights[i] = heightsbuffer[i];
	}

	delete [] heightsbuffer;

	char** buffer;

	//Create buffer to temporarily hold ALL of data plus one more column
	buffer = new char*[width+1];

	//Allocate space for columns in buffer plus one more than width
	for(int i=0; i<width+1; i++)
	{
		//std::cout<<"allocate buffer["<<i<<"] "<<heights[i]<<" chars of data\n";
		buffer[i] = new char[heights[i]];
	}

	//Copy all of the information from data (except the first column) into buffer
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<heights[i+1]; j++)
		{
			buffer[i+1][j] = data[i][j];
		}
	}

	//Delete data
	for(int i=0; i<width; i++)
	{
		delete [] data[i];
	}

	delete [] data;

	//Redefine and reallocate data with new width
	data = new char*[width+1];

	for(int i=0; i<width+1; i++)
	{
		data[i] = new char[heights[i]];
	}

	//Put values in buffer back into data
	for(int i=0; i<width+1; i++)
	{
		for(int j=0; j<heights[i]; j++)
		{
			data[i][j] = buffer[i][j];
		}
	}

	//We're done with buffer, so delete it
	for(int i=0; i<width+1; i++)
	{
		delete [] buffer[i];
	}

	delete [] buffer;

	width = width +1;
}

//Delete any dynamically allocated memory
void Tetris::destroy()
{
	//Delete data array
	for(int i=0; i<width; i++)
	{
		delete [] data[i];
	}

	delete [] data;

	//Delete the heights array
	delete [] heights;
}