#ifndef _BOT_HPP_
#define _BOT_HPP_ 1

#include <cstdlib>
#include <time.h>
#include <SFML/Graphics.hpp>

/*
 * The Bot class is the artificial intelligence of the game.
 * It can be set as easy, medium or hard.
 * The only really interesting functions are playEasy(), playMedium() and playHard().
 * The rest are just graphic functions to ask the player to set an ia level.
 */
class Bot {
    public:
        Bot(sf::RenderWindow &window);
        //This function is call in the main function.
        int play(std::vector<int> &board);
        ~Bot();

    private:
        //A bunch of functions almost copied and pasted from the MainMenu class,
        //used for the player to set the level of the ia
        void setText(std::string textStr, sf::Text &text, sf::Color color,
                           sf::Vector2f scale, sf::Vector2f pos);
        void setButtons();
        bool mouseInButton(sf::RectangleShape &button);
        void mousePosition();
        void handleEvents();
        void showBotMenu();
        //Those three functions are called by the play function above
        //depending of the level of the ia (easy, medium or hard)
        int playEasy(std::vector<int> &board);
        int playMedium(std::vector<int> &board);
        int playHard(std::vector<int> &board);

        sf::RenderWindow *window;
        //Level of the ia (easy, medium or hard)
        int level;
        //Once again, those member variables are copied and pasted from the
        //MainMenu class
        sf::Font font;
        sf::Text title;
        sf::RectangleShape buttons[3];
        sf::Text buttonTexts[3];
        bool inButton[3];
};

#endif
