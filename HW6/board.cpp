#include <cassert>
#include <cstdlib>
#include <cstdio>

#include "board.h"


// this global variable is set in main.cpp and is adjustable from the command line
// (you are not allowed to make your own global variables)
extern int GLOBAL_TILE_SIZE;


// ==========================================================================
// CONSTRUCTOR
Board::Board(int i, int j) {
  board = std::vector<std::vector<Tile*> >
    ( (unsigned int)i,
      std::vector<Tile*>((unsigned int)j,NULL) );

  std::vector<Location> locations;
}


// ==========================================================================
// ACCESSORS
Tile* Board::getTile(int i, int j) const {
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  return board[i][j];
}

std::vector<Location> Board::getLocations() const
{
  return locations;
}

// ==========================================================================
// MODIFIERS
void Board::setTile(int i, int j, Tile* t) {
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  assert (t != NULL);
  assert (board[i][j] == NULL);
  board[i][j] = t;
}
void Board::setTile(int i, int j, Tile* t, int rot)
{
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  assert (t != NULL);
  assert (board[i][j] == NULL);
  board[i][j] = t;
  locations.push_back(Location(i,j,rot));
}

void Board::removeTile(int i, int j)
{
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  board[i][j]=NULL;
  if(locations[locations.size()-1].row == i && locations[locations.size()-1].column == j)
  {
    locations.pop_back();
  }
  else
  {
    removeLocation(i,j);
  }
}

void Board::setLocations(int i, int j, int rotation)
{
  locations.push_back(Location(i,j,rotation));
}

void Board::removeLocation(int i, int j)
{
  for(uint k=0; k<locations.size(); ++k)
  {
    if(locations[k].row == i && locations[k].column == j)
    {
      locations.erase(locations.begin()+k);
    }
  }
}
void Board::removeLocation(int i, int j, int rot)
{
  for(uint k=0; k<locations.size(); ++k)
  {
    if(locations[k].row == i && locations[k].column == j && locations[k].rotation == rot)
    {
      locations.erase(locations.begin()+k);
    }
  }
}
int Board::getRotation(int i, int j) const
{
  for(uint k=0; k<locations.size(); ++k)
  {
    if(locations[k].row == i && locations[k].column == j)
    {
      return locations[k].rotation;
    }
  }
  return 0;
}

//Makes use of comparator defined in the location class
void Board::sortLocations()
{
  std::sort(locations.begin(), locations.end());
}

// ==========================================================================
// PRINTING
void Board::Print() const {
  for (int b = 0; b < numRows(); b++) {
    for (int i = 0; i < GLOBAL_TILE_SIZE; i++) {
      for (int j = 0; j < numColumns(); j++) {
        if (board[b][j] != NULL) {
          board[b][j]->printRow(std::cout,i,getRotation(b,j));
        } else {
          std::cout << std::string(GLOBAL_TILE_SIZE,' ');
        }
      }
      std::cout << "\n";
    }
  }
  fflush(stdout);
}

void Board::PrintLocations() const
{
  for(uint k=0; k<locations.size(); ++k)
  {
    std::cout << locations[k];
  }
}
// ==========================================================================