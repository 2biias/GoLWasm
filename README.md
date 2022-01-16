# GameOfLife written in C++ to be compiled into WebAssembly using Emscripten

To build and run:

Install SDL: 
- sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev

Run Cmake:
- cd build
- cmake ..

Build (from build folder):
- make

Run program (from root folder):
- cd to root folder
- ./build/GameOfLife


Play the game:
- place a cell by clicking on the grid and remove by clicking the live cell
- start/pause and reset the game by using the buttons