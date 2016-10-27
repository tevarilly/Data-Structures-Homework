//traincar.cpp
//This file implements the functions declared in the prototypes helper file
//Some functionalities include adding a train car, deleting all train cars, 

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "traincar.h"
#include <unistd.h>

//Function that adds a train car to the end of the train
//head is the first element in the train list
//current is the train car you would like to add to the end
void PushBack(TrainCar* &head, TrainCar* current)
{
	//If you haven't even created a train yet
	if(head == NULL || isItNull(head) == true)
	{
		//Make the head of the list the car you want to add
		head = current;
	}

	//Otherwise, if you have already created a train
	else
	{
		//Make a temporary pointer to the first train car
		TrainCar* temp = head;

		//While you haven't reached the last train car
		while(temp->next != NULL)
		{
			//Move temp over by 1
			temp = temp->next;
		}

		//If you've reached the end of the train (which you should have, because of that while loop)
		if(temp->next == NULL)
		{
			//Make the last element of the train point to the train car you want to add
			temp->next = current;
			//Make the train car you just added point back at it
			current->prev = temp;
		}
	}

}

// =======================================================================
// =======================================================================

//Function that adds a train car to the beginning of a train
//head is the first element in the train list
//current is the train car you would like to add to the beginning
void PushFront(TrainCar* &head, TrainCar* current)
{
	//If you haven't even created a train yet
	if(head == NULL || isItNull(head) == true)
	{
		//Make the head of the list the car you want to add
		head = current;
	}

	//Otherwise, if you have already created a train
	else
	{
		//Make the first train car point back to the car you want to add
		head->prev = current;

		//Make sure the car you want to add points backward to garbage
		current->prev = NULL;

		//Make the car you want to add point to the old head of the train 
		current->next = head;

		//Reassign the head of the train to current
		head = current;
	}
}

// =======================================================================
// =======================================================================

//Function that deletes all cars in the train
void DeleteAllCars(TrainCar* &head)
{
	if(head==NULL)
	{
		return;
	}
	//Make an iterator that will go through the train cars
	TrainCar* it = head;
	//Make another iterator that will be one ahead of the previous iterator
	TrainCar* temp = it->next;

	//As long as the train isn't empty
	if(head != NULL)
	{
		//While you haven't reached the last train car
		while(it->next != NULL)
		{
			//Delete the train car you're currently on
			//std::cout<<"delete "<<it->getID()<<"\n";
			delete it;

			//Move it over to where temp is (one to the right)
			it = temp;

			//If you aren't in the last train car
			if(it->next != NULL)
			{
				//Move temp over by 1
				temp = it->next;
			}
		}

		//Once you've reached the last train car (which you should have by the end of that while loop)
		if(it->next == NULL)
		{
			//Delete that last train car
			//std::cout<<"delete "<<it->getID()<<"\n";
			delete it;
		}

	}
}

// =======================================================================
// =======================================================================

//Function that finds multiple statistics about the train and returns all the desired values
void TotalWeightAndCountCars(TrainCar* &head,int &total_weight,int &num_engines,int &num_freight_cars,int &num_passenger_cars,int &num_dining_cars,int &num_sleeping_cars)
{
	//Initialize all counting variables to zero
	total_weight = 0;
	num_engines = 0;
	num_freight_cars = 0;
	num_passenger_cars = 0;
	num_dining_cars = 0;
	num_sleeping_cars = 0;

	//Make an iterator beginning at first train car (which was passed into the function)
	TrainCar* it = head;

	//As long as you have a valid train
	if(head != NULL)
	{
		//While you have not reached the last car
		while(it->next != NULL)
		{
			//Add the weight of the car you're on to the total weight
			total_weight += it->getWeight();

			//If the car you're on is an engine
			if(it->isEngine())
			{
				//Add 1 to the engine count
				num_engines += 1;
			}

			//If the car you're on is a frieght car
			if(it->isFreightCar())
			{
				//Add 1 to the freight car count
				num_freight_cars += 1;
			}

			//If the car you're on is a passenger car
			if(it->isPassengerCar())
			{
				//Add 1 to the passenger car count
				num_passenger_cars += 1;
			}

			//If the car you're on is a dining car
			if(it->isDiningCar())
			{
				//Add 1 to the dining car count
				num_dining_cars += 1;
			}

			//If the car you're on is a sleeping car
			if(it->isSleepingCar())
			{
				//Add 1 to the sleeping car count
				num_sleeping_cars += 1;
			}

			//Increment the iterator
			it = it->next;
		}

		//If you've reached the last train car
		if(it->next == NULL)
		{
			//Add the weight of the last car to the total weight
			total_weight += it->getWeight();

			//If the last car is an engine
			if(it->isEngine())
			{
				//Add 1 to the engine count
				num_engines += 1;
			}

			//If the last car is a freight car
			if(it->isFreightCar())
			{
				//Add 1 to the freight car count
				num_freight_cars += 1;
			}

			//If the last car is a passenger car
			if(it->isPassengerCar())
			{
				//Add 1 to the passenger car count
				num_passenger_cars += 1;
			}

			//If the car you're on is a dining car
			if(it->isDiningCar())
			{
				//Add 1 to the dining car count
				num_dining_cars += 1;
			}

			//If the car you're on is a sleeping car
			if(it->isSleepingCar())
			{
				//Add 1 to the sleeping car count
				num_sleeping_cars += 1;
			}
		}
	}
}

