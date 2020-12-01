COMP = g++
FLAGS = -O0 -Wall -pthread -fopenmp -o
#FLAGS = -O0 -Wall -g -pthread -o 
OBJS = main.cpp foo.cpp

.PHONY: clean run all

all: main

main:
	$(COMP) $(OBJS) $(FLAGS) main.exe
#-lm
