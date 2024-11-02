# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source and header files
SRC = board.c pingPong.c
OBJ = $(SRC:.c=.o)
TARGET = program

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile .c files into .o files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