// =======================================================================
// =======================================================================

//Function that will calculate the speed of the train, given the first train car
float CalculateSpeed(TrainCar* &head)
{
	//Need to re-calculate the num_engines variable
	//	because it is only calculated when print train function is called
	float num_engines = 0;
	float total_weight = 0;

	//Iterator for going through the train cars
	TrainCar* it = head;

	//If you actually have a train and not an empty list
	if(head != NULL)
	{
		//While you have not reached the last car
		while(it->next != NULL)
		{
			//If you find an engine car
			if(it->isEngine())
			{
				//Add one to the engine count
				num_engines += 1;
			}

			//And increment the weight
			total_weight += it->getWeight();

			it = it->next;
		}

		//If you have reached the last car, which you should have after the while loop
		if(it->next == NULL)
		{
			//If the last car is an engine car
			//Honestly, who puts an engine at the end of a train
			if(it->isEngine())
			{
				//Add one to the engine count
				num_engines +=1;
			}

			//And increment the weight
			total_weight += it->getWeight();
		}
	}

	//Calculate total horsepower for the train
	float horsepower = num_engines*3000;

	float speed = 0;

	//Calculate speed in miles per hour
	speed = (horsepower*550*3600)/(20*2*5280*total_weight);

	return speed;
}

// =======================================================================
// =======================================================================

//Helper function that takes in a train and a train car you want to add
//	Calculates what the speed would be if this car were on the train
float SpeedProjection(TrainCar* &head, TrainCar* &car_to_add)
{
	//Need to re-calculate the num_engines variable
	//	because it is only calculated when print train function is called
	float num_engines = 0;
	float total_weight = 0;

	//If the car you want to add is an engine
	if(car_to_add->isEngine())
	{
		//Add to total number of engines
		num_engines += 1;
	}

	//Account for the weight of the car you want to add
	total_weight += car_to_add->getWeight();

	//Iterator for going through the train cars
	TrainCar* it = head;

	//If you actually have a train and not an empty list
	if(head != NULL || isItNull(head) == true)
	{
		//While you have not reached the last car
		while(it->next != NULL)
		{
			//If you find an engine car
			if(it->isEngine())
			{
				//Add one to the engine count
				num_engines += 1;
			}

			//And increment the weight
			total_weight += it->getWeight();

			it = it->next;
		}

		//If you have reached the last car, which you should have after the while loop
		if(it->next == NULL)
		{
			//If the last car is an engine car
			//Honestly, who puts an engine at the end of a train
			if(it->isEngine())
			{
				//Add one to the engine count
				num_engines +=1;
			}

			//And increment the weight
			total_weight += it->getWeight();
		}
	}

	//Calculate total horsepower for the train
	float horsepower = num_engines*3000;

	float speed = 0;

	//Calculate speed in miles per hour
	speed = (horsepower*550*3600)/(20*2*5280*total_weight);

	return speed;
}

// =======================================================================
// =======================================================================

