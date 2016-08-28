#include <iostream>
#include <vector>
#include <string>

#include "MainMenu/mainmenu.hpp"

//Some useful macro
#define ONEPLAYER 1
#define TWOPLAYERS 2
#define RESUME 3
#define EXIT 4

/*
 * The square class represents the square of the board. It has coordinates
 * and a player number, which corresponds to the player that possess the square
 * (0 = no player, 1 = player 1, -1 = player 2).
 * It inherits from the RectangleShape class of SFML to be easily manipulates.
 */
class Square : public sf::RectangleShape {
    public:
        //x and y are the indexes of the square in the board.
        Square(unsigned x, unsigned y) {
            player = 0;
            this->x = x;
            this->y = y;
        }

        int getPlayer() {
            return player;
        }

        unsigned getX() {
            return x;
        }

        unsigned getY() {
            return y;
        }

        void setPlayer(int player) {
            this->player = player;
        }

        ~Square(){}

    private:
        //The player who has the square
        int player;
        //indexes on the board
        unsigned x, y;
};

/*
 * This class is a super class used to store Cross objects and Circle objects
 * together.
 * It inherits of the Drawable and Transformable classes of SFML, that allow
 * us to draw the shape and display it on the window.
 */
class Symbol : public sf::Drawable, public sf::Transformable {
    public:
        Symbol() {}

    protected:
        //This function is needed by SFML to draw the shape in the window
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            states.transform *= getTransform();
            target.draw(vertices, states);
        }

    protected:
        //This is an array that stores all the points of the shape.
        sf::VertexArray vertices;
};

/*
 * This class is the cross shape of the second player.
 * It create a cross that will be drawn in the square.
 */
class Cross : public Symbol {
    public:
        //x and y are the coordinates of the square
        Cross(unsigned x, unsigned y){
            vertices.setPrimitiveType(sf::Quads);
            vertices.resize(8);
            vertices[0] = sf::Vector2f(x+30, y+20);
            vertices[1] = sf::Vector2f(x+20, y+30);
            vertices[2] = sf::Vector2f(x+160, y+170);
            vertices[3] = sf::Vector2f(x+170, y+160);
            vertices[4] = sf::Vector2f(x+20, y+160);
            vertices[5] = sf::Vector2f(x+30, y+170);
            vertices[6] = sf::Vector2f(x+170, y+30);
            vertices[7] = sf::Vector2f(x+160, y+20);
            for(unsigned i=0; i < 8; i++)
                vertices[i].color = sf::Color::Black;
        }
};

/*
 * This class is the Circle shape of the player two.
 * It creats black circle and a second white circle inside it to look like
 * a ring.
 */
class Circle : public Symbol {
    public:
        //x and y are the positions of the square in which the circle has to be
        //drawn
        Circle(unsigned x, unsigned y){
            //Number of points by circel
            unsigned N = 100;
            vertices.setPrimitiveType(sf::TrianglesFan);
            vertices.resize((N+2)*2);
            //We create the first black circle
            vertices[0] = sf::Vector2f(x+95, y+95);
            vertices[0].color = sf::Color::White;
            for(unsigned i=1; i <= N+1; i++) {
                vertices[i] = sf::Vector2f(x+95+75*cos(2*M_PI*(i-1)/N),
                                            y+95+75*sin(2*M_PI*(i-1)/N));
                vertices[i].color = sf::Color::Black;
            }
            //We crate the second white circle
            vertices[N+2] = sf::Vector2f(x+95, y+95);
            vertices[N+2].color = sf::Color::White;
            for(unsigned i=N+3; i < (N+2)*2; i++) {
                vertices[i] = sf::Vector2f(x+95+65*cos(2*M_PI*(i-1)/N),
                                            y+95+65*sin(2*M_PI*(i-1)/N));
                vertices[i].color = sf::Color::White;
            }
        }
};

/*
 * This function initialize the game by creating a 3 by 3 board, composed of
 * squares.
 * It is called at the beginning of the program, and if we chose the start a
 * new game.
 */
void initGame(std::vector<Square*> &board) {
    //We delete the existing board if there is any
    for(Square *square : board)
        delete square;
    board.clear();
    //We creates the square and add them to the board
    Square *square;
    for(unsigned i=0; i < 3; i++) {
        for(unsigned j=0; j < 3; j++) {
            square = new Square(j, i);
            square->setSize(sf::Vector2f(190, 190));
            square->setFillColor(sf::Color::White);
            square->setOutlineThickness(5);
            square->setOutlineColor(sf::Color::Black);
            square->setPosition(j*200+105, i*200+55);
            board.push_back(square);
        }
    }
}

