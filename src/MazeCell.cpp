/*
 * @file MazeCell.h
 * @author Tatiana Lipert
 * @date 2024-05-06
 * @brief
 */

#include "MazeCell.h" 
#include <string> 

using namespace std; 

//default constructor 
MazeCell::MazeCell(){ 
  NorthWall = true; 
  SouthWall = true;
  EastWall = true; 
  WestWall = true; 

  //cell starts as unvisited 
  visited = false; 
}

string MazeCell::toString(){ 
  string cell = ""; 
  if (NorthWall){ 

  }
  if (SouthWall){ 

  }
  if (EastWall){ 

  }
  if (WestWall){ 

  }
}

//access functions 
bool MazeCell::hasNorthWall() { 
	return NorthWall; 
}

bool MazeCell::hasSouthWall() { 
	return SouthWall; 
}

bool MazeCell::hasEastWall() { 
	return EastWall; 
}

bool MazeCell::hasWestWall() { 
	return WestWall; 
}