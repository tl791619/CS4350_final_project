/*
 * @file MazeCell.h
 * @author Tatiana Lipert
 * @date 2024-05-06
 * @brief
 */

#pragma once
#include "Maze.h"
#include <string> 

using namespace std; 

class MazeCell  {

//make sure the maze class is a friend so walls can be editted 
friend class Maze; 

public:

  /* 
  Default constructor 
  Assumes there are walls on all four edges 
  */
  MazeCell(); 
  string toString(); 

  //return the bools that stor what sides of the cell have a wall
  bool hasNorthWall();
  bool hasSouthWall();
  bool hasEastWall();
  bool hasWestWall();

protected:

  bool NorthWall, SouthWall, EastWall, WestWall; 
  bool visited; 

};