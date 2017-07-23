CC = gcc
CC_FLAGS = -Wall -O2 -std=c99
PROGRAM_NAME = gauss
LINKER = -fopenmp -lm

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(PROGRAM_NAME).c
	$(CC) $(CC_FLAGS) -o $@ $< $(LINKER)

clear:
	rm $(PROGRAM_NAME)

rebuild:
	$(MAKE) clear
	$(MAKE)
