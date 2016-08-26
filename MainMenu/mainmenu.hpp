#ifndef _MAINMENU_HPP_
#define _MAIN_MENU_HPP_ 1

#include <SFML/Graphics.hpp>

#define WAIT 0
#define ONEPLAYER 1
#define TWOPLAYERS 2
#define RESUME 3
#define EXIT 4

class MainMenu {
    public:
        explicit MainMenu(sf::RenderWindow &window);
        unsigned showMenu();
        void setGame(bool game) {inGame = game;}
        ~MainMenu();

    private:
        void setText(std::string textStr, sf::Text &text, sf::Color color,
                    sf::Vector2f scale, sf::Vector2f pos);
        void setTitle(std::string title);
        void setButtons();
        bool mouseInButton(sf::RectangleShape &button);
        void mouseMoved();
        void leftMousePressed();


    private:
        sf::RenderWindow *window;
        bool inGame;

        unsigned numberButtons;
        sf::RectangleShape *buttons;
        sf::Text *buttonTexts;
        bool *inButton;

        sf::Font font;
        sf::Text title;

        unsigned state;
};

#endif
