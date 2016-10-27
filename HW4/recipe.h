/*
recipe.h

Class that creates a recipe object using the recipe name given (e.g. salad)
Adds ingredients provided for the recipe to an ingredient list
*/

#ifndef __recipe_h_
#define __recipe_h_

#include <iostream>
#include <string>
#include "kitchen.h"

class Recipe
{
public:
	//Default constructor which takes in a recipe name
	Recipe(std::string name);

	//Function that adds ingredients to the ingredient list
	void AddIngredient(std::string name, int units);

	//Accessors
	std::list<Inventory> GetIngredients() const;
	std::string GetName() const;

private:
	//Master list of ingredients for the recipe
	std::list<Inventory> ingredients;

	//Once the recipe name is inputted, it should not be changed
	std::string rname;
};

//Comparator to help sort the list of recipe ingredients alphabetically
bool CompareIngredients(const Inventory& ingredient1,const Inventory& ingredient2);

#endif