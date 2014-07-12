# Makefile, 2014, Tomas Dejmek 

# settings
CXX=g++
CFLAGS=-Wall -pedantic -std=c++11
OBJ=main

# rules 

all: $(OBJ).cpp
	$(CXX) $(OBJ).cpp $(CFLAGS) -o $(OBJ).exe

run: $(OBJ).exe
	./$(OBJ).exe
