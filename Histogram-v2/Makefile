CC = gcc
CCFLAGS = -Wall -O2
LINKER = -fopenmp
PROGRAM_NAME = paralelo

$(PROGRAM_NAME): $(PROGRAM_NAME).c
	$(CC) $(CCFLAGS) -o $@ $< $(LINKER)

clean:
	rm -r $(PROGRAM_NAME)