//Function that looks through train and finds passenger cars
//It then finds the distance to the closest dining car
float AverageDistanceToDiningCar(TrainCar* &head)
{
	//Iterator for going through train cars
	TrainCar* it = head;

	//Running total of passenger cars (for calculating average)
	float num_passenger_cars = 0.0;
	//Running average distance to dining car
	float running_average = 0.0;

	//If you don't have an empty train
	if(head != NULL)
	{
		//While you have not reached the last car on the train
		while(it->next != NULL)
		{
			//If you find a passenger car
			if(it->isPassengerCar())
			{
				//Increment total number of passenger cars
				num_passenger_cars +=1.0;

				//Will count the distances to dining cars forward and backward
				float countright = 0.0;
				float countleft = 0.0;

				bool found_dine1 = false;
				bool found_dine2 = false;

				bool found_engine1 = false;
				bool found_engine2 = false;

				bool found_freight1 = false;
				bool found_freight2 = false;

				//Make new iterator for each passenger car to look for dining cars
				TrainCar* itnew = it;

				//Look through all the train cars after the passenger car (to the right)
				//	as long as you don't reach the last car and have not yet already found 
				//	a dining car to the right
				//	and you have not encountered an engine or a freight car
				while(itnew->next != NULL && found_dine1 == false && found_engine1 == false && found_freight1 == false)
				{

					//If you find a dining car to the right
					if(itnew->isDiningCar())
					{
						//Stop incrementing
						found_dine1 = true;

						//Countright gets incremented one too many, so account for that
						countright--;
					}

					//If you find an engine
					if(itnew->isEngine())
					{
						//Stop incrementing
						found_engine1 = true;
					}

					//If you encounter a freight car
					if(itnew->isFreightCar())
					{
						//Stop incrementing
						found_freight1 = true;
					}

					//As you increment, count the number of cars until you find a dining car
					countright += 1.0;

					//Increment to the right
					itnew = itnew->next;
				}

				//Check the last car (if you got that far in previous while loop)
				if(itnew->next == NULL)
				{
					//If the last car is a dining car
					//	and you haven't already found a dining car to the right 
					//	and you didn't encounter an engine
					if(itnew->isDiningCar() && found_dine1 == false && found_engine1 == false && found_freight1 == false)
					{
						//Add 1 to the right count
						countright += 1.0;

						//Note that we found a dining car
						found_dine1 = true;
					}
				}

				//If you didn't find any dining cars to the right
				if(found_dine1 == false)
				{
					//Reset the right count to 0 so we know
					countright = 0.0;
				}

				//Put the second iterator back on the original passenger car
				itnew = it;	

				//Look through all the train cars before the passenger car (to the left)
				//	as long as you don't reach the first car and have not yet already found
				//	a dining car to the left
				//	and you have not encountered an engine or a frieght car
				while(itnew->prev != NULL && found_dine2 == false && found_engine2 == false && found_freight2 == false)
				{
					//If you find a dining car
					if(itnew->isDiningCar())
					{
						//Stop incrementing
						found_dine2 = true;

						//Countleft gets incremented one too many so account for that
						countleft--;
					}

					//If you encounter an engine
					if(itnew->isEngine())
					{
						//Stop incrementing
						found_engine2 = true;
					}

					//If you encounter a freight car
					if(itnew->isFreightCar())
					{
						//Stop incrementing
						found_freight2 = true;
					}

					//Add 1 to the left count
					countleft += 1.0;

					//Increment to the left
					itnew = itnew->prev;
				}

				//Check the first car (if you got that far in the previous while loop)
				if(itnew->prev == NULL)
				{
					//If the first car is a dining car
					//	and you haven't already found a dining car to the left
					//	and if you haven't encountered an engine or a freight car,
					if(itnew->isDiningCar() && found_dine2 == false && found_engine2 == false && found_freight2 == false)
					{
						//Add 1 to left count
						countleft += 1.0;

						//Note that we found a dining car
						//	and then ponder why somebody put a dining car in the front of a train
						found_dine2 = true;
					}
				}

				//If you didn't find any dining cars to the left
				if(found_dine2 == false)
				{
					//Reset the left count to 0 so we know
					countleft = 0.0;
				}

				//If the passenger car is sandwiched by two engines
				if(found_engine1 == true && found_engine2 == true)
				{
					//Stop what you're doing because the average distance is infinity
					return -1;
				}

				//If the passenger car is sandwiched by two freight cars
				if(found_freight1 == true && found_freight2 == true)
				{
					//Stop what you're doing because the average distance is infinity
					return -1;
				}

				//If the passenger car is sandwiched by an engine and a freight
				if(found_freight1 == true && found_engine2 == true)
				{
					//Stop what you're doing because the average distance is infinity
					return -1;
				}

				//If the passenger car is sandwiched by an engine and a freight
				if(found_engine1 == true && found_freight2 == true)
				{
					//Stop what you're doing because the average distance is infinity
					return -1;
				}

				//If there is no dining car at all
				if(countleft == 0 && countright == 0)
				{
					//Stop what you're doing because the average distance is infinity
					return -1;
				}

				//If the only dining car is to the left
				if(countleft > 0 && countright == 0)
				{
					//Numerator in the running expression for calculating the average
					float numerator = running_average*(num_passenger_cars-1);

					//Add the distance to the closest dining car to the numerator
					numerator += countleft;

					//Recalculate the average
					running_average = numerator/num_passenger_cars;
				}

				//If the only dining car is to the right
				else if(countright > 0 && countleft == 0)
				{
					//Numerator in the running expression for calculating the average
					float numerator = running_average*(num_passenger_cars-1);

					//Add the distance to the closest dining car to the numerator
					numerator += countright;

					//Recalculate the average
					running_average = numerator/num_passenger_cars;
				}

				//If the closest dining car is to the right
				else if(countleft > 0 && countright > 0 && countleft > countright)
				{
					//Numerator in the running expression for calculating the average
					float numerator = running_average*(num_passenger_cars-1);

					//Add the distance to the closest dining car to the numerator
					numerator += countright;

					//Recalculate the average
					running_average = numerator/num_passenger_cars;
				}

				//If the closest dining car is to the left
				else if(countleft > 0 && countright > 0 && countright > countleft)
				{
					//Numerator in the running expression for calculating the average
					float numerator = running_average*(num_passenger_cars-1);

					//Add the distance to the closest dining car to the numerator
					numerator += countleft;

					//Recalculate the average
					running_average = numerator/num_passenger_cars;
				}

				//If there are dining cars of equal distance on both sides
				else if(countleft > 0 && countright > 0 && countright == countleft)
				{
					//Numerator in the running expression for calculating the average
					float numerator = running_average*(num_passenger_cars-1);

					//Add the distance to the closest dining car to the numerator
					numerator += countright;

					//Recalculate the average
					running_average = numerator/num_passenger_cars;
				}

				//This shouldn't ever happen, but you never know
				else
				{
					return -1;
				}
			}

			//Increment the first iterator
			it = it->next;
		}

		//Check if the very last train car
		if(it->next == NULL)
		{
			//If the last traincar is a passenger car
			if(it->isPassengerCar())
			{
				num_passenger_cars += 1;

				//Number of steps to dining car
				float count = 0.0;

				bool found_dine = false;
				bool found_engine = false;
				bool found_freight = false;

				//Make another iterator beginning on the last car
				TrainCar* itnew = it;

				//Look through all the train cars before the passenger car (to the left)
				//	as long as you don't reach the first car and have not yet already found
				//	a dining, engine, or freight car
				while(itnew->prev != NULL && found_dine == false && found_engine == false && found_freight == false)
				{
					//If you find a dining car
					if(itnew->isDiningCar())
					{
						//Stop incrementing
						found_dine = true;

						//Count gets incremented too many times, so account for that
						count--;
					}

					//If you encounter an engine
					if(itnew->isEngine())
					{
						//Stop incrementing
						found_engine = true;
					}

					//If you encounter a freight car
					if(itnew->isFreightCar())
					{
						//Stop incrementing
						found_freight = true;
					}

					//Keep counting steps to nearest dining car
					count += 1.0;

					itnew = itnew->prev;
				}

				//If you encountered an engine
				if(found_engine == true)
				{
					//Stop what you're doing because the average distance is infinity
					return -1;
				}

				//If you encountered a freight car
				if(found_freight == true)
				{
					//Stop what you're doing because the average distance is infinity
					return -1;
				}

				//If you didn't find a dining car on the train
				if(found_dine == false && found_freight == false && found_engine == false)
				{
					//Stop what you're doing because the average distance is infinity
					return -1;
				}

				//count should always be greater than zero if you've found a dining car, but you never know
				if(found_dine == true && count > 0)
				{
					//Numerator in the running expression for calculating the average
					float numerator = running_average*(num_passenger_cars-1);

					//Add the distance to the closest dining car to the numerator
					numerator += count;	
					
					//Recalculate the average
					running_average = numerator/num_passenger_cars;														
				}

			}
		}
	}

	return running_average;
}

