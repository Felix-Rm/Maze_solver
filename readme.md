# Maze solver

## Usage

Example for compiling on Linux:  
`g++ main.cpp -g -o maze_solver.out $(pkg-config --libs opencv)`  
  
To run the program:  
`./maze_solver.out maze.png [optional name of directory]`

## Mazes

Mazes must satisfy following criteria:  

* A black border
* Walls are black, paths are white
* Paths are one pixel wide
* One white pixel at the top (start)
* One white pixel at the bottom (end)
