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
    cell += "N";
  }
  else {
      cell += "-";
  }
  if (SouthWall){ 
    cell += "S";
  }
  else {
      cell += "-";
  }
  if (EastWall){ 
    cell += "E";
  }
  else {
      cell += "-";
  }
  if (WestWall){ 
    cell += "W";
  }
  else {
      cell += "-";
  }
  return cell; 
}