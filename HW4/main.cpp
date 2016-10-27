/*
Program that takes in an input file of grocery commands, modifies the users lists of recipes and kitchen ingredients,
    and outputs the desired results to an output file
The functions available can read in a new recipe, add ingredients to the kitchen, print a recipe, make a recipe
    with available kitchen ingredients, print the available ingredients in the kitchen, suggest recipes that can
    be made with the currently available ingredients
*/

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include "recipe.h"
#include "kitchen.h"


//Function declarations
void ReadRecipe(std::istream &istr, std::ostream &ostr, std::list<Recipe> &recipes);
void AddIngredients(std::istream &istr, std::ostream &ostr, Kitchen &kitchen);
void PrintRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes);
void MakeRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes, Kitchen &kitchen);
void RecipeSuggestions(std::ostream &ostr, std::list<Recipe> &recipes, const Kitchen &kitchen);
bool HaveIngredients(const std::list<Inventory> recipe_ingredients, const std::list<Inventory> ingredients_needed);
bool CompareRecipes(const Recipe& recipe1,const Recipe& recipe2);

//The main loop parses opens the files for I/O & parses the input
int main(int argc, char* argv[])
{

    //Check the number of arguments.
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " in-file out-file\n";
        return 1;
    }

    //Open and test the input file.
    std::ifstream istr(argv[1]);
    if (!istr)
    {
        std::cerr << "Could not open " << argv[1] << " to read\n";
        return 1;
    }

    //Open and test the output file.
    std::ofstream ostr(argv[2]);
    if (!ostr)
    {
        std::cerr << "Could not open " << argv[2] << " to write\n";
        return 1;
    }

    //The kitchen object & recipe list
    Kitchen kitchen;
    std::list<Recipe> recipes;

    //Some variables to help with parsing
    char c;
    while (istr >> c)
    {
        //std::cout << "Char c is "<<c<<" \n";
        if (c == 'r')
        {
            //READ A NEW RECIPE
            ReadRecipe(istr,ostr,recipes);

        }
        else if (c == 'a')
        {
            //ADD INGREDIENTS TO THE KITCHEN
            AddIngredients(istr,ostr,kitchen);

        }
        else if (c == 'p')
        {
            //PRINT A PARTICULAR RECIPE
            PrintRecipe(istr,ostr,recipes);

        }
        else if (c == 'm')
        {
            //MAKE A RECIPE USING AVAILABLE INGREDIENTS
            MakeRecipe(istr,ostr,recipes,kitchen);

        }
        else if (c == 'k')
        {
            //PRINT THE CONTENTS OF THE KITCHEN
            kitchen.PrintIngredients(ostr);

        }
        else if (c == 's')
        {
            //SUGGEST ALL RECIPES THAT CAN BE MADE INDIVIDUALLY FROM THE
            //  CURRENT CONTENTS OF THE KITCHEN
            RecipeSuggestions(ostr,recipes,kitchen);

        }
        else if (c == 'd')
        {
            //Don't do anything. 
            //Leave this in here though in case she calls 'd' so the program doesn't crash
        }
        else
        {
            std::cerr << "unknown character: " << c << std::endl;
            exit(0);
        }
    }
}

//If 'r' command is typed, the recipe name is added to the recipe list and 
//  the corresponding ingredients are added to the recipe
void ReadRecipe(std::istream &istr, std::ostream &ostr, std::list<Recipe> &recipes)
{
    //How much of the ingredient is required for the recipe
    int units;

    std::string name, name2;

    //Recipe name
    istr >> name;

    //Iterator beginning at front of recipe list
    std::list<Recipe>::const_iterator it = recipes.begin();

    bool found = false;

    //For every node in the recipe list
    for(; it != recipes.end(); ++it)
    {
        //If you find the recipe name already in the recipe list
        if(it->GetName() == name)
        {
            //Set the bool to true so you know the recipe already exists
            found = true;
        }
    }

    //If the recipe is not already in the recipe book
    if(found == false)
    {
        //Build the new recipe using the ingredients given
        Recipe r(name);
        while (1)
        {
            istr >> units;
            if (units == 0) break;
            assert (units > 0);
            istr >> name2;

            //Add ingredient to recipe
            r.AddIngredient(name2,units);
        }

        //Add the recipe to the list
        recipes.push_back(r);
        ostr << "Recipe for " << name << " added" << std::endl;
    }

    //If the recipe was found in the recipe book already
    else
    {
        //Tell the user to stop doing dumb things
        ostr << "Recipe for " << name << " already exists\n";

        //And skip over the following ingredients until you get to a zero character
        while (1)
        {
            istr >> units;
            if (units == 0) break;
            assert (units > 0);
            istr >> name2;
        }
    }
}

