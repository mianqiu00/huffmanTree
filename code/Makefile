# Makefile for compiling huffman.c

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -O2

# Source file
SOURCE = huffman.c

# Output executable
OUTPUT = huffman

# Default target
all: $(OUTPUT)

# Rule to build the executable
$(OUTPUT): $(SOURCE)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCE)

# Clean target
clean:
	rm -f $(OUTPUT)
