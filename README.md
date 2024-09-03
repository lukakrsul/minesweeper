# Minesweeper 🎮

A classic Minesweeper game implemented using C++ and SFML. Explore the grid, avoid the mines, and complete the levels!

## 📖 Project Description

This is a simple implementation of the classic Minesweeper game. The game features multiple levels with increasing difficulty, where you need to uncover tiles and avoid mines. If you uncover all non-mine tiles, you advance to the next level. If you hit a mine, the game is over. 

## 🚀 Features

- **Multiple Levels**: Start with a 9x9 grid and advance through progressively challenging levels.
- **Mine Placement**: Mines are placed randomly, avoiding the first move.
- **Number Display**: Cells display numbers indicating the count of adjacent mines.
- **Flagging System**: Right-click to flag suspected mines.
- **Level Transition**: Automatic progression to the next level upon completion.
- **Game Over Screen**: Displays a message and resets the game.

## 🎨 Assets

The game uses textures and a font located in the assets folder. Ensure that the folder is present.

## 👾 Usage

- **Left Click**: Uncover a tile. If you uncover a mine, the game is over.
- **Right Click**: Flag or unflag a tile.
- **Middle Click**: Output the board to the console for debugging.

## 💻 Installation
- **C++ Compiler**: Ensure you have a C++ compiler installed (e.g., GCC, Clang).
- **SFML Library**: Install SFML for handling graphics and update its location in `CMakeLists.txt`


