#ifndef _MAINMENU_HPP_
#define _MAIN_MENU_HPP_ 1

#include <SFML/Graphics.hpp>

//Some useful macro
#define WAIT 0
#define ONEPLAYER 1
#define TWOPLAYERS 2
#define RESUME 3
#define EXIT 4

/*
 * This class is the Main menu of the game.
 * It is shown when the program is launched and the player press Escape.
 */
class MainMenu {
    public:
        //window is the window in which the menu has to be shown
        explicit MainMenu(sf::RenderWindow &window);
        //This function creates the buttons and draw them in the window
        unsigned showMenu();
        //The inGame variable is useful to know if the game is on.
        //If it is, the "Resume" button is clickable.
        void setGame(bool game) {inGame = game;}
        ~MainMenu();

    private:
        //Function to write a given string in the window
        void setText(std::string textStr, sf::Text &text, sf::Color color,
                    sf::Vector2f scale, sf::Vector2f pos);
        //This function only call the setText function. It is only for readability
        void setTitle(std::string title);
        //This function creates all the buttons of the main menu
        void setButtons();
        //This function check if the mouse is in a given button
        bool mouseInButton(sf::RectangleShape &button);
        //This function highlight the button in which the mouse is
        void mousePosition();
        //Handle all events, such as click on the close button of the window,
        //or a click in a button
        void handleEvents();


    private:
        //The window of the program
        sf::RenderWindow *window;
        //Boolean to know if the game is on
        bool inGame;

        //The number of buttons, the shape of the buttons and their label
        unsigned numberButtons;
        sf::RectangleShape *buttons;
        sf::Text *buttonTexts;
        //Array of boolean to know if we are in a button
        bool *inButton;

        //Font of all the texts
        sf::Font font;
        //The title to be drawn
        sf::Text title;

        //The state of the menu (Wait, one player, two players, resume or exit)
        unsigned state;
};

#endif
