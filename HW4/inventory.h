/*
inventory.h

Creates a string and integer pair, where the string is the ingredient and the integer is how many units to add to the corresponding running total
Allows us to store a list of ingredients along with how many of that ingredient we have in the same node
*/

#ifndef __inventory_h_
#define __inventory_h_

#include <iostream>
#include <string>

class Inventory
{
public:
	//Constructor
	Inventory(std::string ingredient, int amount);

	//Accessors
	std::string GetFood() const;
	int GetNumber() const;

	//Modifiers
	void SetFood(std::string ingredient);
	void IncreaseNumber(int amount);
	void DecreaseNumber(int amount);

private:
	int number_of_ingredients;
	std::string food;
};

#endif