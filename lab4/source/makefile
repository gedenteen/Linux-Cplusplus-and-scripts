COMP = g++
FLAGS = -O0 -Wall -g -o
#FLAGS = -O0 -pg -Wall -g -o
OBJS = main.cpp foo.cpp

.PHONY: clean run all

all: main

main:
	$(COMP) $(OBJS) $(FLAGS) main.exe -lm

#all:
#	gcc main.cpp -Wall -o main.exe -lm
