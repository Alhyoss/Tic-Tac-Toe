#include "bot.hpp"
#include <iostream>
/*
 * When we create the bot, we want the player to chose the level of it.
 * Therefore we call the showBotMenu function wich will ask the player to do so.
 */
Bot::Bot(sf::RenderWindow &window) {
    this->window = &window;
    font.loadFromFile("Font/arial.ttf");
    for(unsigned i=0; i < 3; i++)
        inButton[i] = false;
    level = 0;
    turn = 1;
    srand(time(NULL));
    showBotMenu();
}

/*
 * A function to set some text as we want. It the same as in the MainMenu class.
 */
void Bot::setText(std::string textStr, sf::Text &text, sf::Color color,
                   sf::Vector2f scale, sf::Vector2f pos) {
   text.setString(textStr);
   text.setFont(font);
   text.setColor(color);
   text.setScale(scale);
   text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
   text.setPosition(pos);
}

/*
 * This function sets the buttons of the bot menu. It is almost the same as
 * as the setButtons function in the MainMenu class.
 */
void Bot::setButtons(){
    //The label of each button
    std::string buttonNames[3] = {"Easy", "Medium",
                                            "Hard"};
    sf::Color color;
    //For each button
    for(unsigned i=0; i < 3; i++) {
        //We set the text of the ith button
        setText(buttonNames[i], buttonTexts[i], sf::Color::White, sf::Vector2f(1, 1),
                sf::Vector2f(window->getSize().x/2, (3+i)*window->getSize().y/7-10));
        //We set the ith button
        buttons[i].setFillColor(sf::Color::Black);
        buttons[i].setOutlineThickness(5);
        buttons[i].setOutlineColor(color);
        buttons[i].setSize(sf::Vector2f(250, 60));
        buttons[i].setOrigin(buttons[i].getSize().x/2, buttons[i].getSize().y/2);
        buttons[i].setPosition(sf::Vector2f(window->getSize().x/2,
                            (3+i)*window->getSize().y/7));
    }
}

/*
 * This function check whether or not the mouse is in the button given in argument.
 * It is the same function as the MainMenu button.
 */
bool Bot::mouseInButton(sf::RectangleShape &button) {
   //The mouse position
   sf::Vector2i mouse = sf::Mouse::getPosition(*window);
   //We convert the mouse position in the coordinates of the window
   sf::Vector2f mousePos = window->mapPixelToCoords(mouse);
   //Information about the button
   sf::Vector2f buttonSize = button.getSize();
   sf::Vector2f buttonPos = button.getPosition();
   //We check if the mouse is in the button
   if(mousePos.x >= buttonPos.x-buttonSize.x/2 && mousePos.x <= buttonPos.x+buttonSize.x/2 &&
       mousePos.y >= buttonPos.y-buttonSize.y/2 && mousePos.y <= buttonPos.y+buttonSize.y/2)
       return true;
   else
       return false;
}

/*
 * This function check the mouse position and call the mouseInButton function.
 * Once again, it is the same function as the one in the MainMenu class.
 */
void Bot::mousePosition() {
   for(unsigned i=0; i < 3; i++) {
       //We check if the mouse is in the ith button
       inButton[i] = mouseInButton(buttons[i]);
       //If it is, we highlight the button
       if(inButton[i]) {
           buttons[i].setScale(1.1, 1.1);
           buttons[i].setOutlineColor(sf::Color::Yellow);
           buttonTexts[i].setScale(1.2, 1.2);
           buttonTexts[i].setColor(sf::Color::Yellow);
       }
       //If not, we  set back the button to normal (in case it was highlighted before)
       else {
           buttons[i].setScale(1, 1);
           buttons[i].setOutlineColor(sf::Color::White);
           buttonTexts[i].setColor(sf::Color::White);
           buttonTexts[i].setScale(1, 1);
       }
   }
}

/*
 * This function handle all the events that occured in one loop of the program.
 */
void Bot::handleEvents() {
    sf::Event event;
    while(window->pollEvent(event)) {
        //If we click on the cross of the window
        if(event.type == sf::Event::Closed)
            window->close();
        else if(event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                //If we click on a button, we update the level of the ia
                if(inButton[0]) //Easy
                    level = 1;
                else if(inButton[1]) //Medium
                    level = 2;
                else if(inButton[2]) //Hard
                    level = 3;
            }
        }
    }
}

/*
 * This function called by the constructor of the class shows the bot menu in
 * which the player has to chose the level of the ia.
 */
void Bot::showBotMenu() {
    //We create the "title" and the buttons.
    setText("One player", title, sf::Color::White, sf::Vector2f(2, 2),
            sf::Vector2f(window->getSize().x/2, window->getSize().y/6));
    setButtons();

    while(window->isOpen() && level == 0) {
        handleEvents();
        mousePosition();
        //We update what has to be drawn.
        window->clear();
        window->draw(title);
        for(sf::RectangleShape box : buttons)
            window->draw(box);
        for(sf::Text text : buttonTexts)
            window->draw(text);
        window->display();
    }
}

/*
 * This function is called in the main function if we are in single player mode.
 * It call the appropriate function to play, depending on the level of the ia.
 */
