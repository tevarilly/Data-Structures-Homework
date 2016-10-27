#ifndef __tennis_h_
#define __tennis_h_

#include <iostream>
#include <string>
#include <vector>

class Tennis 
{
public:
	//Default constructor
	Tennis ();

	Tennis(std::vector<std::string> input_vector);
	int names_list(std::vector<std::string> input_vector);

	//Accessor
	std::vector< std::vector< std::string > > get_player_database();

private:
	//player_database will hold player first name, player last name, number of matches won, number of matches lost,
	//percent matches won, number of games won, number of games lost, and percent games won (universal and private)
	std::vector< std::vector< std::string > > player_database;
	int match_stats(std::vector< std::vector< std::string > > &player_database, std::vector<std::string> input_vector);
	int game_database(std::vector< std::vector< std::string > > &player_database, std::vector<std::string> input_vector);
	int game_stats(std::vector< std::vector< std::string > > &player_database);
};

//Comparators for sorting
bool compare_percent_match(const std::vector<std::string> &a, const std::vector<std::string> &b);
bool compare_percent_games(const std::vector<std::string> &a, const std::vector<std::string> &b);

#endif