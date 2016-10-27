/*
kitchen.cpp

File that implements the functions from kitchen.h
Stores a list of ingredients in the kitchen (the pantry)
Adds ingredients to the pantry
Prints the ingredients in the pantry
*/

#include "kitchen.h"

//Default constructor
Kitchen::Kitchen()
{
}

//Function that prints the current contents of the pantry
void Kitchen::PrintIngredients(std::ofstream &ostr)
{
	//Sort list of ingredients before they are printed
	ingredients.sort(CompareKitchenAmounts);

	std::list<Inventory>::iterator it = ingredients.begin();

	ostr << "In the kitchen:\n";

	//For every item in the pantry
	for(it = ingredients.begin(); it != ingredients.end(); ++it)
	{
		//If there's only one of them, print singular tense
		if(it->GetNumber() == 1)
		{
			ostr << "  " << it->GetNumber() << " unit of " << it->GetFood() << "\n";
		}

		//If there is more than one of the item, print plural tense
		else
		{
			ostr << "  " << it->GetNumber() << " units of " << it->GetFood() << "\n";
		}
	}
}

//Add ingredient to the pantry 
void Kitchen::AddIngredient(std::string name, int units)
{
	std::list<Inventory>::iterator it = ingredients.begin();

	bool found = false;

	//For every item in kitchen pantry
	for(it = ingredients.begin(); it != ingredients.end(); ++it)
	{
		//If the ingredient you're looking for is already in stock
		if(it->GetFood()==name)
		{
			//Simply increase the number of stock
			found = true;
			it->IncreaseNumber(units);
		}
	}

	//If the ingredient is not already in the pantry
	if(found == false)
	{
		//Add the ingredient and amount to the pantry 
		Inventory temp(name,units);
		ingredients.push_back(temp);
	}
}

//Ingredient list accessor
std::list<Inventory> Kitchen::GetIngredients() const
{
	return ingredients;
}

//Function that subtracts ingredients in the recipe (qith quantities accounted for) from the kitchen pantry
void Kitchen::UseIngredients(std::string recipe_ingredient, int quantity)
{
	std::list<Inventory>::iterator it = ingredients.begin();

	//For every ingredient in the pantry
	for(it = ingredients.begin(); it != ingredients.end(); ++it)
	{
		//If you find the recipe ingredient in the pantry
		//And the quantity required for the recipe is the same as the quantity you have in stock
		if(it->GetFood() == recipe_ingredient && it->GetNumber() == quantity)
		{
			//Delete the ingredient from the pantry
			it = ingredients.erase(it);
		}

		//Otherwise, if you find the recipe ingredient in the pantry
		//And the quantity required for the recipe is less than the quantity you have in stock
		else if(it->GetFood() == recipe_ingredient && it->GetNumber() > quantity)
		{
			//Decrement the amount you have in the pantry by quantity
			it->DecreaseNumber(quantity);
		}
	}
}

std::list<Inventory> Kitchen::IngredientsNeeded(std::list<Inventory> recipe_ingredients)
{
	std::list<Inventory> ingredients_needed;

	std::list<Inventory>::iterator it = ingredients.begin();

	std::list<Inventory>::iterator it2 = recipe_ingredients.begin();

	bool found_ingredient = false;

	for(it2 = recipe_ingredients.begin(); it2 != recipe_ingredients.end(); ++it2)
	{
		for(it = ingredients.begin(); it != ingredients.end(); ++it)
		{
			if(it2->GetFood() == it->GetFood())
			{
				found_ingredient = true;

				if(it2->GetNumber() > it->GetNumber())
				{
					int new_amount = it2->GetNumber()-it->GetNumber();

					Inventory temp(it2->GetFood(),new_amount);

					ingredients_needed.push_back(temp);
				}
			}
		}

		if(found_ingredient == false)
		{
			Inventory temp(it2->GetFood(),it2->GetNumber());
			ingredients_needed.push_back(temp);
		}

		found_ingredient = false;
	}

	return ingredients_needed;
}

//Comparator to help sort the list of kitchen objects by number first and alphabetically second
bool CompareKitchenAmounts(const Inventory& ingredient1,const Inventory& ingredient2)
{
	//If ingredient 1 amount is smaller than ingredient 2 amount
	if(ingredient1.GetNumber() < ingredient2.GetNumber())
	{
		//Return true
		return ingredient1.GetNumber() < ingredient2.GetNumber();
	}

	//Otherwise, if the amounts are equal, sort alphabetically
	else if(ingredient1.GetNumber() == ingredient2.GetNumber())
	{
		return ingredient1.GetFood() < ingredient2.GetFood();
	}

	//Otherwise, return that ingredient 1 amount is not less than ingredient 2 amount
	else
	{
		return false;
	}
}