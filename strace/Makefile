CC = gcc
CFLAGS = -Wall -Wextra -pedantic
SRC = strace_0.c
TARGET = strace_0

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

