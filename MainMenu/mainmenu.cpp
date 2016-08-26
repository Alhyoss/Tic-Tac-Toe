#include "mainmenu.hpp"

MainMenu::MainMenu(sf::RenderWindow &window) {
    this->window = &window;
    inGame = false;
    font.loadFromFile("Font/arial.ttf");
    numberButtons = 4;
    buttons = new sf::RectangleShape[numberButtons];
    buttonTexts = new sf::Text[numberButtons];
    inButton = new bool[numberButtons];
    for(unsigned i=0; i < numberButtons; i++)
        inButton[i] = false;
}

void MainMenu::setText(std::string textStr, sf::Text &text, sf::Color color,
                    sf::Vector2f scale, sf::Vector2f pos) {
    text.setString(textStr);
    text.setFont(font);
    text.setColor(color);
    text.setScale(scale);
    text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
    text.setPosition(pos);
}

void MainMenu::setTitle(std::string titleStr) {
    setText(titleStr, title, sf::Color::White, sf::Vector2f(3, 3),
            sf::Vector2f(window->getSize().x/2, window->getSize().y/7));
}

void MainMenu::setButtons(){
    std::string buttonNames[numberButtons] = {"Single Player", "Two Players",
                                            "Resume", "Exit"};
    sf::Color color;
    for(unsigned i=0; i < numberButtons; i++) {
        if(i==0 || (!inGame && i == 2))
            color = sf::Color(94,94,94);
        else
            color = sf::Color::White;
        setText(buttonNames[i], buttonTexts[i], color, sf::Vector2f(1, 1),
                sf::Vector2f(window->getSize().x/2, (3+i)*window->getSize().y/8-10));

        buttons[i].setFillColor(sf::Color::Black);
        buttons[i].setOutlineThickness(5);
        buttons[i].setOutlineColor(color);
        buttons[i].setSize(sf::Vector2f(250, 60));
        buttons[i].setOrigin(buttons[i].getSize().x/2, buttons[i].getSize().y/2);
        buttons[i].setPosition(sf::Vector2f(window->getSize().x/2,
                            (3+i)*window->getSize().y/8));
    }
}

bool MainMenu::mouseInButton(sf::RectangleShape &button) {
    sf::Vector2i mouse = sf::Mouse::getPosition(*window);
    sf::Vector2f mousePos = window->mapPixelToCoords(mouse);
    sf::Vector2f buttonSize = button.getSize();
    sf::Vector2f buttonPos = button.getPosition();

    if(mousePos.x >= buttonPos.x-buttonSize.x/2 && mousePos.x <= buttonPos.x+buttonSize.x/2 &&
            mousePos.y >= buttonPos.y-buttonSize.y/2 && mousePos.y <= buttonPos.y+buttonSize.y/2)
        return true;
    else
        return false;
}

void MainMenu::mouseMoved() {
    for(unsigned i=0; i < numberButtons; i++) {
        inButton[i] = mouseInButton(buttons[i]);
        if(i != 0 && !(!inGame && i == 2) && inButton[i]) {
            buttons[i].setScale(1.1, 1.1);
            buttons[i].setOutlineColor(sf::Color::Yellow);
            buttonTexts[i].setScale(1.2, 1.2);
            buttonTexts[i].setColor(sf::Color::Yellow);
        }
        else if(i != 0 && !(!inGame && i == 2)) {
            buttons[i].setScale(1, 1);
            buttons[i].setOutlineColor(sf::Color::White);
            buttonTexts[i].setColor(sf::Color::White);
            buttonTexts[i].setScale(1, 1);
        }
    }
}

void MainMenu::leftMousePressed() {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if(inButton[1]) //Play two players
            state = TWOPLAYERS;
        else if(inButton[2] && inGame)
            state = RESUME;
        else if(inButton[3]) //Exit
            state = EXIT;
    }
}

unsigned MainMenu::showMenu() {
    setTitle("Tic Tac Toe");
    setButtons();
    state = WAIT;
    while(window->isOpen() && state == WAIT) {
        mouseMoved();
        leftMousePressed();
        sf::Event event;
        while(window->pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window->close();
            else if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape)
                    state = RESUME;
            }
        }

        window->clear();
        window->draw(title);
        for(unsigned i=0; i < numberButtons; i++) {
            window->draw(buttons[i]);
            window->draw(buttonTexts[i]);
        }
        window->display();
    }
    if(state == EXIT && window->isOpen())
        window->close();
    return state;
}

MainMenu::~MainMenu() {
    delete [] buttons;
    delete [] buttonTexts;
    delete [] inButton;
}