// =======================================================================
// =======================================================================

//Function for finding minimum distance between any sleeper car and the engine
//We assume all trains have at least one engine
int ClosestEngineToSleeperCar(TrainCar* &head)
{
	//Iterator for going through train cars
	TrainCar* it = head;

	//This is our master variable for storing the minimun engine-sleeper distance
	int mincount = 0;

	//If you don't have an empty train
	if(head != NULL)
	{
		//While you have not reached the last train car
		while(it->next != NULL)
		{
			//If you find an engine
			if(it->isEngine())
			{
				//Make new iterator for each engine to look for sleeper cars
				TrainCar* itnew = it;

				//Variables for finding the closest sleeper car to each engine
				int countleft = 0;
				int countright = 0;

				bool found_sleeper1 = false;
				bool found_sleeper2 = false;

				//While you have not reached the last train car
				//	and while you have not yet found a sleeper car to the right
				while(itnew->next != NULL && found_sleeper1 == false)
				{
					//If you find a sleeper car
					if(itnew->isSleepingCar())
					{
						//Stop incrementing
						found_sleeper1 = true;

						//Countright gets incremented one too many, so account for that
						countright--;
					}

					//Increment the number of steps
					countright += 1;

					itnew = itnew->next;
				}

				//Check last train car
				if(itnew->next == NULL)
				{
					//If the last train car is a sleeping car
					//	and you haven't already found one to the right
					if(itnew->isSleepingCar() && found_sleeper1 == false)
					{
						//Note that we have found a sleeper car
						found_sleeper1 = true;
					}
				}

				//If you didn't find any sleeper cars to the right
				if(found_sleeper1 == false)
				{
					//Make note of it by resetting countright to 0
					countright = 0;
				}

				//Reset the second iterator to point to the engine
				itnew = it;

				//While you have not reached the first train car
				//	and while you have not yet found a sleeper car to the left
				while(itnew->prev != NULL && found_sleeper2 == false)
				{
					//If you find a sleeping car
					if(itnew->isSleepingCar())
					{
						//Stop incrementing
						found_sleeper2 = true;

						//Countleft gets incremented one too many, so account for that
						countleft--;
					}

					//Increment the number of steps
					countleft += 1;

					itnew = itnew->prev;
				}

				//Check first train car
				if(itnew->prev == NULL)
				{
					//If the first car is a sleeper car
					//	and you haven't already found one to the left
					if(itnew->isSleepingCar() && found_sleeper2 == false)
					{
						//Note that we have found a sleeper car
						found_sleeper2 = true;

						countleft += 1;
					}
				}

				//If you didn't find any sleeper cars to the left
				if(found_sleeper2 == false)
				{
					//Make note of it by resetting countleft to 0
					countleft = 0;
				}

				//If there is no sleeping car on the train
				if(countleft == 0 && countright == 0)
				{
					//Stop everything
					return -1;
				}

				//If we haven't established a value for mincount yet
				if(mincount == 0)
				{
					//If there is a sleeper car to the left and right
					if(countleft > 0 && countright > 0)
					{
						//If the sleeper on the right is closer
						if(countleft > countright)
						{
							mincount = countright;
						}

						//If the sleeper on the left is closer
						if(countright > countleft)
						{
							mincount = countleft;
						}

						//If the left and right sleeper cars are the same distance
						if(countright == countleft)
						{
							mincount = countright;
						}
					}

					//If there is no sleeper car on the right, but there is one to the left
					if(countleft > 0 && countright == 0)
					{
						mincount = countleft;
					}

					//If there is no sleeper car on the left, but there is one to the right
					if(countleft == 0 && countright > 0)
					{
						mincount = countright;
					}
				}

				//If we already have a value for mincount
				if(mincount > 0)
				{
					//If there is a sleeper car to the left and right
					if(countleft > 0 && countright > 0)
					{
						//If the sleeper on the right is closer
						//	and if its distance is less than our running total for mincount
						if(countleft > countright && countright < mincount)
						{
							mincount = countright;
						}

						//If the sleeper on the left is closer
						//	and if its distance is less than our running total for mincount
						if(countright > countleft && countleft < mincount)
						{
							mincount = countleft;
						}

						//If the left and right sleeper cars are the same distance
						//	and if this distance is less than our running total for mincount
						if(countleft == countright && countright < mincount)
						{
							mincount = countright;
						}
					}

					//If there is no sleeper car on the right, but there is one to the left
					//	and if the left car distance is less than our running total for mincount
					if(countleft > 0 && countright == 0 && countleft < mincount)
					{
						mincount = countleft;
					}

					//If there is no sleeper car on the left, but there is one to the right
					//	and if the right car distance is less than our running total for mincount
					if(countleft == 0 && countright > 0 && countright < mincount)
					{
						mincount = countright;
					}
				}
			}

			//Move to next train car
			it = it->next;
		}

		//Check the last train car, which you should have reached by the end of the previous while loop
		if(it->next == NULL)
		{
			//If the last train car is an engine
			if(it->isEngine())
			{
				//Make new iterator for the engine to look for sleeper cars
				TrainCar* itnew = it;	

				int count = 0;

				bool found_sleeper = false;

				//While you have not reached the first train car
				//	and have not yet found a sleeper car
				while(itnew->prev != NULL && found_sleeper == false)
				{
					//If you find a sleeper car
					if(itnew->isSleepingCar())
					{
						//Stop incrementing
						found_sleeper = true;

						//count gets incremented one too many, so account for that
						count--;
					}

					count += 1;

					itnew = itnew->prev;
				}

				//Check the first train car as long as you have not yet found a sleeper car
				if(itnew->prev == NULL && found_sleeper == false)
				{
					//If the first train car is a sleeper
					if(itnew->isSleepingCar())
					{
						//Note that you have found one
						found_sleeper = true;
					}
				}

				//If you didn't find a sleeper at all
				if(found_sleeper == false)
				{
					//Stop everything
					return -1;
				}

				//If you have not already set mincount
				if(mincount == 0)
				{
					mincount = count;
				}

				//If you alreayd have a value for mincount
				if(mincount > 0 && count < mincount)
				{
					mincount = count;
				}
			}
		}
	}

	return mincount;
}