//Function that adds ingredients to the kitchen pantry
void AddIngredients(std::istream &istr, std::ostream &ostr, Kitchen &kitchen)
{
    //How many of the ingredient
    int units;

    //Ingredient name
    std::string name;

    int count = 0;

    while (1)
    {
        istr >> units;
        if (units == 0) break;
        assert (units > 0);
        istr >> name;

        //Add the ingredients to the kitchen
        kitchen.AddIngredient(name,units);
        count++;
    }

    if(count == 1)
    {
    ostr << count << " ingredient added to kitchen\n";
    }

    else if(count > 1)
    {
    ostr << count << " ingredients added to kitchen\n";
    }
}

//Function that prints the ingredients and amounts required to make a recipe
void PrintRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes)
{
    std::string name;
    istr >> name;

    //Iterator for counting through available recipes
    std::list<Recipe>::const_iterator it = recipes.begin();

    //This list will store the list of ingredients from the recipe class
    std::list<Inventory> recipe_ingredients;

    bool found = false;

    //For all the recipe names
    for(it = recipes.begin(); it != recipes.end(); ++it)
    {
        //If you find the recipe name you're looking for
        if(it->GetName() == name)
        {
            ostr << "To make " << name << ", mix together:\n";

            //Access the ingredient list for the recipe in question
            recipe_ingredients = it->GetIngredients();

            //Iterator for counting through the ingredients for the given recipe
            std::list<Inventory>::iterator it2 = recipe_ingredients.begin();

            //Go through each ingredient in recipe and print to output file
            for(it2 = recipe_ingredients.begin(); it2 != recipe_ingredients.end(); ++it2)
            {
                if(it2->GetNumber() == 1)
                {
                    ostr << "  " << it2->GetNumber() << " unit of " << it2->GetFood() << "\n";
                }

                else
                {
                    ostr << "  " << it2->GetNumber() << " units of " << it2->GetFood() << "\n";
                }
            }

            found = true;
        }
    }

    //If you do not find the recipe name you're looking for
    if(found == false)
    {
        //Tell the user the recipe does not exist
        ostr<<"No recipe for " << name << "\n";
    }

}

//Function that iterates through both recipe ingredients and the pantry ingredients
//If you have enough of the ingredients, it will "make" the recipe by subtracting the recipe ingredients from the kitchen ingredients
void MakeRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes, Kitchen &kitchen)
{
    std::string name;
    istr >> name;

    //Iterator for counting through available recipes
    std::list<Recipe>::const_iterator it = recipes.begin();   

    //This list will store the list of ingredients from the recipe class
    std::list<Inventory> recipe_ingredients; 

    //If we don't have the required amount of ingredients to make the recipe, the missing ingredients will be stored here
    std::list<Inventory> ingredients_needed;

    bool found_recipe = false;

    //For all the recipe names
    for(it = recipes.begin(); it != recipes.end(); ++it)
    {
        //If you find the recipe name you're looking for
        if(it->GetName() == name)
        {
            //You found the recipe in the recipe book
            found_recipe = true;

            //Access the ingredient list for the recipe you found
            recipe_ingredients = it->GetIngredients();

            //This list will store the available ingredients in the pantry
            std::list<Inventory> kitchen_ingredients;

            //Access the list of available kitchen ingredients and store them in our list
            kitchen_ingredients = kitchen.GetIngredients();

            //For iterating through recipe ingredient list
            std::list<Inventory>::const_iterator recipe_ingredient_it = recipe_ingredients.begin();

            //For iterating through kitchen ingredient list            
            std::list<Inventory>::iterator kitchen_ingredient_it = kitchen_ingredients.begin();

            //Check if we have all the ingredients in the pantry we need for the recipe with our bool function
            bool have_ingredients = HaveIngredients(recipe_ingredients, kitchen_ingredients);

            //If we have all the ingredients we need, along with the correct amounts
            if(have_ingredients == true)
            {
                //For every ingredient in recipe list
                for(recipe_ingredient_it = recipe_ingredients.begin(); recipe_ingredient_it != recipe_ingredients.end(); ++recipe_ingredient_it)
                {
                    kitchen.UseIngredients(recipe_ingredient_it->GetFood(), recipe_ingredient_it->GetNumber());
                }

                //Tell the user that the recipe was successfully made
                ostr << "Made " << name << "\n";      
            }

            else if(have_ingredients == false)
            {
                ostr << "Cannot make " << name << ", need to buy:\n";
                std::list<Inventory> ingredients_needed = kitchen.IngredientsNeeded(recipe_ingredients);

                std::list<Inventory>::const_iterator ingit = ingredients_needed.begin();
                
                for(ingit = ingredients_needed.begin(); ingit != ingredients_needed.end(); ++ingit)
                {
                    //If there's only one of them, print singular tense
                    if(ingit->GetNumber() == 1)
                    {
                        ostr << "  " << ingit->GetNumber() << " unit of " << ingit->GetFood() << "\n";
                    }

                    //If there is more than one of the item, print plural tense
                    else
                    {
                        ostr << "  " << ingit->GetNumber() << " units of " << ingit->GetFood() << "\n";
                    }
                } 
            }
        }
    }

    //If you couldn't find the recipe in the recipe book
    if(found_recipe == false)
    {
        ostr << "Don't know how to make " << name << "\n";
    }
}

