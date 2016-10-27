/*
inventory.cpp

Implements functions from inventory.h
Modifies the amount of the ingredient we have
*/

#include "inventory.h"

//Constructor
Inventory::Inventory(std::string ingredient, int amount)
{
	//Initialize number of ingredients to amount
	food = ingredient;
	number_of_ingredients = amount;
}

//Access food string (ingredient)
std::string Inventory::GetFood() const
{
return food;
}

//Access the running total of food amount
int Inventory::GetNumber() const
{
return number_of_ingredients;
}

//Set the food string to be the given ingredient
void Inventory::SetFood(std::string ingredient)
{
	food = ingredient;
}

//Increment the number of ingredient type
void Inventory::IncreaseNumber(int amount)
{
	number_of_ingredients += amount;
}

//Decrement the number of ingredient type
void Inventory::DecreaseNumber(int amount)
{
	number_of_ingredients -= amount;
}