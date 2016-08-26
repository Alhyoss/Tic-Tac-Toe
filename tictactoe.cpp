#include <iostream>
#include <vector>
#include <string>

#include "MainMenu/mainmenu.hpp"

void handleEvents(sf::RenderWindow &window, MainMenu *menu, unsigned &state) {
    sf::Event event;
    while(window.pollEvent(event)) {
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Escape)
                state = menu->showMenu();
        }
        else if(event.type == sf::Event::Closed)
            window.close();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "Tic Tac Toe");

    MainMenu *menu = new MainMenu(window);
    unsigned gameState = menu->showMenu();

    while(window.isOpen()) {
        menu->setGame(true);
        handleEvents(window, menu, gameState);
        window.clear();
        window.display();
    }

    delete menu;
    return 0;
}
