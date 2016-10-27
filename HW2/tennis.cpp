#include <iostream>
#include <iomanip>
#include <string>
#include <locale>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "tennis.h"

//Default constructor
Tennis::Tennis()
{	
}

//Outputs vector of winners, corresponding vectors of how many matches they won, vector of losers, and corresponding vector of how many matches they lost
int Tennis::names_list(std::vector<std::string> input_vector)
{

	//Run through input vector
	for(unsigned int i = 0; i < input_vector.size(); i++)
	{
		//Essentially black magic which allots a space in memory for each line in the input vector (converting from c string to c++)
		char* LineIn = new char [input_vector[i].length()+1];
		std::strcpy (LineIn, input_vector[i].c_str());

		char* temp;
		std::string first_name = "";
		std::string last_name = "";

		//Find first string before space delimeter (First name of player 1)
		temp = strtok(LineIn, " ");

		first_name = temp;

		std::vector<std::string> row_temp;

		//Add first name of player 1 to temporary vector
		row_temp.push_back(first_name);

		//Find next string (Last name of player 1)
		temp = strtok(NULL, " ");

		last_name = temp;

		//Add last name of player 1 to temporary vector
		row_temp.push_back(last_name);

		bool found = false;

		//Look through existing database and see if the winning player is already there
		for(unsigned int j=0; j < player_database.size(); j++)
		{
			//If we already have the player in the database, add their wins only
			if(player_database[j][0] == first_name && player_database[j][1] == last_name)
			{
				#ifdef DEBUG
				std::cout<<first_name<<" "<<last_name<<" was found in the database, updating entry...\n";
				#endif

				//Convert wins of player to integer
				std::istringstream buffer(player_database[j][2]);
				int wins;
				buffer >> wins;

				wins++;

				//Convert wins back to a string to put it back in the database vector
				std::string wincount;
				std::ostringstream convert;
				convert << wins;
				wincount = convert.str();

				player_database[j][2] = wincount;

				//Set the bool found to true so we don't accidentally re-add the player name to the database
				found = true;
				break;
			}

		}

		//If you didn't find player 1 in the database, add new row to player_database with player name and game won
		if(!found)
		{
			#ifdef DEBUG
			std::cout<<first_name<<" "<<last_name<<" was not in database, adding new entry...\n";
			#endif

			row_temp.push_back("1");

			row_temp.push_back("0");

			player_database.push_back(row_temp);
		}	

		//Have to clear row_temp for re-use
		row_temp.clear();

		//Find the string d.
		temp = strtok(NULL, " ");

		//Find the next string (First name of player 2)
		temp = strtok(NULL, " ");

		first_name = temp;

		row_temp.push_back(first_name);

		//Find the next string (Last name of player 2)
		temp = strtok(NULL, " ");

		last_name = temp;

		#ifdef DEBUG
		std::cout << "Checking if loser " << first_name << " " << last_name << " is in database\n";
		#endif
		found=false;
		//See if player 2 is already in the database
		for(unsigned int j=0; j < player_database.size(); j++)
		{
			//If the player is already in the database, add one to their losses only
			if(player_database[j][0] == first_name && player_database[j][1] == last_name)
			{
				#ifdef DEBUG
				std::cout<<first_name<<" "<<last_name<<" was found in the database, updating entry...\n";
				#endif

				//Convert number of losses to integer
				std::istringstream buffer(player_database[j][3]);
				int losses;
				buffer >> losses;

				losses++;

				//Convert number of losses back to a string to put back in the vector
				std::string losscount;
				std::ostringstream convert;
				convert << losses;
				losscount = convert.str();

				player_database[j][3] = losscount;

				//Set the bool found to true so we don't accidentally re-add the player name to the database
				found = true;
			}

		}

		//If we didn't find the player name in the database, add new row to player database with player name and game lost
		if(!found)
		{
			#ifdef DEBUG
			std::cout<<first_name<<" "<<last_name<<" was not in the database, adding new entry...\n";
			#endif

			row_temp.push_back(last_name);

			row_temp.push_back("0");

			row_temp.push_back("1");

			player_database.push_back(row_temp);
		}

	}

	#ifdef DEBUG
	std::cout<<"Calculating match statistics...\n";
	#endif

	match_stats(player_database, input_vector);

	#ifdef DEBUG
	std::cout<<"Done!\n";
	#endif

	return 0;
}

//Function that finds the percent matches won and adds this to the player database
int Tennis::match_stats(std::vector< std::vector< std::string > > &player_database, std::vector<std::string> input_vector)
{
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		float percent_matches;

		//Convert number of wins to an integer
		std::istringstream buffer(player_database[i][2]);
		int wins;
		buffer >> wins;

		//Convert number of losses to an integer
		std::istringstream buffer2(player_database[i][3]);
		int losses;
		buffer2 >> losses;

		//Type cast so we get decimal answer
		percent_matches = (float)(wins) / (float)((wins + losses));

		//Convert number of wins back to string
		std::string wincount;
		std::ostringstream convert;
		convert << wins;
		wincount = convert.str();
		player_database[i][2] = wincount;

		//Convert number of losses back to string
		std::string losscount;
		std::ostringstream convert2;
		convert2 << losses;
		losscount = convert2.str();
		player_database[i][3] = losscount;

		//Convert percent to a string
		std::string percent;
		std::ostringstream convert3;
		//Only use 3 digits of precision
		convert3.precision(3);
		convert3 << percent_matches;
		percent = convert3.str();

		player_database[i].push_back(percent);
	}

	#ifdef DEBUG
	std::cout<<"Calculating game statistics...\n";
	#endif

	game_database(player_database, input_vector);

	return 0;
}