// =======================================================================
// =======================================================================

//Function that takes a list of engines, a list of freight cars, a minimum speed, and a maximum train length
//This builds a series of trains meeting the required parameters for shipment
std::vector<TrainCar*> ShipFreight(TrainCar* &all_engines, TrainCar* &all_freight, int min_speed, int max_cars_per_train)
{
	std::vector<TrainCar*> freight_trains;

	TrainCar* engineit = all_engines;
	TrainCar* freightit = all_freight;

	int num_engines = 0;
	int num_freight_cars = 0;

	//While you don't reach the end of the list
	while(engineit->next != NULL)
	{
		//Count the number of engines
		num_engines += 1;

		engineit = engineit->next;
	}

	//Add 1 for the last value on the list
	num_engines += 1;

	//While you don't reach the end of the list
	while(freightit->next != NULL)
	{
		//Count the number of freight cars
		num_freight_cars += 1;

		freightit = freightit->next;
	}

	//Add 1 for the last value on the list
	num_freight_cars += 1;

	//Iterator for going through engines
	TrainCar* engines = all_engines;

	//Iterator for going through freight cars
	TrainCar* freight = all_freight;

	/*for(int i=0; i<num_engines; i++)
	{
		freight_trains.push_back(NULL);
	}*/
	while(all_engines!=NULL && freight!=NULL)
	{
		//std::cout<<"ALL ENGINES BEFORE REMOVE\n";
		//PrintTrain(all_engines);
		if(all_engines->next==NULL)
		{
			freight_trains.push_back(all_engines);
			all_engines=NULL;
		}
		else
		{
			freight_trains.push_back(Remove_TrainCar(all_engines));
		}
		//std::cout<<"ALL ENGINES AFTER REMOVE\n";
		//PrintTrain(all_engines);
		//std::cout<<"ENGINES AFTER REMOVE\n";
		//PrintTrain(engines);
		engines=all_engines;
		//For every train in the vector of trains
		for(unsigned int i=0; i<freight_trains.size(); i++)
		{
			if(freight_trains[i]->isEngine() == false && all_engines != NULL)
			{
				TrainCar* engine = Remove_TrainCar(all_engines);

				PushBack(freight_trains[i],engine);
			}

			//While there is enough room in the freight train
			while(count_cars(freight_trains[i])<max_cars_per_train && freight!=NULL && engines!=NULL)
			{
				//std::cout<<"should we add to\n";
				//PrintTrain(freight_trains[i]);
				//If adding another freight car would keep you under the minimum speed
				if(SpeedProjection(freight_trains[i],freight) > min_speed && freight->next != NULL)
				{
					//std::cout<<"Let's add a car!\n";
					//Remove the freight car from the list of freight trains
					TrainCar* addCar = Remove_TrainCar(freight);

					//Add it to the freight train
					PushBack(freight_trains[i],addCar);

					all_freight = freight;
				}

				//If adding another freight car would not keep you under the minimum speed
				else if(SpeedProjection(freight_trains[i],freight) <= min_speed && engines->next != NULL)
				{
					//std::cout<<"Let's add and engine!\n";
					//Remove an engine from the engines list
					TrainCar* addEngine = Remove_TrainCar(engines);
					//PrintTrain(addEngine);
					//Add it to the front of the freight train
					PushFront(freight_trains[i],addEngine);

					all_engines = engines;	
				}

				else if(SpeedProjection(freight_trains[i],freight) > min_speed && freight->next == NULL)
				{
					PushBack(freight_trains[i],freight);
					freight=NULL;
					all_freight=NULL;
				}

				else
				{
					//std::cout<<"BREAK!\n";
					break;
				}
			}
			//std::cout<<"exit while\n";
			//std::cout<<"freight remaining\n";
			//PrintTrain(freight);
			//std::cout<<"engines remaining\n";
			//PrintTrain(all_engines);
		}
	}
	return freight_trains;
}

