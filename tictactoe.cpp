#include <iostream>
#include <vector>
#include <string>

#include "MainMenu/mainmenu.hpp"

#define ONEPLAYER 1
#define TWOPLAYERS 2
#define RESUME 3
#define EXIT 4

class Square : public sf::RectangleShape {
    public:
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

class Symbol : public sf::Drawable, public sf::Transformable {
    public:
        Symbol() {}

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            states.transform *= getTransform();
            target.draw(vertices, states);
        }

    protected:
        sf::VertexArray vertices;
};

class Cross : public Symbol {
    public:
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

class Circle : public Symbol {
    public:
        Circle(unsigned x, unsigned y){
            unsigned N = 100;
            vertices.setPrimitiveType(sf::TrianglesFan);
            vertices.resize((N+2)*2);
            vertices[0] = sf::Vector2f(x+95, y+95);
            vertices[0].color = sf::Color::White;
            for(unsigned i=1; i <= N+1; i++) {
                vertices[i] = sf::Vector2f(x+95+75*cos(2*M_PI*(i-1)/N),
                                            y+95+75*sin(2*M_PI*(i-1)/N));
                vertices[i].color = sf::Color::Black;
            }

            vertices[N+2] = sf::Vector2f(x+95, y+95);
            vertices[N+2].color = sf::Color::White;
            for(unsigned i=N+3; i < (N+2)*2; i++) {
                vertices[i] = sf::Vector2f(x+95+65*cos(2*M_PI*(i-1)/N),
                                            y+95+65*sin(2*M_PI*(i-1)/N));
                vertices[i].color = sf::Color::White;
            }
        }
};

void initGame(std::vector<Square*> &board) {
    for(Square *square : board)
        delete square;
    board.clear();
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

bool mouseInSquare(sf::RenderWindow &window, Square *square) {
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mouse);
    unsigned squareSize = 190;
    sf::Vector2f squarePosition = square->getPosition();

    if(mousePos.x >= squarePosition.x && mousePos.x <= squarePosition.x + squareSize &&
        mousePos.y >= squarePosition.y && mousePos.y <= squarePosition.y + squareSize)
        return true;
    else
        return false;
}

void givePlayer(Square *square, int &player, std::vector<Symbol*> &symbols) {
    if(square->getPlayer() == 0) {
        Symbol *symbol;
        square->setPlayer(player);
        if(player == 1) {
            symbol = new Cross(square->getPosition().x, square->getPosition().y);
        }
        else if(player == -1)
            symbol = new Circle(square->getPosition().x, square->getPosition().y);
        symbols.push_back(symbol);
        player *= -1;
    }
}

void handleEvents(sf::RenderWindow &window, MainMenu *menu,
                std::vector<Square*> &board, unsigned &state, int &player,
                std::vector<Symbol*> &symbols) {
    sf::Event event;
    while(window.pollEvent(event)) {
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Escape)
                state = menu->showMenu();
        }
        else if(event.type == sf::Event::Closed)
            window.close();
        else if(event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                for(unsigned i=0; i < 9; i++) {
                    if(mouseInSquare(window, board[i])) {
                        givePlayer(board[i], player, symbols);
                    }
                }
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tic Tac Toe");

    MainMenu *menu = new MainMenu(window);
    unsigned gameState = menu->showMenu();
    if(gameState == ONEPLAYER || gameState == TWOPLAYERS) {
        menu->setGame(true);
        gameState = RESUME;
    }

    std::vector<Square*> board;
    initGame(board);
    std::vector<Symbol*> symbols;
    int player = 1;
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

    while(window.isOpen()) {
        handleEvents(window, menu, board, gameState, player, symbols);
        if(gameState == ONEPLAYER || gameState == TWOPLAYERS) {
            for(Symbol* symbol : symbols)
                delete symbol;
            symbols.clear();
            initGame(board);
            player = 1;
            gameState = RESUME;
        }
        if(player == 1) {
            players[0]->setColor(sf::Color::Yellow);
            players[1]->setColor(sf::Color::White);
        }
        else if(player == -1) {
            players[0]->setColor(sf::Color::White);
            players[1]->setColor(sf::Color::Yellow);
        }
        window.clear();
        window.draw(*players[0]);
        window.draw(*players[1]);
        for(Square* square : board)
            window.draw(*square);
        for(Symbol* symbol : symbols)
            window.draw(*symbol);
        window.display();
    }
    delete players[0];
    delete players[1];
    for(Symbol* symbol : symbols)
        delete symbol;
    for(Square* square : board)
        delete square;
    delete menu;

    return 0;
}
