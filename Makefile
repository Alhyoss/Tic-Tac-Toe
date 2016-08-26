#Define special targets
.PHONY: clean, mrproper

#Desable implicit rules
.SUFFIXES:

#Variables
GXX = g++
CXXFLAGS = -std=c++11 -W -Wall -g
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

#Creates the executable
all: tictactoe.o
	$(GXX) tictactoe.o -o tictactoe $(SFMLFLAGS)
	$(MAKE) clean
	./tictactoe

#Builds dependecies
tictactoe.o: tictactoe.cpp
	$(GXX) -c tictactoe.cpp -o tictactoe.o $(CXXFLAGS)

#Removes temporary files
clean:
	rm -rf *.o

#Removes all files (except the sources) in order to rebuild
mrproper: clean
	rm -rf tictactoe