// =======================================================================
// =======================================================================

//Helper function that counts the length of a train
int count_cars(TrainCar* &head)
{
	int count = 0;

	TrainCar* counter = head;

	//While you haven't reached the last train car
	while(counter->next != NULL)
	{
		//Increment count
		count++;

		counter = counter->next;
	}

	//Account for last train car
	count += 1;

	return count;
}

// =======================================================================
// =======================================================================

//Function that removes a singlar train car from the train
//It takes in a pointer to the train car you want to remove
//It returns the detached train car that you removed
//Your list is modified because the pointer is passed by reference
TrainCar* Remove_TrainCar(TrainCar* &carToRemove)
{
	//If you have an empty list
	if(carToRemove==NULL) 
	{
		return NULL;
	}

	//If the item to remove is the only item in the list
	else if(carToRemove->next == NULL && carToRemove->prev == NULL) 
	{
		return carToRemove;
	}

	//If the item to remove is the first item in the list
	else if(carToRemove->next != NULL && carToRemove->prev == NULL) 
	{
		//Make a new pointer to the second car in the list
		TrainCar* newHead = carToRemove->next;

		//Make it point back to NULL
		newHead->prev = NULL; 

		//Detach the car to remove so that it no longer points to the second car
		carToRemove->next =NULL; 

		//Make another pointer to point to our newly detached train car
		TrainCar* returnCar = carToRemove; 

		//Move the pointer being passed by reference from the detached car to 
		//	the new head of the list
		carToRemove = newHead;

		//Return the pointer to the detached car
		return returnCar;
	}

	//If the item to remove is the last item in the list
	else if(carToRemove->next == NULL && carToRemove->prev != NULL)
	{
		//Make a pointer to the second to last car in the list
		TrainCar* newTail = carToRemove->prev;

		//Make the second to last pointer point to NULL 
		newTail->next = NULL;

		//Detach the car to remove so that it no longer points back to newTail
		carToRemove->prev = NULL;

		//Make another pointer that points to our newly detached train car
		TrainCar* returnCar = carToRemove;

		//Move the pointer being passed by reference from the detached car
		//	to the new head of the list
		carToRemove = newTail;

		//Return the pointer to the detached car
		return returnCar;
	}

	//If the item to remove is in the middle of the list
	else if(carToRemove->next != NULL && carToRemove->prev != NULL)
	{
		//Make a pointer to the car after the one you want to remove
		TrainCar* newNext = carToRemove->next;

		//Make a pointer to the car before the one you want to remove
		TrainCar* newPrev = carToRemove->prev;

		//Detach the train car from the list
		carToRemove->prev = NULL;
		carToRemove->next = NULL;

		//Make the car to the right point back to the car to the left
		// instead of the removed car
		newNext->prev = newPrev;

		//Make the car to the left point toward the car to the right
		//	instead of the removed car
		newPrev->next = newNext;

		//Make another pointer that points to our newly detached train car
		TrainCar* returnCar = carToRemove;

		//Move the pointer being passed by reference to the car to the right 
		//	of the detached car
		carToRemove = newNext;

		//Return the pointer to the detached car
		return returnCar;
	}

	//There should be no other cases, but return NULL to be safe
	else
	{
		return NULL;
	}
}