//Returns true if we have all the ingredients required to make the recipe
//Returns false if we do not have all the ingredients required to make the recipe
bool HaveIngredients(const std::list<Inventory> recipe_ingredients, const std::list<Inventory> kitchen_ingredients)
{
    int count1 = 0;

    int count2 = 0;

    //For iterating through recipe ingredient list
    std::list<Inventory>::const_iterator recipe_ingredient_it = recipe_ingredients.begin();

    //For iterating through kitchen ingredient list            
    std::list<Inventory>::const_iterator kitchen_ingredient_it = kitchen_ingredients.begin();

    //For every ingredient in recipe list
    for(recipe_ingredient_it = recipe_ingredients.begin(); recipe_ingredient_it != recipe_ingredients.end(); ++recipe_ingredient_it)
    {
        //And for every ingredient in the kitchen pantry
        for(kitchen_ingredient_it = kitchen_ingredients.begin(); kitchen_ingredient_it != kitchen_ingredients.end(); ++kitchen_ingredient_it)
        {
            //If you found the ingredient in the pantry
            //And if you have enough of that ingredient in the pantry
            if(recipe_ingredient_it->GetFood() == kitchen_ingredient_it->GetFood() && recipe_ingredient_it->GetNumber() <= kitchen_ingredient_it->GetNumber())
            {
                //Add one to the second count (count2 will be == to number of recipe ingredients if you have them all)
                count2++;
            }
        }
        //Count how many recipe ingredients there are total
        count1++;
    }

    //If we could find all the necessary ingredients in the pantry
    if(count1 == count2)
    {
        return true;
    }

    //Otherwise, return false
    else
    {
        return false;
    }
}

//Function that tells the user what recipes they can make using the ingredients they have
//This does not take into account the use of ingredients for each recipe
//For instance, if you are able to make both cake and quiche with the given pantry ingredients,
//      you still might not be able to make cake *then* quiche
void RecipeSuggestions(std::ostream &ostr, std::list<Recipe> &recipes, const Kitchen &kitchen)
{
    //Sort the recipe list by name
    recipes.sort(CompareRecipes);

    //Will add recipes we can make to this list
    //Only need to do this because we need to know if the amount of recipes we can make
    //      is singular or plural
    std::list<std::string> recipe_temp;

    //For iterating through recipe list
    std::list<Recipe>::iterator it = recipes.begin();

    //This list will store the available ingredients in the pantry
    std::list<Inventory> kitchen_ingredients;

    //Access the list of available kitchen ingredients and store them in our list
    kitchen_ingredients = kitchen.GetIngredients();

    bool can_make_stuff = false;

    int count = 0;

    //For all the recipe names
    for(it = recipes.begin(); it != recipes.end(); ++it)
    {  
        //This list will store the list of ingredients from the recipe class
        std::list<Inventory> recipe_ingredients; 

        //Access the ingredient list for the recipe you found
        recipe_ingredients = it->GetIngredients();

        //The have_ingredients function checks if you have the necessary ingredients in the kitchen for each recipe
        bool have_ingredients = HaveIngredients(recipe_ingredients,kitchen_ingredients);

        //If you find a recipe that can be prepared
        if(have_ingredients == true)
        {
            can_make_stuff = true;

            recipe_temp.push_back(it->GetName());

            count++;
        }
    }

    //If you didn't find any valid recipes
    if(can_make_stuff == false)
    {
        ostr << "No recipes can be prepared\n";
    }

    else if(can_make_stuff == true)
    {
        std::list<std::string>::iterator recipe_it = recipe_temp.begin();

        if(count == 1)
        {
            ostr << "Recipe that can be prepared:\n";

            for(recipe_it = recipe_temp.begin(); recipe_it != recipe_temp.end(); ++recipe_it)
            {
                ostr << "  " << *recipe_it << "\n";
            }
        }

        else
        {
            ostr << "Recipes that can be prepared:\n";

            for(recipe_it = recipe_temp.begin(); recipe_it != recipe_temp.end(); ++recipe_it)
            {
                ostr << "  " << *recipe_it << "\n";
            }
        }
    }
}

//Comparator to help sort the list of recipes alphabetically
bool CompareRecipes(const Recipe& recipe1,const Recipe& recipe2)
{
    //Check if recipe 1 comes alphabetically before recipe 2
    if(recipe1.GetName() < recipe2.GetName())
    {
        //Return true
        return recipe1.GetName() < recipe2.GetName();
    }

    //Otherwise, return false
    else
    {
        return false;
    }
}