/*
 * This function check if the mouse is in the square given in argument.
 * It is called when the left button of the mouse is pressed.
 */
bool mouseInSquare(sf::RenderWindow &window, Square *square) {
    //Coordinate of the mouse
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    //We convert the mouse coordinates in the coordinates of all the objects
    sf::Vector2f mousePos = window.mapPixelToCoords(mouse);
    unsigned squareSize = 190;
    sf::Vector2f squarePosition = square->getPosition();

    //We check if the mouse is inside the square
    if(mousePos.x >= squarePosition.x && mousePos.x <= squarePosition.x + squareSize &&
        mousePos.y >= squarePosition.y && mousePos.y <= squarePosition.y + squareSize)
        return true;
    else
        return false;
}

/*
 * In this function we assign a square of the board to a player, if he click on it.
 * It can't be reassigned.
 * It also creates the symbol (cross or circle) that will be displayed on this
 * square.
 */
void givePlayer(Square *square, int &player, std::vector<Symbol*> &symbols) {
    //If not already assigned
    if(square->getPlayer() == 0) {
        Symbol *symbol;
        //We assigned it to the player
        square->setPlayer(player);
        //We create the symbol, depending on which player has the square
        if(player == 1)
            symbol = new Cross(square->getPosition().x, square->getPosition().y);
        else if(player == -1)
            symbol = new Circle(square->getPosition().x, square->getPosition().y);
        symbols.push_back(symbol);
        //It is the next player's turn
        player *= -1;
    }
}

/*
 * This function check if the game is won after a player took the ith square.
 */
bool gameFinished(std::vector<Square*> &board, unsigned i) {
    //Information about the square
    unsigned x = board[i]->getX();
    unsigned y = board[i]->getY();
    int player = board[i]->getPlayer();
    //Here we check all the possibilities (vertically, diagonnaly and horizontally)
    if(x == 0) {
        if(player == board[i+1]->getPlayer()) {
            if(player == board[i+2]->getPlayer())
                return true;
        }
        if(y == 0) {
            if(player == board[(y+1)*3+x+1]->getPlayer()) {
                if(player == board[(y+2)*3+x+2]->getPlayer())
                    return true;
            }
        }
        else if(y == 2) {
            if(player == board[(y-1)*3+x+1]->getPlayer()) {
                if(player == board[(y-2)*3+x+2]->getPlayer())
                    return true;
            }
        }
    }
    else if(x == 1) {
        if(player == board[i+1]->getPlayer()) {
            if(player == board[i-1]->getPlayer())
                return true;
        }
        if(y == 1) {
            if(player == board[(y+1)*3+x+1]->getPlayer()) {
                if(player == board[(y-1)*3+x-1]->getPlayer())
                    return true;
            }
            if(player == board[(y+1)*3+x-1]->getPlayer()) {
                if(player == board[(y-1)*3+x+1]->getPlayer())
                    return true;
            }
        }
    }
    else if(x == 2) {
        if(player == board[i-1]->getPlayer()) {
            if(player == board[i-2]->getPlayer())
                return true;
        }
        if(y == 0) {
            if(player == board[(y+1)*3+x-1]->getPlayer()) {
                if(player == board[(y+2)*3+x-2]->getPlayer())
                    return true;
            }
        }
        else if(y == 2) {
            if(player == board[(y-1)*3+x-1]->getPlayer()) {
                if(player == board[(y-2)*3+x-2]->getPlayer())
                    return true;
            }
        }
    }
    if(y == 0) {
        if(player == board[(y+1)*3+x]->getPlayer()) {
            if(player == board[(y+2)*3+x]->getPlayer())
                return true;
        }
    }
    else if(y == 1) {
        if(player == board[(y+1)*3+x]->getPlayer()) {
            if(player == board[(y-1)*3+x]->getPlayer())
                return true;
        }
    }
    else if(y == 2) {
        if(player == board[(y-1)*3+x]->getPlayer()) {
            if(player == board[(y-2)*3+x]->getPlayer())
                return true;
        }
    }
    return false;
}

/*
 * This function simply draw a box that displays a congratulation message.
 * It is called if the game is won by a player
 */
