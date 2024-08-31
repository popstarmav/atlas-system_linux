#include <stdio.h>
#include <stdlib.h>
#include "laps.h"

static Car *cars = NULL;  // Dynamic array to hold the cars
static int car_count = 0; // Number of cars in the race

// Function to manage race state
void race_state(int *id, size_t size) {
    for (size_t i = 0; i < size; i++) {
        int found = 0;

        // Check if car is already in the race
        for (int j = 0; j < car_count; j++) {
            if (cars[j].id == id[i]) {
                cars[j].lap++; // Increment laps if car is found
                found = 1;
                break;
            }
        }

        // If car is not found, add it to the race
        if (!found) {
            // Reallocate memory to add new car
            cars = realloc(cars, (car_count + 1) * sizeof(Car));
            if (cars == NULL) {
                perror("Unable to allocate memory");
                exit(EXIT_FAILURE);
            }

            // Initialize the new car's data
            cars[car_count].id = id[i];
            cars[car_count].lap = 0;
            car_count++;
            printf("Car %d joined the race\n", id[i]);
        }
    }

    // Sort the cars by their id for display purposes
    for (int i = 0; i < car_count - 1; i++) {
        for (int j = i + 1; j < car_count; j++) {
            if (cars[i].id > cars[j].id) {
                Car temp = cars[i];
                cars[i] = cars[j];
                cars[j] = temp;
            }
        }
    }

    // Print the current state of the race
    printf("Race state:\n");
    for (int k = 0; k < car_count; k++) {
        printf("Car %d [%d laps]\n", cars[k].id, cars[k].lap);
    }
    printf("--\n");
}