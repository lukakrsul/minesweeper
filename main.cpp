#include <iostream>
#include <vector>
#include <cstdlib>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// Function to randomly place mines on the board
void draw_mines(vector<vector<int>>& board, int size, int mine_count, int firstMoveX, int firstMoveY) {
    // Keep track of the number of mines placed
    int mines = 0;
    while(mines < mine_count) {
        int cordinateX = rand() % size;
        int cordinateY = rand() % size;
        // Ensure no mine is placed on the initial move position and only in empty cells
        if(board[cordinateX][cordinateY] == 0 && (cordinateX != firstMoveX || cordinateY != firstMoveY)) {
            board[cordinateX][cordinateY] = -1;
            mines++;
        }
    }
}

// Function to calculate numbers for each cell based on adjacent mines
void draw_numbers(vector<vector<int>>& board, int size) {
    int mines;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(board[i][j] == 0) {
                int mines = 0;

                // Check all adjacent cells for mines
                if(i > 0 && j > 0 && board[i-1][j-1] == -1) mines++;
                if(i > 0 && board[i-1][j] == -1) mines++;
                if(i > 0 && j < size-1 && board[i-1][j+1] == -1) mines++;
                if(j > 0 && board[i][j-1] == -1) mines++;
                if(j < size-1 && board[i][j+1] == -1) mines++;
                if(i < size-1 && j > 0 && board[i+1][j-1] == -1) mines++;
                if(i < size-1 && board[i+1][j] == -1) mines++;
                if(i < size-1 && j < size-1 && board[i+1][j+1] == -1) mines++;

                board[i][j] = mines;
            }
        }
    }
}

// Function to initialize the game board and discovered cells
void initialize_board(vector<vector<int>>& board, vector<vector<int>>& discovered, int size, int mine_count) {
    board.clear();
    discovered.clear();
    board.resize(size, vector<int>(size, 0));
    discovered.resize(size, vector<int>(size, 0));
}

// Function to draw the grid on the window
void draw_grid(RenderWindow &window, vector<vector<int>> &board, vector<vector<int>> discovered, Texture textures[], int tileSize) {
    Sprite tileSprite;

    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board.size(); ++j) {

            if(!discovered[i][j]) {
                tileSprite.setTexture(textures[0]);  // Undiscovered cell
            } else if(discovered[i][j] == 2) {
                tileSprite.setTexture(textures[11]); // Flagged cell
            } else {
                int value = board[i][j];
                if(value >= 1 && value <= 8) {
                    tileSprite.setTexture(textures[value]); // Numbered cell
                } else if(value == -1){
                    tileSprite.setTexture(textures[10]); // Mine cell
                } else {
                    tileSprite.setTexture(textures[9]); // Empty cell
                }
            }

            tileSprite.setPosition(j * tileSize, i * tileSize);
            window.draw(tileSprite);
        }
    }
}

// Function to reveal empty cells recursively
void reveal_empty(vector<vector<int>>& board, vector<vector<int>>& discovered, int x, int y) {
    if(x < 0 || x >= board.size() || y < 0 || y >= board.size() || discovered[x][y] == 1) {
        return;
    }

    discovered[x][y] = 1;

    if (board[x][y] != 0) {
        return;
    }

    // Recursively reveal adjacent cells
    reveal_empty(board, discovered, x-1, y-1);   // Top-left
    reveal_empty(board, discovered, x-1, y);     // Top
    reveal_empty(board, discovered, x-1, y+1);   // Top-right
    reveal_empty(board, discovered, x, y-1);     // Left
    reveal_empty(board, discovered, x, y+1);     // Right
    reveal_empty(board, discovered, x+1, y-1);   // Bottom-left
    reveal_empty(board, discovered, x+1, y);     // Bottom
    reveal_empty(board, discovered, x+1, y+1);   // Bottom-right
}

// Function to print the board to the console (for debugging)
void output_board(vector<vector<int>> board) {
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board.size(); j++) {
            cout << board[i][j] << "|";
        }
        cout << endl << "--------------------" << endl;
    }
}

// Function to update level settings based on the current level
void next_level(int& level, int& size, int& mine_count) {
    switch (level) {
        case 1:
            size = 9;
            mine_count = 5;
            break;
        case 2:
            size = 9;
            mine_count = 10;
            break;
        case 3:
            size = 16;
            mine_count = 40;
            break;
        case 4:
            size = 16;
            mine_count = 60;
            break;
        case 5:
            size = 16;
            mine_count = 99;
            break;
        default:
            size += 2; // Increase board size for higher levels
            mine_count += 2; // Increase mine count for higher levels
            break;
    }
}