// =======================================================================
// =======================================================================

//Sometimes empty train heads point to garbage and do not evaluate as NULL when they should
//This helper function tests if there are actually train cars in the list
bool isItNull(TrainCar* &Train)
{
	if(Train==NULL)
	{
		return true;
	}
	else
	{
		TrainCar T = *Train; 
		
	 	if (T.isEngine() || T.isFreightCar() || 
	 		T.isPassengerCar() || T.isDiningCar() ||
	 		T.isSleepingCar())
	 	{
	 		return false;
	 	}

	 	else
	 	{
	 		//std::cout<<"NULL ID="<<Train->getID()<<"\n";
	 		return true;
	 	}
	}
}

//Helper function that returns a pointer to the next non-engine train car
//RECURSION
TrainCar* getNextNonTrain(TrainCar* &trainy)
{
	if(trainy->isEngine() && !isItNull(trainy->next))
	{
		return getNextNonTrain(trainy->next);
	}
	else
	{
		return trainy;
	}
}

//Function that takes in a list of traincars 
//	and separates the list into two trains of ~equal speed
//The two resulting trains are passed by reference
void Separate(TrainCar* &train1, TrainCar* &train2, TrainCar* &train3)
{
	int train1Speed = 0;

	//If you have not yet added anything to train2
	train2=NULL;
	train3=NULL;
	if(isItNull(train2) == true && isItNull(train1) == false)
	{
		TrainCar* it = train1;

		//Find and save its original speed
		train1Speed = CalculateSpeed(it);

		bool found_engine = false;

		//While you have not reached the last train car and 
		//	while you have not yet found an engine
		while(it->next != NULL && found_engine == false)
		{
			//If you found an engine
			if(it->isEngine())
			{
				//Quit iterating
				found_engine = true;
			}

			//If you found an engine in this spot
			if(found_engine == true)
			{
				//If the engine is the first train car
				if(it->prev == NULL)
				{
					//Remove the engine from train1
					TrainCar* engine = Remove_TrainCar(train1);

					//Add the engine to train2
					if(train2!=NULL)
					{
						PushBack(train2, engine);
					}

					else
					{
						train2=engine;
					}

					//Make sure it is pointing to beginning of train
					it = train1;

					//Make another iterator for adding more train cars to train2
					TrainCar* itnew = it;

					bool found_second_engine = false;

					int second_engine_ID = 0;

					//While you have not reached the last train car
					while(itnew->next != NULL)
					{
						//If you find a second engine, skip over it
						if(itnew->isEngine() && found_second_engine == false)
						{
							second_engine_ID = itnew->getID();
							found_second_engine = true;
							itnew = itnew->next;
						}

						//Temp is the new iterator you're using which skips over a second engine
						TrainCar* temp = itnew;

						//If adding a car does not make train2 slower than the original
						//And if you are on the engine meant for train3
						if(temp->getID() == second_engine_ID && SpeedProjection(train2,train1->next) > train1Speed)
						{
							TrainCar* nextcar = it;

							nextcar = Remove_TrainCar(train1->next);

							//And add it to train2
							PushBack(train2,nextcar);

							//Iterate over that spot, which is now NULL space
							temp = train1;
							itnew = train1;

							itnew = itnew->next;
						}

						//If adding a car does not make train2 slower than the original
						//And if you are not on the engine meant for train3
						else if(temp->getID() != second_engine_ID && SpeedProjection(train2,train1->next) > train1Speed)
						{
							TrainCar* nextcar = it;

							nextcar = Remove_TrainCar(train1);

							//And add it to train2
							PushBack(train2,nextcar);

							//Iterate over that spot, which is now NULL space
							temp = train1;
							itnew = train1;

							itnew = itnew->next;
						}

						//Once you start hitting cars that cause us to go under the speed
						//	minimum, just stop looping
						if(temp->getID() != second_engine_ID && SpeedProjection(train2,itnew) < train1Speed)
						{
							break;
						}

						//Once you start hitting cars that cause us to go under the speed
						//	minimum, just stop looping
						if(temp->getID() == second_engine_ID && SpeedProjection(train2,itnew->next) < train1Speed)
						{
							break;
						}
					}

					//If you somehow reached the last train car
					//	without breaking our speed limit of train2
					if(itnew->next == NULL && CalculateSpeed(train2) > train1Speed && itnew->isEngine() == false)
					{
						//Remove the last train car from train1
						TrainCar* lasttraincar = Remove_TrainCar(train1);

						PushBack(train2,lasttraincar);
					}
				}

				//Otherwise, if the engine is not the first in the train
				else if(it->prev != NULL)
				{
					//Get the ID number of the engine
					int engineID = it->getID();

					//Remove the engine from train1
					TrainCar* ourengine = Remove_TrainCar(it);


					if(train2==NULL)
					{
						train2=ourengine;
					}
					else
					{
						PushBack(train2,ourengine);
					}

					//Make a new iterator starting at the first car
					//	of train1
					TrainCar* itnew = train1;

					int second_engine_ID = 0;

					bool found_second_engine = false;

					//While you haven't reached the last train car
					while(itnew->next != NULL)
					{
						//If you find an engine
						if(itnew->isEngine() && found_second_engine == false)
						{
							//Make note of its location
							second_engine_ID = itnew->getID();

							found_second_engine = true;
								
							//Skip over it
							itnew = itnew->next;
						}

						//If adding a car does not make train2 slower than the original
						if(SpeedProjection(train2,itnew) > train1Speed)
						{
							//If the car is located after the original engine
							if(itnew->getID() > engineID)
							{
								TrainCar * nexttraincar = it;

								if(itnew->getID() == second_engine_ID && SpeedProjection(train2,itnew->next) > train1Speed)
								{
									//Remove the next train car from the train
									nexttraincar = Remove_TrainCar(train1->next);
								}

								else
								{
									//Remove the next train car from the train
									nexttraincar = Remove_TrainCar(train1);
								}

								//Add it to the end of train2
								PushBack(train2,nexttraincar);

								//Keep iterating
								itnew = train1;
							}

							//If the train car is located before the engine
							if(itnew->getID() < engineID)
							{
								//Remove the train car we're on from the train
								TrainCar* nexttraincar = Remove_TrainCar(train1);

								//Start at beginning of train 2
								TrainCar* addit = train2;

								//If train2 only has the engine in it
								if(addit->next == NULL)
								{
									PushFront(train2,nexttraincar);
								}

								//If train2 has at least one additional car in it
								if(addit->next != NULL)
								{
									bool found_engine = false;

									//While you haven't reached the last car
									while(addit->next != NULL)
									{
										//If you find the engine car
										if(addit->getID() == engineID)
										{
											found_engine = true;
										}

										if(found_engine == true)
										{
											//Make the car before it equal to temp
											TrainCar* temp = addit->prev;

											//Add the new train car
											nexttraincar->next = addit;
											nexttraincar->prev = temp;

											addit->prev = nexttraincar;
											temp->next = nexttraincar;
										}

										addit = addit->next;
									}

									//If you reach the end of train2 and still haven't found the engine
									if(addit->next == NULL && found_engine == false)
									{
										//If the last car is the engine
										if(addit->isEngine())
										{
											//Make the car before it equal to temp
											TrainCar* temp = addit->prev;

											//Add the new train car
											nexttraincar->next = addit;
											nexttraincar->prev = temp;

											addit->prev = nexttraincar;
											temp->next = nexttraincar;
										}
									}
								}
							}
						}

						if(SpeedProjection(train2,itnew) < train1Speed)
						{
							break;
						}
					}

				}
			}

			//Keep iterating until you find an engine
			it = it->next;
		}
	}

	//If you've already put cars in train2, but not in train3
	//	basically take the same steps we took for adding cars to train2
	//	except add the cars to train3 instead

	if(isItNull(train2) == false && isItNull(train1) == false)
	{

		TrainCar* it = train1;

		//While you have not yet reached the last train car
		//	and while you have not already encountered an engine
		while(it->next != NULL)
		{
			//Remove the car from train1
			TrainCar* detachcar = Remove_TrainCar(train1);

			//And add it to train3
			if(train3==NULL)
			{
				train3=detachcar;
			}
			else
			{
				PushBack(train3,detachcar);
			}

			it = train1;
		}

		//When you get to the last train car
		if(it->next == NULL)
		{
			//Remove the last car from train1
			TrainCar* detachcar = Remove_TrainCar(train1);

			//And add it to train3
			if(train3==NULL)
			{
				train3=detachcar;
			}
			else
			{
				PushBack(train3,detachcar);
			}
		}

		train1 = NULL;
	}
}

// =======================================================================
// =======================================================================