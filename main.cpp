#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;


class Game {
public:
    explicit Game(RenderWindow& rendWind) : _rendWind(rendWind){
        _board = {{
                          {"", "", ""},
                          {"", "", ""},
                          {"", "", ""}
        }};
    }

    [[nodiscard]] bool run() const{
        return _run;
    }

    pair<int, int> miniMax(){
        int bestScore = 1000;
        pair<int, int> bestMove = {-1, -1};

        for(int row = 0; row < 3; ++row) {
            for(int col = 0; col < 3; ++col) {
                // If the cell is empty
                if (_board[row][col].empty()) {
                    _board[row][col] = "O";

                    int score = miniMaxRecursive(true, 0);
                    _board[row][col].clear(); // Undo move

                    // Minimize the score; in case of tie, prefer the move with the lower depth
                    if (score < bestScore || (score == bestScore && bestMove == pair<int, int>{-1, -1})) {
                        bestScore = score;
                        bestMove = {row, col};
                    }
                }
            }
        }
        return bestMove;
    }

    int miniMaxRecursive(bool maximizingPlayer, int depth) {
        if (checkWinner()) {
            return maximizingPlayer ? -1 : 1;
        }
        if (isBoardFull()) {
            return 0;
        }

        int bestScore = maximizingPlayer ? -1000 : 1000;

        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (_board[row][col].empty()) {
                    _board[row][col] = maximizingPlayer ? "X" : "O";

                    int score = miniMaxRecursive(!maximizingPlayer, depth + 1);

                    _board[row][col].clear();

                    if (maximizingPlayer) {
                        bestScore = max(bestScore, score);
                    } else {
                        if (score < bestScore || (score == bestScore && depth < 3)) {
                            bestScore = score;
                        }
                    }
                }
            }
        }
        return bestScore;
    }

    bool checkWinner(){
        // Check rows
        for(int i = 0; i < 3; ++i){
            if(!_board[i][0].empty() && _board[i][0] == _board[i][1] && _board[i][1] == _board[i][2]){
                return true;
            }
        }

        // Check columns
        for(int i = 0; i < 3; ++i){
            if(!_board[0][i].empty() && _board[0][i] == _board[1][i] && _board[1][i] == _board[2][i]){
                return true;
            }
        }

        // Check diagonals
        if(!_board[0][0].empty() && _board[0][0] == _board[1][1] && _board[1][1] == _board[2][2]){
            return true;
        }
        if(!_board[2][0].empty() && _board[2][0] == _board[1][1] && _board[1][1] == _board[0][2]){
            return true;
        }

        return false;
    }

    bool isBoardFull(){
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                if(_board[i][j].empty()) return false;
            }
        }
        return true;
    }

    void handleEvents(){
        Event event{};
        while(_rendWind.pollEvent(event)){
            if(event.type == Event::Closed){
                _rendWind.close();
                _run = false;
            }

            if(event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left){
                Vector2i mousePos = Mouse::getPosition(_rendWind);
                handleClick(mousePos.x, mousePos.y);
            }
        }

        draw();
    }

    void drawGrid(){
       RectangleShape line;

        // Vertical lines
        for(int i = 0; i <= _cols; ++i){
            float x = static_cast<float>(i) * _cellSize;
            line.setPosition(x - _lineThickness/2, 0); // Sets the x pos
            line.setSize(Vector2f(_lineThickness, static_cast<float>(_rows) * _cellSize)); // Sets the length
            line.setFillColor(Color::Red); // Sets the color
            _rendWind.draw(line);
        }

        // Horizontal Lines
        for(int i = 0; i <= _rows; ++i) {
            float y = static_cast<float>(i) * _cellSize;
            line.setPosition(0, y - _lineThickness / 2);
            line.setSize(Vector2f(static_cast<float>(_cols) * _cellSize, _lineThickness));
            line.setFillColor(Color::Red);
            _rendWind.draw(line);
        }
    }

    void drawX(float x, float y){
        float thickness = 8.0f;

        RectangleShape line1(Vector2f(250.0f, thickness)); // Length and thickness
        line1.setPosition(x + 16, y + 10 );
        line1.setFillColor(Color::Black);
        line1.setRotation(45);
        _rendWind.draw(line1);

        RectangleShape line2(Vector2f(250.0f, thickness));
        line2.setPosition(x + 14, y + _cellSize - 10);
        line2.setFillColor(Color::Black);
        line2.setRotation(-45);
        _rendWind.draw(line2);

    }

    void drawO(float x, float y){
        float radius = (_cellSize / 2) - 20; // -20 takes into account circle thickness
        CircleShape circle(radius);
        circle.setOutlineThickness(8);
        circle.setOutlineColor(Color::White);
        circle.setFillColor(Color::Transparent);


        // Center the circle in the cell
        float offsetX = (_cellSize - 2 * radius) / 2;
        float offsetY = (_cellSize - 2 * radius) / 2;

        circle.setPosition(x + offsetX, y + offsetY);
        _rendWind.draw(circle);
    }

    void drawBoard(){
        for(int row = 0; row < 3; ++row){
            for(int col = 0; col < 3; ++col){
                float x = static_cast<float> (col) * _cellSize;
                float y = static_cast<float> (row) * _cellSize;
                if(_board[row][col] == "X"){
                    drawX(x, y);
                }
                else if(_board[row][col] == "O"){
                    drawO(x, y);
                }
            }
        }
    }

    void draw(){
        // Clear the window and draw the grid and board
        _rendWind.clear(Color::Blue);
        drawGrid();
        drawBoard();
        _rendWind.display();
    }

    // Debugging function to print the board state
    void printBoard() {
        cout << "Board state:" << endl;
        for(int row = 0; row < 3; ++row) {
            for(int col = 0; col < 3; ++col) {
                cout << (_board[row][col].empty() ? "-" : _board[row][col]) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
private:
    RenderWindow& _rendWind;
    array<array<string, 3>, 3> _board;
    bool _run = true;
    int _rows = 3;
    int _cols = 3;
    float _cellSize = 200.0f;
    float _lineThickness = 5.0f;
    bool _playerXTurn = true;
    char _playerX = 'x';

    void handleClick(int mouseX, int mouseY){
        if (_playerXTurn && _run) {
            int col = mouseX / static_cast<int>(_cellSize);
            int row = mouseY / static_cast<int>(_cellSize);

            if (row >= 0 && row < 3 && col >= 0 && col < 3 && _board[row][col].empty()) {
                // Player's move
                _board[row][col] = "X";
                _playerXTurn = false; // Switch turns

                // Check if player X wins
                if (checkWinner()) {
                    _run = false;
                } else {
                    // If no winner, trigger the computer's move
                    // Introduce a delay before the computer makes its move
                    draw();
                    computerMove();
                }
            }
        }
        draw();
    }

    void computerMove() {
        // Introduce a delay before the computer makes its move
        sleep(milliseconds(500));

        // Computer's move (O) using minimax
        auto [bestRow, bestCol] = miniMax();
        _board[bestRow][bestCol] = 'O';

        // Ensure the board is drawn after the move
        printBoard();
        _playerXTurn = true; // Switch turns back to the player
        draw();

        // Check if player O wins
        if (checkWinner()) {
            _run = false;
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