int main() {
    vector<vector<int>> board;
    vector<vector<int>> discovered;
    int level = 1;
    int size = 9;
    int mine_count = 5;
    int tile_size = 32; // Size of each tile in pixels
    bool firstMove = true;  // Flag to indicate the first move
    bool levelCompleted  = false; // Flag to indicate if the current level is completed
    bool gameOver = false; // Flag to indicate game over

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Initialize the game board and discovered cells
    initialize_board(board, discovered, size, mine_count);

    // Create the window
    RenderWindow window(VideoMode(size * tile_size, size * tile_size), "Minesweeper");

    // Load textures for the game
    Texture textures[12];
    string textureFiles[12] = {
        "../assets/undiscovered.jpg", // 0
        "../assets/1.jpg",            // 1
        "../assets/2.jpg",            // 2
        "../assets/3.jpg",            // 3
        "../assets/4.jpg",            // 4
        "../assets/5.jpg",            // 5
        "../assets/6.jpg",            // 6
        "../assets/7.jpg",            // 7
        "../assets/8.jpg",            // 8
        "../assets/discovered.jpg",   // 9
        "../assets/mine.jpg",         // 10
        "../assets/flag.jpg"          // 11
    };

    // Error handling for texture loading
    for (int i = 0; i < 12; ++i) {
        if (!textures[i].loadFromFile(textureFiles[i])) {
            cout << "Error loading texture: " << textureFiles[i] << endl;
            return -1;
        }
    }

    // Load font for text rendering
    Font font;
    if (!font.loadFromFile("../assets/arial.ttf")) {  // Ensure font file is in assets folder
        cout << "Error loading font" << endl;
        return -1;
    }

    // Create Text objects for displaying messages
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(40); // Font size in pixels
    gameOverText.setFillColor(Color::Red);
    gameOverText.setStyle(Text::Bold);

    Text levelCompleteText;
    levelCompleteText.setFont(font);
    levelCompleteText.setString("Level Complete");
    levelCompleteText.setCharacterSize(40); // Font size in pixels
    levelCompleteText.setFillColor(Color::Green);
    levelCompleteText.setStyle(Text::Bold);

    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::MouseButtonPressed) {
                // Get mouse coordinates (x and y are inverted)
                int x = event.mouseButton.y / tile_size;
                int y = event.mouseButton.x / tile_size;

                if(event.mouseButton.button == Mouse::Left) {
                    // Handle first move: place mines and compute numbers
                    if(firstMove) {
                        firstMove = false;
                        draw_mines(board, size, mine_count, x, y);
                        draw_numbers(board, size);
                    }
                    // Handle cell uncovering
                    if(!discovered[x][y]) {
                        if(board[x][y] == -1) { // Mine hit
                            cout << "Game over" << endl;
                            gameOver = true;
                            discovered[x][y] = 1;
                        } else if(board[x][y] == 0) { // Empty cell
                            reveal_empty(board, discovered, x, y);
                        } else { // Numbered cell
                            discovered[x][y] = 1;
                        }
                    }
                } else if(event.mouseButton.button == Mouse::Right) {
                    // Handle flagging cells
                    if(!discovered[x][y]) {
                        discovered[x][y] = 2; // Flag the cell
                    } else if(discovered[x][y] == 2) {
                        discovered[x][y] = 0; // Remove flag
                    }
                } else if(event.mouseButton.button == Mouse::Middle) {
                    // Output board for debugging
                    output_board(board);
                }
            }
        }

        // Check for level completion
        int noDiscovered = 0;
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board.size(); ++j) {
                if(discovered[i][j] == 1) noDiscovered++;
            }
        }
        if ((noDiscovered + mine_count == size * size) && !levelCompleted) {
            levelCompleted = true;
            gameOver = false;
            cout << "Level " << level << " completed!" << endl;

            // Display the "Level Complete" message for 2 seconds
            FloatRect levelCompleteTextRect = levelCompleteText.getLocalBounds();
            levelCompleteText.setOrigin(levelCompleteTextRect.left + levelCompleteTextRect.width / 2.0f,
                                        levelCompleteTextRect.top + levelCompleteTextRect.height / 2.0f);
            levelCompleteText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
            window.draw(levelCompleteText);
            window.display();

            sleep(seconds(2));  // Pause to show the message

            // Proceed to the next level
            level++;
            next_level(level, size, mine_count);
            initialize_board(board, discovered, size, mine_count);

            // Resize the window to fit the new board size
            window.close();  // Close the old window
            window.create(VideoMode(size * tile_size, size * tile_size), "Minesweeper");  // Create a new window with updated size
            firstMove = true; // Reset firstMove for the new level
            levelCompleted = false;
        }

        // Clear the window and draw the current game state
        window.clear(Color::Black);
        draw_grid(window, board, discovered, textures, tile_size);

        if (gameOver) {
            // Display the "Game Over" message
            FloatRect gameOverTextRect = gameOverText.getLocalBounds();
            gameOverText.setOrigin(gameOverTextRect.left + gameOverTextRect.width / 2.0f,
                                   gameOverTextRect.top + gameOverTextRect.height / 2.0f);
            gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
            window.draw(gameOverText);
            window.display();

            sleep(seconds(2));  // Pause to show the message

            // Reset the game to level 1
            level = 1;
            next_level(level, size, mine_count);
            initialize_board(board, discovered, size, mine_count);

            gameOver = false; // Reset gameOver flag
            firstMove = true; // Reset firstMove for a fresh start

            // Clear the window to remove the "Game Over" message
            window.clear(Color::Black);
        }

        window.display(); // Display the updated frame
    }

    return 0;
}
