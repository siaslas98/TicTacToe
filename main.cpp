#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Game {
public:
    Game(RenderWindow& rendWind) : _rendWind(rendWind){
        _board = {{
                          {"", "", ""},
                          {"", "", ""},
                          {"", "", ""}
        }};
    }

    bool run(){
        return _run;
    }

    void handleEvents(){
        Event event;
        while(_rendWind.pollEvent(event)){
            if(event.type == Event::Closed){
                _rendWind.close();
            }

            if(event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left){
                Vector2i mousePos = Mouse::getPosition(_rendWind);
                handleClick(mousePos.x, mousePos.y);
            }
        }
        _rendWind.clear(Color::Blue);
        drawGrid();
        drawBoard();
        _rendWind.display();
    }

    void drawGrid(){
       RectangleShape line;

        // Vertical lines
        for(int i = 0; i <= _cols; ++i){
            float x = i * _cellSize;
            line.setPosition(x - _lineThickness/2, 0); // Sets the x pos
            line.setSize(Vector2f(_lineThickness, _rows * _cellSize)); // Sets the length
            line.setFillColor(Color::Red); // Sets the color
            _rendWind.draw(line);
        }

        // Horizontal Lines
        for(int i = 0; i <= _rows; ++i) {
            float y = i * _cellSize;
            line.setPosition(0, y - _lineThickness / 2);
            line.setSize(Vector2f(_cols * _cellSize, _lineThickness));
            line.setFillColor(Color::Red);
            _rendWind.draw(line);
        }
    }

    void drawX(float x, float y, float size){
        float thickness = 8.0f;

        RectangleShape line1(Vector2f(250.0f, thickness));
        line1.setPosition(x + 16, y + 10 );
        line1.setFillColor(Color::Black);
        line1.setRotation(45);
        _rendWind.draw(line1);

        RectangleShape line2(Vector2f(250.0f, thickness));
        line2.setPosition(x + 14, y + size - 10);
        line2.setFillColor(Color::Black);
        line2.setRotation(-45);
        _rendWind.draw(line2);

    }

    void drawO(float x, float y, float size){
        float radius = (size / 2) - 20;
        CircleShape circle(radius);
        circle.setOutlineThickness(8);
        circle.setOutlineColor(Color::White);
        circle.setFillColor(Color::Transparent);


        // Center the circle in the cell
        float offsetX = (size - 2 * radius) / 2;
        float offsetY = (size - 2 * radius) / 2;

        circle.setPosition(x + offsetX, y + offsetY);
        _rendWind.draw(circle);
    }

    void drawBoard(){
        for(int row = 0; row < 3; ++row){
            for(int col = 0; col < 3; ++col){
                float x = col * _cellSize;
                float y = row * _cellSize;
                if(_board[row][col] == "X"){
                    drawX(x, y, _cellSize);
                }
                else if(_board[row][col] == "O"){
                    drawO(x, y, _cellSize);
                }
            }
        }
    }
private:
    RenderWindow& _rendWind;
    array<array<string, 3>, 3> _board;
    bool _run = true;
    int _rows = 3;
    int _cols = 3;
    float _cellSize = 200.0f;
    int _lineThickness = 5;
    bool _playerXTurn = true;

    void handleClick(int mouseX, int mouseY){
        int col = mouseX / _cellSize;
        int row = mouseY / _cellSize;

        if(row >= 0 && row < 3 && col >= 0 && col < 3 && _board[row][col] == ""){
            _board[row][col] = _playerXTurn ? "X" : "O";
            _playerXTurn = !_playerXTurn; // Switch turns
        }
    }
};


int main()
{
    RenderWindow screen(VideoMode(600, 600), "Tic-Tac-Toe");
    Game game(screen);

    while(game.run()){
        game.handleEvents();
    }

    return 0;
}
