CC = gcc
CFLAGS = -Wextra -Werror -Wall
DEPS = marcros.h
TARGETS = writenoncanonical noncanonical
OBJ = main.o

all: $(TARGETS)

%.o: %.c $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $<

writenoncanonical: $(OBJ)
		$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm

clean:
		@rm $(TARGETS) *.o