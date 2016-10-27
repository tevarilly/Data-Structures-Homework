/*
This is a program which takes an input file name, output file name, mode, and arguments specific to the mode chosen.
Depending on the mode chosen amongst replace, dilation, erosion, and floodfill, the program will modify the input file and produce a new output.
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

//Declaring functions to be used later on in order of appearance
void replace_function(std::vector<std::string> &input_vector, char target, char replacement);
void dilation_function(std::vector<std::string> &input_vector, char target);
void erosion_function(std::vector<std::string> &input_vector, char target, char background);
void floodfill_function(std::vector<std::string> &input_vector, int coord1_value, int coord2_value, char replacement, char body_character);
void writer(std::vector<std::string> &input_vector, std::string output_filename);

//The main function will analyze user input and run the desired commands
int main(int argv, char* argc[]) 
{
	//Ensures user enters the correct number of arguments
	if (argv < 5 || argv > 7)
	{
		std::cout << "Please enter 4-6 arguments: Executable, Input file name, Output file name, Mode, and specifications for replacement characters. \n";
		std::cout << "If you are using floodfill, enter the coordinates of the body before your replacement character. \n";
		return 1;
	}
	
	//Define some variables we will be using extensively later on
	std::string input_filename; 
	std::string output_filename;
	std::string mode;
	std::vector<std::string> input_vector;
	std::string coord1;
	std::string coord2;
	char target;
	char background;
	char replacement;


	//Assigning user input (command line arguments) as strings
	input_filename = argc[1];
	output_filename = argc[2];
	mode = argc[3];

	//Warns user if they enter more than the required number of arguments for replace mode
	if (!mode.compare("replace"))
	{
		if (argv > 6)
		{
			std::cout << "Please enter only 4 additional arguments for replace mode: input file, output file, target character, and replacement character.\n";
			return 1;
		}
	}

	//Warns user if they enter more than the required number of arguments for dilation mode
	if (!mode.compare("dilation"))
	{
		if (argv > 5)
		{
			std::cout << "Please enter only 3 additional arguments for dilation mode: input file, output file, and character to dilate.\n";
			return 1;
		}
	}

	//Warns user if they enter more than the required number of arguments for erosion mode
	if (!mode.compare("erosion"))
	{
		if (argv > 6)
		{
			std::cout << "Please enter only 4 additional arguments for erosion mode: input file, output file, target character to erode, and background character.\n";
			return 1;
		}
	}

	//Makes sure user only enters one character to find or replace if they are using replace mode
	if (!mode.compare("replace"))
	{	
		//Have to define replacement here because some modes have fewer required arguments
		replacement = argc[5][0];
		if (argc[4][1] != '\0' || argc[5][1] != '\0')
		{
			std::cout << "Please only enter one character to find or replace.\n";
			return 1;
		}
	}

	//Makes sure user only enters one character to find or replace if they are using dilation mode
	if(!mode.compare("dilation"))
	{
		if(argc[4][1] != '\0')
		{
			std::cout << "Please only enter one character you wish to dilate.\n";
			return 1;
		}
	}

	//Makes sure user only enters one character to find or replace if they are using erosion mode
	if(!mode.compare("erosion"))
	{
		if(argc[4][1] != '\0' || argc[5][1] != '\0')
		{
			std::cout << "Please only enter one character you wish to find or erode.\n";
			return 1;
		}
	}


	//Makes sure user only enters one character to fill with if they are using floodfill mode
	if(!mode.compare("floodfill"))
	{
		if(argc[6][1] != '\0')
		{
			std::cout << "Please only enter one character you wish to fill with.\n";
			return 1;
		}
	}

	//If user input does not match one of the acceptible mode commands, the program produces an error
	if (mode.compare("replace") && mode.compare("dilation") && mode.compare("erosion") && mode.compare("floodfill"))
	{
		std::cout << "Please enter a valid mode. Acceptable modes are replace, dilation, erosion, and floodfill\n";
		std::cout << mode << " is not a valid mode\n";
		return 1;
	}

	//Open the input file for reading
	std::ifstream reader;
	reader.open(argc[1]);

	//Putting the lines from the input file into a vector to be modified and outputted later
	while(getline (reader, input_filename)) 
	{
		//Print out input file before modifying it to check if we get the right answer					
		input_vector.push_back(input_filename);
	}

	//Close the reader
	reader.close();

	//Produces error if input file is empty
	if (input_vector.empty())	
	{
		std::cout << "Can not open input file.\n";
		return 1;
	}

	//If mode entered is replace, run replace function
	if (!mode.compare("replace"))
	{
		target = argc[4][0];
		replace_function(input_vector, target, replacement);
	}

	//If mode entered is dilation, run dilation function
	if (!mode.compare("dilation"))
	{
		target = argc[4][0];
		dilation_function(input_vector, target);
	}

	//If mode entered is erosion, run erosion function
	if (!mode.compare("erosion"))
	{
		target = argc[4][0];
		background = argc[5][0];
		erosion_function(input_vector, target, background);
	}

	//If mode entered is floodfill, run the floodfill function
	if (!mode.compare("floodfill"))
	{
		coord1 = argc[4];
		coord2 = argc[5];

		//Converting the user coordinate inputs from strings to integers
		std::istringstream buffer(coord1);
 		int coord1_value;
 		buffer >> coord1_value;
 		std::istringstream buffer2(coord2);
 		int coord2_value;
 		buffer2 >> coord2_value;
 		
		char body_character;

		//If the coordinates entered do not exist in the input file, produce an error
		if (coord1_value > (int)(input_vector.size()-1) || coord1_value < 0 || coord2_value <0 || coord2_value > (int)(input_vector[coord1_value].length()-1))
		{
			std::cout << "You are outside of the range of the array.\n";
			return 1;
		}

		replacement = argc[6][0];

		//Stores the character found at the original coordinates so we know what the body is made of 
		body_character = input_vector[coord1_value][coord2_value];

		floodfill_function(input_vector, coord1_value, coord2_value, replacement, body_character);
	}

	//Run the writer function using the modified input vector to the specified output file name
	writer(input_vector, output_filename);

	return 0;
}

//Function that replaces specified character (target) with another specified character (replacement)
void replace_function(std::vector<std::string> &input_vector, char target, char replacement)
{
	//Run through each line in the vector
	for(unsigned int i=0;i<input_vector.size();i++)
	{
		//Run through the characters in the line
		for(unsigned int j=0;j<input_vector[i].length();j++)
		{
			//If you find the target value		
			if(input_vector[i][j]==target)
			{
				//Replace with the new user specified character
				input_vector[i][j]=replacement;
			}
		}
	}
}

//Function that takes the character specified (target) and dilates bodies consisting of that character
void dilation_function(std::vector<std::string> &input_vector, char target)
{
	int i=0;
	int j=0;

	//Type casting the input vector size so it can be equated to i
	//Makes sure i does not go past the size of the input vector (boundary conditions)
	for(i=0;i<(int)(input_vector.size());i++)
	{
		//Makes sure j does not go past the length of the vector (boundary conditions)
		for(j=0;j<(int)(input_vector[i].length());j++)
		{
			//If you find the target character in the array
			if(input_vector[i][j] == target)
			{
				//If you haven't hit a top boundary
				if(i-1>=0)
				{
					//And the character above the target does not match the target, then you have found an edge character. Replace it with a 'd'
					if (input_vector[i-1][j] != target)
					{
						input_vector[i-1][j] = 'd';
					}
				}
				//If you haven't hit a bottom boundary
				if(i+1<(int)(input_vector.size()))
				{
					//And the character below the target does not match the target, then you have found an edge character. Replace it with a 'd'
					if(input_vector[i+1][j] != target)
					{
						input_vector[i+1][j] = 'd';
					}
				}
				//If you haven't hit a right boundary
				if(j+1<(int)(input_vector[i].length()))
				{
					//And the character to the right of the target does not match the target, then you have found an edge character. Replace it with a 'd'
					if(input_vector[i][j+1] != target)
					{
						input_vector[i][j+1] = 'd';
					}
				}
				//If you haven't hit a left boundary
				if(j-1>=0)
				{
					//And the character to the left of the target does not match the target, then you have found and edge character. Replace it with a 'd'
					if(input_vector[i][j-1] != target)
					{
						input_vector[i][j-1] = 'd';
					}
				}

			}
		}
	}
	//You now have the modified input vector with 'd's around the edge of target character bodies.
	//Run this through the replace function to replace the 'd's with the character specified by the user (target)
	replace_function(input_vector, 'd', target);
}

//Function that finds the points on the edge of an object made of specified characters (target) and replaces the edge points with background characters
void erosion_function(std::vector<std::string> &input_vector, char target, char background)
{
	int i = 0;
	int j = 0;

	//Type cast the input vector size so it can be equated to i
	//Makes sure i doesn't go past the size of the input vector (boundary conditions)
	for(i=0;i<(int)(input_vector.size());i++)
	{
		//Makes sure j doesn't go past the length of the input vector (boundary conditions)
		for(j=0;j<(int)(input_vector[i].length());j++)
		{
			//If you find the target character in the array
			if(input_vector[i][j] == target)
			{
				//If you haven't hit a top boundary
				if(i-1>=0)
				{
					//And the character above the target does not match the target
					//And if you have not already placed a 'd' above the character
					//Replace the current character with a 'd'
					if (input_vector[i-1][j] != target && input_vector[i-1][j] != 'd')
					{
						input_vector[i][j] = 'd';
					}
				}
				//If you haven't hit a bottom boundary
				if(i+1<(int)(input_vector.size()))
				{
					//And the character below the target does not match the target
					//And if you have not already placed a 'd' below the character
					//Replace the current character with a 'd'
					if(input_vector[i+1][j] != target && input_vector[i+1][j] != 'd')
					{
						input_vector[i][j] = 'd';
					}
				}
				//If you haven't hit a right boundary
				if(j+1<(int)(input_vector[i].length()))
				{
					//And the character to the right of the target does not match the target
					//And if you have not already placed a 'd' to the right of the character
					//Replace the current character with a 'd'
					if(input_vector[i][j+1] != target && input_vector[i][j+1] != 'd')
					{
						input_vector[i][j] = 'd';
					}
				}
				//If you haven't hit a left boundary
				if(j-1>=0)
				{
					//And the character to the left of the target does not match the target
					//And if you have not already placed a 'd' to the left of the character
					//Replace the current character with a 'd'
					if(input_vector[i][j-1] != target && input_vector[i][j-1] != 'd')
					{
						input_vector[i][j] = 'd';
					}
				}

			}
		}
	}

	//You now have a modified input vector with 'd's at the points by the edge of the body
	//Run this through the replace function to replace the 'd's with character specified by the user (likely the background character)
	replace_function(input_vector, 'd', background);
}

//Function that finds a body of characters at a specified coordinate and replaces those characters with another character specified by the user (replacement)
void floodfill_function(std::vector<std::string> &input_vector, int coord1_value, int coord2_value, char replacement, char body_character)
{
	//If you find a character that comprises the'body character' (starting at first input coordinates)
	if (input_vector[coord1_value][coord2_value] == body_character)
	{
		input_vector[coord1_value][coord2_value] = replacement;
		
		if (coord1_value + 1 < (int)(input_vector.size()))
		{
			floodfill_function(input_vector, coord1_value + 1, coord2_value, replacement, body_character);
		}

		if (coord1_value - 1 >= 0)
		{
			floodfill_function(input_vector, coord1_value - 1, coord2_value, replacement, body_character);
		}

		if (coord2_value + 1 < (int)(input_vector[coord1_value].size()))
		{
			floodfill_function(input_vector, coord1_value, coord2_value + 1, replacement, body_character);
		}

		if (coord2_value - 1 >= 0)
		{
			floodfill_function(input_vector, coord1_value, coord2_value - 1, replacement, body_character);
		}

	}
	else
	{
		return;
	}

}

//Function that writes out edited input file to specified output file
void writer(std::vector<std::string> &input_vector, std::string output_filename)
{
	std::ofstream writer;
	writer.open(output_filename.c_str(), std::ios::trunc|std::fstream::out);

	unsigned int i = 0;

	//Write every character from the modified input vector into another file called output_filename
	for (i = 0; i<input_vector.size(); i++)
	{
		writer<<input_vector[i]<<"\n";
	}

	writer.close();
}