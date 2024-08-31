#include <stdio.h>
#include <stdlib.h>
#include "laps.h"

// A list to store all cars in race
car* carlist = NULL;
size_t carsCount = 0;


// Function to ro add a new car the list
void add_car(int car_id)
{
	car* newlist = realloc(carlist, (carsCount +1)* sizeof(car));
	if (!newlist)
	{
		perror("Failed to allocate memory");
		exit(1);
	}
	carlist = newlist;


	carlist[carsCount].id = car_id;
	carlist[carsCount].lap = 0;
	carsCount++;

	printf("Car %d joined the race\n", car_id);
}

car *find_car(int car_id)
{
	for (size_t i = 0; i < carsCount; i++)
	{
		if (carlist[i].id == car_id)
		{
			return &carlist[i];
		}
	}
	return (NULL);
}

void print_car_state()
{
	printf("Race state:\n");
	for (size_t i = 0; i < carsCount; i++)
	{
		printf("Car %d [%d laps]\n", carlist[i].id, carlist[i].lap);
	}
}

void race_state(int* id, size_t size)
{
	if (size == 0)
	{
		free(carlist);
		carlist = NULL;
		carsCount = 0;
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		int car_id = id[i];
		car* car = find_car(car_id);

		if (car != NULL)
		{
			car->lap += 1;
		}
		else
		{
			add_car(car_id);
		}
	}

	for (size_t i = 0; i < carsCount - 1; i++)
	{
		for (size_t j = i + 1; j < carsCount; j++)
		{
			if (carlist[i].id > carlist[j].id)
			{
				car temp = carlist[i];
				carlist[i] = carlist[j + 1];
				carlist[j + 1] = temp;
			}
		}
	}
	
	print_car_state();
}
