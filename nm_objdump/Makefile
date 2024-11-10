CC = gcc
CFLAGS = -Wall -Werror -g
LDFLAGS = -lelf  # Add the libelf library here

OBJS = hnm_elf_files.o
TARGET = hnm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

