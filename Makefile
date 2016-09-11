#Define special targets
.PHONY: clean, mrproper

#Disable implicit rules
.SUFFIXES:

#Variables
GXX = g++
CXXFLAGS = -std=c++11 -W -Wall -g
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

#Creates the executable
all: mainmenu.o bot.o tictactoe.o
	$(GXX) mainmenu.o bot.o tictactoe.o -o tictactoe $(SFMLFLAGS)
	$(MAKE) clean
	./tictactoe

#Builds dependecies
mainmenu.o: MainMenu/mainmenu.cpp
	$(GXX) -c MainMenu/mainmenu.cpp -o mainmenu.o $(CXXFLAGS)

bot.o: ArtificialIntelligence/bot.cpp
	$(GXX) -c ArtificialIntelligence/bot.cpp -o bot.o $(CXXFLAGS)

tictactoe.o: tictactoe.cpp
	$(GXX) -c tictactoe.cpp -o tictactoe.o $(CXXFLAGS)

#Removes temporary files
clean:
	rm -rf *.o

#Removes all files (except the sources) in order to rebuild
mrproper: clean
	rm -rf tictactoe
