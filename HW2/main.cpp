#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include "tennis.h"

//Declare functions not related to the class
int convert_to_double(std::vector< std::vector< std::string > > player_database,std::vector<double>& x,std::vector<double>& y);
std::vector<double> LinearFit(std::vector<double> xData, std::vector<double> yData);

int main(int argv, char* argc[])
{
	std::string input_filename;
	std::string output_filename;
	std::ifstream reader;
	std::vector<std::string> input_vector;
	std::ofstream writer;

	input_filename = argc[1];
	output_filename = argc[2];

	#ifdef DEBUG
	std::cout<<"Readerizing file\n";
	#endif

	//Open input file for reading
	reader.open(argc[1]);

	//Read input file
	while (getline (reader, input_filename))
	{
		input_vector.push_back(input_filename);
	}

	reader.close();

	#ifdef DEBUG
	std::cout<<"I readerized it\n";
	#endif

	//Produce error if input file has no values
	if(input_vector.empty())
	{
		std::cout <<"Error: Could not open input file.";
		return 1;
	}

	//Ensures user enters the correct number of arguments
	if (argv !=2)
	{
		std::cout << "Please enter 3 arguments: Executable, input, and output.\n";
		return 1;
	}

	//Run input vector through tennis class to make database of players and win/loss data
	Tennis make_database;

	#ifdef DEBUG
	std::cout<<"Time to Esla code\n";
	#endif

	make_database.names_list(input_vector);

	#ifdef DEBUG
	std::cout<<"Esla's database got builded\n";
	#endif

	//Access the private player database (giant 3D vector with all names and scores stored) with the accessor function
	std::vector< std::vector< std::string > > player_database;
	player_database = make_database.get_player_database();

	//Variables related to output writing format
	unsigned int max_length = 0;
	unsigned int matches_won_length =0;
	unsigned int matches_lost_length = 0;
	unsigned int games_won_length = 0;
	unsigned int games_lost_length = 0;

	//Find the longest player name in the input file
	//Will account for this length when formatting output
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		unsigned int tmp_length = player_database[i][0].size() + player_database[i][1].size();
		max_length = std::max(max_length, tmp_length);
	}

	//Find the longest match wins number in the input file
	//Will account for this length when formatting output	
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		unsigned int tmp_length = player_database[i][2].size();
		matches_won_length = std::max(matches_won_length, tmp_length);
	}

	//Find the longest match losses number in the input file
	//Will account for this length when formatting output
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		unsigned int tmp_length = player_database[i][3].size();
		matches_lost_length = std::max(matches_lost_length, tmp_length);
	}

	//Find the longest games won number in the input file
	//Will account for this length when formatting output
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		unsigned int tmp_length = player_database[i][5].size();
		games_won_length = std::max(games_won_length, tmp_length);
	}

	//Find the longest games lost number in the input file
	//Will account for this length when formatting output
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		unsigned int tmp_length = player_database[i][6].size();
		games_lost_length = std::max(games_lost_length, tmp_length);
	}

	//Add 2 to all the lengths. This will make the output look more spaced out and prettier
	max_length +=2;
	matches_won_length +=2;
	matches_lost_length +=2;
	games_won_length +=2;
	games_lost_length +=2;

	//Open output file for writing
	writer.open(output_filename.c_str(), std::ios::trunc|std::fstream::out);

	//Sort the player database vector by percent match wins before we output the first table
	//Run sort again to subsort by number of matches won
	std::sort(player_database.begin(), player_database.end(), compare_percent_match);

	//Make header lines for the match statistics table
	//Account for the maximum lengths of strings that will be in each column
	writer << "MATCH STATISTICS\n";
	std::string header =  std::string("Player") + std::string(max_length-6, ' ') + "   W" + std::string(matches_won_length-2, ' ') + "   L " + std::string(matches_lost_length-2, ' ') + " percentage";
	writer << header << "\n";

	//Write the match components of the player database vector to the output file
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		//Total length of player name
		unsigned int length = player_database[i][0].size() + player_database[i][1].size() +2;
		//Length of matches won string
		unsigned int length2 = player_database[i][2].size() +2;
		//Length of matches lost string
		unsigned int length3 = player_database[i][3].size() +2;

		writer << player_database[i][0] << " ";
		writer << player_database[i][1] << "  ";
		writer << std::string(max_length - length, ' ') << "  ";
		writer << player_database[i][2] << "  ";
		writer << std::string(matches_won_length - length2, ' ') << "  ";
		writer << player_database[i][3] << " ";
		writer << std::string(matches_lost_length - length3, ' ') << "       ";
		//Set number of decimal points on the percent
		writer << std::setprecision(3) << std::fixed << atof(player_database[i][4].c_str());
		writer << "\n";
	}

	//Sort the player database vector by percent game wins before outputting second table
	//Run sort again to subsort by number of games won
	std::sort(player_database.begin(), player_database.end(), compare_percent_games);

	//Make header lines for that game statistics table
	//Account for the maximum length of strings that will be in each column
	writer << "\n";
	writer << "GAME STATISTICS\n";
	std::string header2 = std::string("Player") + std::string(max_length-6, ' ') + "    W" + std::string(games_won_length-2, ' ') + "   L " + std::string(games_lost_length-2, ' ') + " percentage";
	writer << header2 << "\n";

	//Write the game components of the player database vector to the output file
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		//Total length of player name
		unsigned int length = player_database[i][0].size() + player_database[i][1].size() +2;
		//Length of games won string
		unsigned int length2 = player_database[i][5].size() + 2;
		//Length of games lost string
		unsigned int length3 = player_database[i][6].size()+2;

		writer << player_database[i][0] << " ";
		writer << player_database[i][1] << "  ";
		writer << std::string(max_length - length, ' ') << "  ";
		writer << player_database[i][5] << "  ";
		writer << std::string(games_won_length - length2, ' ') << "  ";
		writer << player_database[i][6] << "  ";
		writer << std::string(games_lost_length - length3, ' ') << "       ";
		//Set number of decimal points on the percent
		writer << std::setprecision(3) << std::fixed << atof(player_database[i][7].c_str());
		writer << "\n";		
	}

	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> Linear_Equation;

	//Put player database and 2 empty vectors into convert to double function
	convert_to_double(player_database,x,y);

	//Since x and y were passed by reference, put these modified vectors into the linearfit function
	Linear_Equation = LinearFit(x,y);

	//Output my personal statistics 
	writer << "\n";
	writer << "CORRELATION BETWEEN MATCHES WON AND GAMES WON\n";
	writer << "The line correlating number of matches won and number of games won is:\n";
	writer <<  "y = " << Linear_Equation[0] << "*x + " << Linear_Equation[1];
	writer << "\n";
	writer << "Where matches won is the x variable and games won is the y variable\n";

	writer.close();


	return 0;
}

