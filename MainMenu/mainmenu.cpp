#include "mainmenu.hpp"

/*
 * Here we initialize the menu and what has to be shown in it.
 * Window is the window in which the menu is displayed
 */
MainMenu::MainMenu(sf::RenderWindow &window) {
    this->window = &window;
    //The game is not yet launched
    inGame = false;
    font.loadFromFile("Font/arial.ttf");
    //Arrays for the buttons, their text and if the mouse is in it
    numberButtons = 4;
    buttons = new sf::RectangleShape[numberButtons];
    buttonTexts = new sf::Text[numberButtons];
    inButton = new bool[numberButtons];
    //The mouse is not in any box
    for(unsigned i=0; i < numberButtons; i++)
        inButton[i] = false;
}

/*
 * This function set a text with SFML, for it to be displayed later.
 * It is call for each button and for the title
 */
void MainMenu::setText(std::string textStr, sf::Text &text, sf::Color color,
                    sf::Vector2f scale, sf::Vector2f pos) {
    text.setString(textStr);
    text.setFont(font);
    text.setColor(color);
    text.setScale(scale);
    text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
    text.setPosition(pos);
}

/*
 * This function set the title that is shwon in the menu.
 * It only calls the setText function, it a bit useless, it is just there to
 * improve readability.
 */
void MainMenu::setTitle(std::string titleStr) {
    setText(titleStr, title, sf::Color::White, sf::Vector2f(3, 3),
            sf::Vector2f(window->getSize().x/2, window->getSize().y/7));
}

/*
 * This function will creates the buttons and their text, and place them in the
 * buttons and buttonTexts arrays.
 */
void MainMenu::setButtons(){
    //The label of each button
    std::string buttonNames[numberButtons] = {"Single Player", "Two Players",
                                            "Resume", "Exit"};
    sf::Color color;
    //For each button
    for(unsigned i=0; i < numberButtons; i++) {
        //If the button is clickable, it is colored in white, otherwise in grey
        if(i==0 || (!inGame && i == 2))
            color = sf::Color(94,94,94);
        else
            color = sf::Color::White;
        //We set the text of the ith button
        setText(buttonNames[i], buttonTexts[i], color, sf::Vector2f(1, 1),
                sf::Vector2f(window->getSize().x/2, (3+i)*window->getSize().y/8-10));
        //We set the ith button
        buttons[i].setFillColor(sf::Color::Black);
        buttons[i].setOutlineThickness(5);
        buttons[i].setOutlineColor(color);
        buttons[i].setSize(sf::Vector2f(250, 60));
        buttons[i].setOrigin(buttons[i].getSize().x/2, buttons[i].getSize().y/2);
        buttons[i].setPosition(sf::Vector2f(window->getSize().x/2,
                            (3+i)*window->getSize().y/8));
    }
}

/*
 * This function check whether or not the mouse is in the button given in argument
 */
bool MainMenu::mouseInButton(sf::RectangleShape &button) {
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
 * This function is called whenever the mouse moves.
 * It the call the mouseInButton for each button to see if the mouse is in one of
 * them. If it is, the button is higlighted.
 */
void MainMenu::mousePosition() {
    for(unsigned i=0; i < numberButtons; i++) {
        //We check if the mouse is in the ith button
        inButton[i] = mouseInButton(buttons[i]);
        //If it is, we highlight the button
        if(i != 0 && !(!inGame && i == 2) && inButton[i]) {
            buttons[i].setScale(1.1, 1.1);
            buttons[i].setOutlineColor(sf::Color::Yellow);
            buttonTexts[i].setScale(1.2, 1.2);
            buttonTexts[i].setColor(sf::Color::Yellow);
        }
        //If not, we  set back the button to normal (in case it was highlighted before)
        else if(i != 0 && !(!inGame && i == 2)) {
            buttons[i].setScale(1, 1);
            buttons[i].setOutlineColor(sf::Color::White);
            buttonTexts[i].setColor(sf::Color::White);
            buttonTexts[i].setScale(1, 1);
        }
    }
}

/*
 * This function handle all the events thrown by SFML, such as a click on the
 * mouse.
 */
void MainMenu::handleEvents() {
    sf::Event event;
    while(window->pollEvent(event)) {
        //If we close the window
        if(event.type == sf::Event::Closed)
            window->close();
        //If we press escape
        else if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Escape)
                state = RESUME;
        }
        //If we left click with the mouse
        else if(event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                //If we click on a button, we set update the state of the menu
                if(inButton[1]) //Play two players
                    state = TWOPLAYERS;
                else if(inButton[2] && inGame)
                    state = RESUME;
                else if(inButton[3]) //Exit
                    state = EXIT;
            }
        }
    }
}

/*
 * In this function, the mainloop of SFML shows the window and displays all
 * the buttons.
 * It initialize the buttons, the title and then draw them.
 */
unsigned MainMenu::showMenu() {
    //We initialize the title and the buttons
    setTitle("Tic Tac Toe");
    setButtons();
    //The state is set to wait
    state = WAIT;
    //While the window is open and the state of the menu is still wait
    while(window->isOpen() && state == WAIT) {
        //We handle the events and check the position of the mouse
        mousePosition();
        handleEvents();
        //We draw the buttons and the title
        window->clear();
        window->draw(title);
        for(unsigned i=0; i < numberButtons; i++) {
            window->draw(buttons[i]);
            window->draw(buttonTexts[i]);
        }
        window->display();
    }
    //If we close the window
    if(state == EXIT && window->isOpen())
        window->close();
    return state;
}

MainMenu::~MainMenu() {
    delete [] buttons;
    delete [] buttonTexts;
    delete [] inButton;
}
