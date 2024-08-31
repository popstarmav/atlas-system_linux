#include <stdio.h>
#include <stdlib.h>
#include "laps.h"

static Car *cars = NULL;
static int car_count = 0;

void race_state(int *id, size_t size) {
    if (size == 0) {
        free(cars);
        cars = NULL;
        car_count = 0;
        return;
    }

    for (size_t i = 0; i < size; i++) {
        int found = 0;

        for (int j = 0; j < car_count; j++) {
            if (cars[j].id == id[i]) {
                cars[j].laps++;
                found = 1;
                break;
            }
        }

        if (!found) {
            cars = realloc(cars, (car_count + 1) * sizeof(Car));
            if (cars == NULL) {
                perror("Unable to allocate memory");
                exit(EXIT_FAILURE);
            }

            cars[car_count].id = id[i];
            cars[car_count].laps = 0;
            car_count++;
            printf("Car %d joined the race\n", id[i]);
        }
    }

    for (int i = 0; i < car_count - 1; i++) {
        for (int j = i + 1; j < car_count; j++) {
            if (cars[i].id > cars[j].id) {
                Car temp = cars[i];
                cars[i] = cars[j];
                cars[j] = temp;
            }
        }
    }

    printf("Race state:\n");
    for (int k = 0; k < car_count; k++) {
        printf("Car %d [%d laps]\n", cars[k].id, cars[k].laps);
    }
}