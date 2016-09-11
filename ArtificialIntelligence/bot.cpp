#include "bot.hpp"

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
        //If the button is clickable, it is colored in white, otherwise in grey
        if(i == 1 || i == 2)
            color = sf::Color(94,94,94);
        else
            color = sf::Color::White;
        //We set the text of the ith button
        setText(buttonNames[i], buttonTexts[i], color, sf::Vector2f(1, 1),
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
       if(i != 1 && i != 2 && inButton[i]) {
           buttons[i].setScale(1.1, 1.1);
           buttons[i].setOutlineColor(sf::Color::Yellow);
           buttonTexts[i].setScale(1.2, 1.2);
           buttonTexts[i].setColor(sf::Color::Yellow);
       }
       //If not, we  set back the button to normal (in case it was highlighted before)
       else if(i != 1 && i != 2) {
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
int Bot::play(std::vector<int> &board) {
    if(level == 1)
        return playEasy(board);
    else if(level == 2)
        return playMedium(board);
    else
        return playHard(board);
}

/*
 * This function is called by the play function if the ia level is easy.
 * It rather simple: the "ia" simply pick a random number between 0 and 9,
 * check if this case on the board has already been played, and play there if not.
 */
int Bot::playEasy(std::vector<int> &board) {
    srand(time(NULL));
    int i;
    do {
        i = rand()%9;
    } while(board[i] != 0);
    return i;
}

/*
 * This function is called by the play function if the ia level is medium.
 * It is yet to be implemented.
 */
int Bot::playMedium(std::vector<int> &board) {

}

/*
 * This function is called by the play function if the ia level is hard.
 * This function is yet to be implemented.
 */
int Bot::playHard(std::vector<int> &board) {

}

Bot::~Bot() {}
