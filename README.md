# GameOfLife written in C++ compilable to WebAssembly using Emscripten
Can be compiled to run natively instead (use cmake).


## To build and run in the browser:

### Install emsdk : 
Get the emsdk repo
- git clone https://github.com/emscripten-core/emsdk.git

Enter that directory
- cd emsdk

Fetch the latest version of the emsdk (not needed the first time you clone)
- git pull

Download and install the latest SDK tools.
- ./emsdk install latest

Make the "latest" SDK "active" for the current user. (writes .emscripten file)
- ./emsdk activate latest

Activate PATH and other environment variables in the current terminal
- source ./emsdk_env.sh

### Build (from root folder):
Run the command below to build emrun version. Remove --emrun to build for normal use. 
- emcc src/main.cpp -std=c++20 -Iinclude/ -fexceptions -s USE_SDL=2 --preload-file resources -o gol.html -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL_TTF=2 -s USE_BOOST_HEADERS=1 --emrun

### Run program (from root folder):
Run with a browser of your choice. To run in firefox:
- emrun --browser firefox gol.html 

### Play the game:
- Place a cell by clicking on the grid and remove by clicking the live cell.
- Start/pause and reset the game by using the buttons.