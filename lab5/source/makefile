COMP = g++
FLAGS = -fopenmp -O0 -Wall -lpthread -o
#FLAGS = -O0 -Wall -g -pthread -o 
OBJS = main.cpp foo.cpp

.PHONY: clean run all

all: main

main:
	$(COMP) $(OBJS) $(FLAGS) main.exe
#-lm