void drawWinBox(sf::RenderWindow &window, int player, sf::Font &font) {
    if(player == -1)
        player = 2;
    sf::RectangleShape winBox;
    winBox.setSize(sf::Vector2f(600, 300));
    winBox.setFillColor(sf::Color::Black);
    winBox.setOutlineThickness(10);
    winBox.setOutlineColor(sf::Color::Yellow);
    winBox.setPosition(100, 250);
    sf::Text winText;
    winText.setString("  Player " + std::to_string(player) + " won!\nCongratulations!");
    winText.setFont(font);
    winText.setOrigin(winText.getLocalBounds().width/2, winText.getLocalBounds().height/2);
    winText.setPosition(400, 400);
    winText.setColor(sf::Color::Yellow);
    window.draw(winBox);
    window.draw(winText);
}

/*
 * This function handle all the events that occurs in the program, such as
 * a mouse click or a keyboard input.
 */
void handleEvents(sf::RenderWindow &window, MainMenu *menu,
                std::vector<Square*> &board, unsigned &state, int &player,
                std::vector<Symbol*> &symbols, bool &gameWon) {
    sf::Event event;
    while(window.pollEvent(event)) {
        //If we press escape
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Escape)
                state = menu->showMenu();
        }
        //If we click the close button or chose to exit at the end of the game
        else if(event.type == sf::Event::Closed || state == EXIT)
            window.close();
        //When we click
        else if(event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                for(unsigned i=0; i < 9; i++) {
                    //Check if we click in a square
                    if(!gameWon && mouseInSquare(window, board[i])) {
                        //The player has the square
                        givePlayer(board[i], player, symbols);
                        //We check if the player won
                        gameWon = gameFinished(board, i);
                    }
                }
            }
        }
    }
}

/*
 * The main function of the program. We initialize everything in it.
 * This is where the main loop of the program is, where it updates the data,
 * draws in the window and displays it.
 */
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tic Tac Toe",
                            sf::Style::Titlebar | sf::Style::Close);
    //We launch the menu
    MainMenu *menu = new MainMenu(window);
    unsigned gameState = menu->showMenu();
    //If we chose to play
    if(gameState == ONEPLAYER || gameState == TWOPLAYERS) {
        menu->setGame(true);
        gameState = RESUME;
    }
    //Initialization of the game
    std::vector<Square*> board;
    initGame(board);
    //The crosses and circles
    std::vector<Symbol*> symbols;
    //The player: 1 = player 1, -1 = player 2
    int player = 1;
    //We create the players texts shown below the board
    sf::Text *players[2];
    sf::Font font;
    font.loadFromFile("Font/arial.ttf");
    std::string p;
    for(unsigned i=0; i < 2; i++) {
        p = "Player " + std::to_string(i+1);
        players[i] = new sf::Text;
        players[i]->setString(p);
        players[i]->setFont(font);
        players[i]->setCharacterSize(30);
        players[i]->setPosition(230*(i+1), 700);
    }

    bool gameWon = false;
    //Main loop of the program
    while(window.isOpen()) {
        handleEvents(window, menu, board, gameState, player, symbols, gameWon);
        //If we restart the game in the menu
        if(gameState == ONEPLAYER || gameState == TWOPLAYERS) {
            for(Symbol* symbol : symbols)
                delete symbol;
            symbols.clear();
            initGame(board);
            player = 1;
            gameState = RESUME;
            gameWon = false;
        }
        //The player is in Yellow, the other in grey
        if(!gameWon) //If the game is won, both texts are grey
            players[(player+2)%3]->setColor(sf::Color::Yellow);
        players[(player+3)%3%2]->setColor(sf::Color(94,94,94));
        //We draw what need to be drawn
        window.clear(sf::Color(5,5,5));
        window.draw(*players[0]);
        window.draw(*players[1]);
        for(Square* square : board)
            window.draw(*square);
        for(Symbol* symbol : symbols)
            window.draw(*symbol);
        //We display the congratulation message if the game is won
        if(gameWon) {
            drawWinBox(window, player*(-1), font);
            menu->setGame(false);
        }
        window.display();
    }
    //We delete everything
    delete players[0];
    delete players[1];
    for(Symbol* symbol : symbols)
        delete symbol;
    for(Square* square : board)
        delete square;
    delete menu;

    return 0;
}
