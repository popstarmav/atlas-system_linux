#ifndef LAPS_H
#define LAPS_H

typedef struct car {
    int id;
    int lap;
} car

void race_state(int *id, size_t size);

#endif /* LAPS_H */
