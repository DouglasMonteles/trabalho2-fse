# Executavel
BINFOLDER := bin/
# .hpp
INCFOLDER := inc/
# .cpp
SRCFOLDER := src/
# .o
OBJFOLDER := obj/
CC := gcc
CFLAGS := -std=c99
LINKFLAGS := -lrt -lbcm2835 -lwiringPi -pthread
SRCFILES := $(wildcard src/*.c)

all: $(SRCFILES:src/%.c=obj/%.o)
	$(CC) $(CFLAGS) obj/*.o -o bin/main $(LINKFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I./inc $(LINKFLAGS)

run: bin/main
	bin/main

.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf bin/*
	touch obj/.gitkeep
	touch bin/.gitkeep

build:
	make clean
	make 
	make run