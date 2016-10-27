/*
kitchen.h

Class that creates a kitchen cabinet
Stores a list of ingredients available in the pantry with the respective amounts
*/

#ifndef __kitchen_h_
#define __kitchen_h_

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "inventory.h"

class Kitchen
{
public:
	//Default constructor
	Kitchen();

	//Function that prints the ingredients in the kitchen
	void PrintIngredients(std::ofstream &ostr);

	//Function that adds ingredient to the ingredient master list
	void AddIngredient(std::string name, int units);

	//Accessor
	std::list<Inventory> GetIngredients() const;

	//Function that modifies contents of pantry
	void UseIngredients(std::string recipe_ingredient, int quantity);

	std::list<Inventory> IngredientsNeeded(std::list<Inventory> recipe_ingredients);

private:
	//Master list of ingredients in the kitchen
	std::list<Inventory> ingredients;
};

//Comparator to sort kitchen objects by amount first and alphabetically second
bool CompareKitchenAmounts(const Inventory& ingredient1,const Inventory& ingredient2);

#endif