//Function that converts number of matches won and number of games won to floats and puts them into vectors
int convert_to_double(std::vector< std::vector< std::string > > player_database,std::vector<double>& x,std::vector<double>& y)
{

	//For each row in player database
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		//Convert number of games and matches to floats
		double numb_match = atof(player_database[i][2].c_str());
		double numb_games = atof(player_database[i][5].c_str());

		//Put these floats into two vectors 
		x.push_back(numb_match);
		y.push_back(numb_games);
	}

return 0;
}

//Function that performs linear regression on the number of matches won and number of games won (see README.txt)
std::vector<double> LinearFit(std::vector<double> xData, std::vector<double> yData)
{
	double xSum = 0, ySum = 0, xxSum = 0, xySum = 0, slope, intercept;

	//For each point in the games vector (same length as matches vector)
	for (unsigned long i = 0; i < yData.size(); i++)
    {
    	//Sum the x data and y data
        xSum += xData[i];
        ySum += yData[i];
        //Sum of the squares for x and y data
        xxSum += xData[i] * xData[i];
        xySum += xData[i] * yData[i];
    }

    //Calculate slopes and intercepts using standard linear regression method
    slope = (yData.size() * xySum - xSum * ySum) / (yData.size() * xxSum - xSum * xSum);
    intercept = (ySum - slope * xSum) / yData.size();

    //Put the slope and intercept into the xy_equation vector
	std::vector<double> xy_equation;
    xy_equation.push_back(slope);
    xy_equation.push_back(intercept);

return xy_equation;
}