//Function that adds number of games won and lost to player database
int Tennis::game_database(std::vector< std::vector< std::string > > &player_database, std::vector<std::string> input_vector)
{
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		//Create blank space for game points won in database (babyyy and I'll write your name)
		player_database[i].push_back("0");
		//Create blank space for game points lost in database
		player_database[i].push_back("0");
	}

	for(unsigned int i = 0; i < input_vector.size(); i++)
	{
		//Essentially black magic which allots a space in memory for each line in the input vector (converting from c string to c++)
		char* LineIn = new char [input_vector[i].length()+1];
		std::strcpy (LineIn, input_vector[i].c_str());

		char* temp;
		std::string first_name1 = "";
		std::string last_name1 = "";
		std::string first_name2 = "";
		std::string last_name2 = "";
		std::string score1 = "";
		std::string score2 = "";
		std::vector<std::string> test1;

		//Find first string before space delimeter (First name of player 1)
		temp = strtok(LineIn, " ");

		first_name1 = temp;

		//Find next string (Last name of player 1)
		temp = strtok(NULL, " ");

		last_name1 = temp;

		//Find the string d.
		temp = strtok(NULL, " ");

		//Find the next string (First name of player 2)
		temp = strtok(NULL, " ");

		first_name2 = temp;

		#ifdef DEBUG
		std::cout<<"Game: "<<first_name1<<" "<<last_name1<<" vs. "<<first_name2<<" "<<last_name2<<"\n";
		std::cout<<input_vector[i]<<"\n";
		#endif

		//Find the next string (Last name of player 2)
		temp = strtok(NULL, " ");

		#ifdef DEBUG
		std::cout<<"tokenize successful!\n";
		#endif

		last_name2 = temp;

		#ifdef DEBUG
		std::cout<<"name set successful!\n";
		#endif

		//This large while loop goes through every line in input vector (until end of line) and counts player game points
		while(temp != NULL)
		{
			//Find first game score before "-" delimeter
			temp = strtok(NULL, " -");

			//Because nobody wants a seg fault
			if(temp==NULL)
			{
				break;
			}

			score1 = temp;

			#ifdef DEBUG
			std::cout<<"score1="<<score1<<"\n";
			#endif

			//Add the first score to the wins of the first player and losses of second player
			for(unsigned int j=0; j<player_database.size(); j++)
			{
				//Find the first match player in the database
				if(player_database[j][0] == first_name1 && player_database[j][1] == last_name1)
				{
					//Convert the number of games they won so far to an integer
					std::istringstream buffer(player_database[j][5]);
					int games_won;
					buffer >> games_won;

					//Convert the score you just found with strtok to an integer
					std::istringstream buffer2(score1);
					int add_win_score;
					buffer2 >> add_win_score;

					int new_score;

					#ifdef DEBUG
					std::cout<<first_name1<<" "<<last_name1<<" has score "<<games_won<<". Adding "<<add_win_score<<"\n";
					#endif

					//Add game score found running total of games won 
					new_score = games_won + add_win_score;

					//Convert this back to a string and put it back in the vector
					std::string gamewincount;
					std::ostringstream convert;
					convert << new_score;
					gamewincount = convert.str();
					player_database[j][5] = gamewincount;
				}

				//Find the second match player in the database
				if(player_database[j][0] == first_name2 && player_database[j][1] == last_name2)
				{
					//Convert number of games they've lost so far to an integer
					std::istringstream buffer(player_database[j][6]);
					int games_lost;
					buffer >> games_lost;

					//Convert the score we just found with strtok to an integer
					std::istringstream buffer2(score1);
					int add_lose_score;
					buffer2 >> add_lose_score;

					int new_score;

					#ifdef DEBUG
					std::cout<<first_name2<<" "<<last_name2<<" has score "<<games_lost<<". Adding "<<add_lose_score<<"\n";
					#endif
					
					//Add score found to running total of games lost
					new_score = games_lost + add_lose_score;

					//Convert this back to a string and put it back in the vector
					std::string gamelostcount;
					std::ostringstream convert;
					convert << new_score;
					gamelostcount = convert.str();
					player_database[j][6] = gamelostcount;
				}
			}

			//Get the next game score
			temp = strtok(NULL, " -");

			//Once again, avoid seg faults
			if(temp==NULL)
			{
				break;
			}

			score2 = temp;

			//Add the second score you find to the wins of the second player listed and the losses of the first player (Same exact method as above)
			for(unsigned int j=0; j<player_database.size(); j++)
			{
				//Find the second match player in the database
				if(player_database[j][0] == first_name2 && player_database[j][1] == last_name2)
				{
					//Convert the number of games they won so far to an integer
					std::istringstream buffer(player_database[j][5]);
					int games_won;
					buffer >> games_won;

					//Convert the score you just found with strtok to an integer
					std::istringstream buffer2(score2);
					int add_win_score;
					buffer2 >> add_win_score;

					int new_score;

					#ifdef DEBUG
					std::cout<<first_name1<<" "<<last_name1<<" has score "<<games_won<<". Adding "<<add_win_score<<"\n";
					#endif

					//Add game score found running total of games won 
					new_score = games_won + add_win_score;

					//Convert this back to a string and put it back in the vector
					std::string gamewincount;
					std::ostringstream convert;
					convert << new_score;
					gamewincount = convert.str();
					player_database[j][5] = gamewincount;
				}

				//Find the first match player in the database
				if(player_database[j][0] == first_name1 && player_database[j][1] == last_name1)
				{
					//Convert the number of games lost so far to an integer
					std::istringstream buffer(player_database[j][6]);
					int games_lost;
					buffer >> games_lost;

					//Convert the score you juts found with strtok to an integer
					std::istringstream buffer2(score2);
					int add_lose_score;
					buffer2 >> add_lose_score;

					int new_score;

					#ifdef DEBUG
					std::cout<<first_name2<<" "<<last_name2<<" has score "<<games_lost<<". Adding "<<add_lose_score<<"\n";
					#endif
					
					//Add game score to running total of games lost
					new_score = games_lost + add_lose_score;

					//Convert this back to a string and put it back in the vector
					std::string gamelostcount;
					std::ostringstream convert;
					convert << new_score;
					gamelostcount = convert.str();
					player_database[j][6] = gamelostcount;
				}
			}
		}
	}

	//Run the game_stats function on your now modified vector
	game_stats(player_database);

