# Makefile

CC = gcc
CFLAGS = -Wall -Werror -g
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lelf

TARGET = hnm
SRC = hnm.c
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c hnm.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean

