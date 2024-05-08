/*
* @file Maze.h
* @author Tatiana Lipert
* @date 2024-05-06
* @brief class to generate and store a maze as a 2 dimentional vector 
* it will also automatically remove the southern wall 
*/

#pragma once
#include "MazeCell.h" 
#include <vector> 
#include <string> 
#include <queue> 

using namespace std; 

enum class DIRECTION : char { 
  NORTH, 
  SOUTH, 
  EAST, 
  WEST, 
};

/* 
Class definition for the Maze class 
The constructor take two integer dimensions for the height and width of the maze 
If no values are given, the maze defaults to a 3x3 square 
*/
class Maze { 
public: 
  Maze(int mazeHeight = 3, int mazeWidth = 3); 
  string toString(); 

  MazeCell* getCell(int row, int column);  

  int getheight();
  int getWidth();

protected: 

  void generateMaze(int height, int width); 
  void tunnel(int row, int column, DIRECTION direction); 
  bool validCell(int row, int column); 
  void generatePath(int row, int column); 


  //member variables  
  int height, width; 
  vector<vector<MazeCell*>> maze; 

  //queue to store the unvisited cells during maze path generation 
    queue<MazeCell*> unvisited; 

};