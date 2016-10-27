/*
recipe.cpp

File that implements the functions from recipe.h
Adds ingredients to ingredient list for the given recipe
*/

#include "recipe.h"

//Default constructor
Recipe::Recipe(std::string name)
{
	//Make the name the user inputs a private variable
	rname=name;
}

//Function that adds the inputted ingredients to the ingredient list
//If the input is "2 tomatoes", tomatoes are added to the ingredient list twice
void Recipe::AddIngredient(std::string iname, int units)
{
	Inventory temp(iname,units);
	//For as many times as the number of units inputted
	ingredients.push_back(temp);

	//Sort list of recipe ingredients each time you add one
	ingredients.sort(CompareIngredients);
}

//Ingredient list accessor
std::list<Inventory> Recipe::GetIngredients() const
{
	return ingredients;
}

//Recipe name accessor
std::string Recipe::GetName() const
{
	return rname;
}

//Comparator to help sort the list of recipe ingredients alphabetically
bool CompareIngredients(const Inventory& ingredient1,const Inventory& ingredient2)
{
	//Check if ingredient 1 comes alphabetically before ingredient 2
	if(ingredient1.GetFood() < ingredient2.GetFood())
	{
		//Return true
		return ingredient1.GetFood() < ingredient2.GetFood();
	}

	//Otherwise, return false
	else
	{
		return false;
	}
}