#ifndef LAPS_H
#define LAPS_H

#include <stddef.h>

typedef struct {
    int id;
    int lap;
} Car;

void race_state(int *id, size_t size);

#endif /* LAPS_H */