return 0;
}

//Function that adds the percent games won and lost to the player database
int Tennis::game_stats(std::vector< std::vector< std::string > > &player_database)
{
	for(unsigned int i=0; i<player_database.size(); i++)
	{
		float percent_games;

		//Convert number of wins to an integer
		std::istringstream buffer(player_database[i][5]);
		int wins;
		buffer >> wins;

		//Convert number of losses to an integer
		std::istringstream buffer2(player_database[i][6]);
		int losses;
		buffer2 >> losses;

		//Type cast so we get decimal answer
		percent_games = (float)(wins) / (float)((wins + losses));

		//Convert number of wins back to string
		std::string wincount;
		std::ostringstream convert;
		convert << wins;
		wincount = convert.str();
		player_database[i][5] = wincount;

		//Convert number of losses back to string
		std::string losscount;
		std::ostringstream convert2;
		convert2 << losses;
		losscount = convert2.str();
		player_database[i][6] = losscount;

		//Convert percent to a string
		std::string percent;
		std::ostringstream convert3;
		//Only use 3 digits of precision
		convert3.precision(3);
		convert3 << percent_games;
		percent = convert3.str();

		player_database[i].push_back(percent);
	}

return 0;
}

//Get the player database for use
std::vector< std::vector< std::string > > Tennis::get_player_database()
{
return player_database;
}

//Comparator which compares percent matches won and returns whether this value is greater for one row or another
//If the percent matches are equal between two rows, it compares the names of the players
bool compare_percent_match(const std::vector<std::string> &a, const std::vector<std::string> &b)
{
	//Percent matches won
	float comp1 = atof(a[4].c_str());
	float comp2 = atof(b[4].c_str());

	//First name
	std::string comp3 = a[0]; 
	std::string comp4 = b[0];
	//Last name
	std::string comp5 = a[1]; 
	std::string comp6 = b[1];

	//Sort by percent
	if(comp1>comp2) 
	{
		return comp1>comp2;
	}

	//If the percents are equal
	else if(comp1 == comp2)
	{
		//And if the last names are equal
		if(comp5 == comp6)
		{
			//Sort by first name
			return comp3<comp4; 
		}
		//Otherwise, Sort by last name
		else
		{
 			return comp5<comp6;
		}
	}
	else
	{
		//This shouldn't happen, but you never know
		return false; 
	}
}

//Comparator which compares percent games won and returns whether this value is greater for one row or another
//If the percent games are equal between two rows, it compares the names of the players
bool compare_percent_games(const std::vector<std::string> &a, const std::vector<std::string> &b)
{
	//Percent games won
	float comp1 = atof(a[7].c_str());
	float comp2 = atof(b[7].c_str());

	//First name
	std::string comp3 = a[0];
	std::string comp4 = b[0];
	//Last name
	std::string comp5 = a[1];
	std::string comp6 = b[1];

	//Sort by percent
	if(comp1>comp2) 
	{
		return comp1>comp2;
	}

	//If scores are equal
	else if(comp1 == comp2) 
	{
		//And if last names are equal
		if(comp5 == comp6) 
		{
			//Sort by first name
			return comp3<comp4; 
		}
		else
		{
			//Otherwise, sort by last name
 			return comp5<comp6; 
		}	
	}

	else
	{
		//This shouldn't happen, but you never know
		return false;
	}

}