int Bot::play(int board[3][3]) {
    if(level == 1)
        return playEasy(board);
    else if(level == 2)
        return playMedium(board);
    else
        return playHard(board);
}

/*
 * This function is called by the play function if the ia level is easy.
 * It's rather simple: the "ia" simply pick a random number between 0 and 9,
 * check if this case on the board has already been played, and play there if not.
 */
int Bot::playEasy(int board[3][3]) {
    int i;
    do {
        i = rand()%9;
    } while(board[i%3][i/3] != 0);
    return i;
}

int Bot::searchDouble(int board[3][3], int player) {
    for(unsigned i=0; i < 3; i++) {
        for(unsigned j=0; j < 3; j++) {
            if(board[j][i] == player) {
                if(j == 0) {
                    if(board[j+1][i] == player) {
                        if(board[j+2][i] == 0)
                            return i*3+j+2;
                    }
                    else if(board[j+2][i] == player){
                        if(board[j+1][i] == 0)
                            return i*3+j+1;
                    }
                }
                else if(j == 1) {
                    if(board[j-1][i] == player){
                        if(board[j+1][i] == 0)
                            return i*3+j+1;
                    }
                    else if(board[j+1][i] == player){
                        if(board[j-1][i] == 0)
                            return i*3+j-1;
                    }
                }
                else if(j == 2) {
                    if(board[j-1][i] == player){
                        if(board[j-2][i] == 0)
                            return i*3+j-2;
                    }
                    if(board[j-2][i] == player){
                        if(board[j-1][i] == 0)
                            return i*3+j-1;
                    }
                }

                if(i == 0) {
                    if(board[j][i+1] == player){
                        if(board[j][i+2] == 0)
                            return (i+2)*3+j;
                    }
                    else if(board[j][i+2] == player){
                        if(board[j][i+1] == 0)
                            return (i+1)*3+j;
                    }
                }
                else if(i == 1) {
                    if(board[j][i-1] == player){
                        if(board[j][i+1] == 0)
                            return (i+1)*3+j;
                    }
                    else if(board[j][i+1] == player){
                        if(board[j][i-1] == 0)
                            return (i-1)*3+j;
                    }
                }
                else if(i == 2) {
                    if(board[j][i-1] == player){
                        if(board[j][i-2] == 0)
                            return (i-2)*3+j;
                    }
                    if(board[j][i-2] == player){
                        if(board[j][i-1] == 0)
                            return (i-1)*3+j;
                    }
                }

                if(j == 0 && i == 0) {
                    if(board[1][1] == player) {
                        if(board[2][2] == 0)
                            return 8;
                    }
                    else if(board[2][2] == player) {
                        if(board[1][1] == 0)
                            return 4;
                    }
                }
                else if(j == 1 && i == 1) {
                    if(board[2][2] == player) {
                        if(board[0][0] == 0)
                            return 0;
                    }
                    else if(board[0][0] == player) {
                        if(board[2][2] == 0)
                            return 8;
                    }
                }
                else if(j == 2 && i == 2) {
                    if(board[1][1] == player) {
                        if(board[0][0] == 0)
                            return 0;
                    }
                    else if(board[0][0] == player) {
                        if(board[1][1] == 0)
                            return 4;
                    }
                }
                else if(j == 2 && i == 0) {
                    if(board[1][1] == player) {
                        if(board[0][2] == 0)
                            return 6;
                    }
                    else if(board[0][2] == player) {
                        if(board[1][1] == 0)
                            return 4;
                    }
                }
                else if(j == 0 && i == 2) {
                    if(board[1][1] == player) {
                        if(board[2][0] == 0)
                            return 2;
                    }
                    else if(board[0][2] == player) {
                        if(board[1][1] == 0)
                            return 4;
                    }
                }
            }
        }
    }
    return -1;
}

int Bot::block(int board[3][3]) {
    return searchDouble(board, 1);
}

int Bot::tryWin(int board[3][3]) {
    return searchDouble(board, -1);
}

/*
 * This function is called by the play function if the ia level is medium.
 * It is yet to be implemented.
 */
int Bot::playMedium(int board[3][3]) {
    int i = -1;
    if(turn == 1 && board[1][1] == 0)
        i = 4;
    else if(turn == 1)
        i = rand()%4*2+1;
    else
        i = tryWin(board);
    if(i == -1)
        i = block(board);
    if(i == -1) {
        do {
            i = rand()%9;
        } while(board[i%3][i/3] != 0);
    }
    turn++;
    return i;
}

/*
 * This function is called by the play function if the ia level is hard.
 * This function is yet to be implemented.
 */
int Bot::playHard(int board[3][3]) {
    int i = -1;
    if(turn == 1 && board[1][1] == 0)
        i = 4;
    else if(turn == 1) {
        i = rand()%4;
        if(i == 0 || i == 1)
            i *= 2;
        else
            i = i*2 + 2;
    }
    else
        i = tryWin(board);
    if(i == -1)
        i = block(board);
    if(i == -1) {
        do {
            i = rand()%4;
            if(i == 0 || i == 1)
                i *= 2;
            else
                i = i*2 + 2;
        } while(board[i%3][i/3] != 0);
    }
    turn++;
    return i;
}

Bot::~Bot() {}
