/*
* @file Maze.cpp
* @author Tatiana Lipert
* @date 2024-05-06
* @brief
*/

#include "Maze.h"
#include "MazeCell.h"
#include <vector> 
#include <queue> 
#include <algorithm> 
#include <random> 

using namespace std; 

//default constructor 
Maze::Maze(int mazeHeight, int mazeWidth){ 
  height = mazeHeight; 
  width = mazeWidth;
  
}

string Maze::toString(){ 
  
}

//access the size of the maze 
int Maze::getheight() {
    return height; 
}

int Maze::getWidth() { 
    return width; 
}

//return a pointer to the cell at location row, column
MazeCell* Maze::getCell(int row, int column){ 
  return maze[row][column];
}

//simple helper function to check that a cell's column/row position is in bounds 
bool Maze::validCell(int row, int column){ 
    if((row < height) && (column < width) && (row >= 0) && (column >= 0)){ 
      return true; 
    }
    else{ 
      return false; 
    }
}

/* 
Helper function to remove the wall from one side of a cell 
Assumes that the inputed locatoin is a valid cell 
*/
void Maze::tunnel(int row, int column, DIRECTION direction){ 
  if(direction == DIRECTION::NORTH){ 
    maze[row][column]->NorthWall = false; 
  }
  else if(direction == DIRECTION::SOUTH){ 
    maze[row][column]->SouthWall = false;
  }
  else if(direction == DIRECTION::EAST){ 
    maze[row][column]->EastWall = false;
  }
  else if(direction == DIRECTION::WEST){ 
    maze[row][column]->WestWall = false;
  }
}

/* 
Maze generation process: 
1. initialize maze grid  with new cell pointerss with all four walls 
        - add cell to "unvisited" queue 
2. start at cell 0,0, this is the bottom left corner 
3. randomly select a valid adjacent cell 
        - seed changes and is generated based on the current system time
4. if cell is unvisited, tunnel to it 
5. mark adjacent cell as visited and move to it 
6. continue steps 3-5 until there are no valid adjacent cells 
7. back up to last visited cell, then continue tunnelling to unvisited cells 
8. once finished, tunnel the south wall of the bottem left cell, and the north wall of the top right cell

*/

void Maze::generateMaze(int height, int width){

    //resize the maze to match the given height and width  
    maze.resize(height);
    //set all of the row widths 
    for( int i = 0; i < height; i++){
      maze[i].resize(width); 
    }

    //generate the grid of maze cells 
    for (int h = 0; h < height; h++){ 
      for (int w = 0; w < width; w++){ 
        maze[h][w] = new MazeCell(); 
        unvisited.push(maze[h][w]); 
      }
    }

    //start maze path generation at cell 0,0 
    generatePath(0,0); 

    //tunnel the entrance and exit walls 
    tunnel(0, 0, DIRECTION::SOUTH); 
    tunnel(height-1, width-1, DIRECTION::NORTH); 

}


/* 
This function recursively generates a path through the maze
*/
void Maze::generatePath(int row, int column){ 
    //end condition (stop generating a path once all the cells have been visited) 
    if (unvisited.size() == 0){ 
      return; 
    }
    unvisited.pop(); 
    //mark the current cell as visited 

    //define array of directions, then shuffle it. 
    vector<DIRECTION> dir = {DIRECTION::NORTH, DIRECTION::SOUTH, DIRECTION::EAST, DIRECTION::WEST};
    int seed = 0; //seed for the random generator 
    int end = sizeof(dir)/sizeof(dir[0]);
    shuffle(dir.begin(), dir.end(), default_random_engine(seed));

    for(int i = 0; i <=3; i++){ 
      if (dir[i] == DIRECTION::NORTH){ 
        if(validCell(row+1, column) && !maze[row+1][column]->visited){ 
          tunnel(row+1, column, DIRECTION::SOUTH); 
          tunnel(row, column, DIRECTION::NORTH); 
          generatePath(row+1, column); 
        }
      }
      else if (dir[i] == DIRECTION::SOUTH){ 
        if(validCell(row-1, column && !maze[row-1][column]->visited)){ 
          tunnel(row-1, column, DIRECTION::NORTH); 
          tunnel(row, column, DIRECTION::SOUTH); 
          generatePath(row-1, column); 
        }
      }
      else if (dir[i] == DIRECTION::EAST){ 
        if(validCell(row, column+1) && !maze[row][column+1]->visited){ 
          tunnel(row, column+1, DIRECTION::WEST); 
          tunnel(row, column, DIRECTION::EAST); 
          generatePath(row+1, column); 
        }
      }
      else{ 
        if(validCell(row, column-1) && !maze[row][column-1]->visited){ 
          tunnel(row, column-1, DIRECTION::EAST); 
          tunnel(row, column, DIRECTION::WEST); 
          generatePath(row+1, column); 
        }
      }
    }

}