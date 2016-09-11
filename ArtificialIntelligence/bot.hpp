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
        int play(int board[3][3]);
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
        int playEasy(int board[3][3]);
        int playMedium(int board[3][3]);
        int playHard(int board[3][3]);

        int searchDouble(int board[3][3], int player);
        int block(int board[3][3]);
        int tryWin(int board[3][3]);

        sf::RenderWindow *window;
        //Level of the ia (easy, medium or hard)
        int level;
        //The turn of the game
        int turn;
        //Once again, those member variables are copied and pasted from the
        //MainMenu class
        sf::Font font;
        sf::Text title;
        sf::RectangleShape buttons[3];
        sf::Text buttonTexts[3];
        bool inButton[3];
};

#endif
