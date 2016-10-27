#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <algorithm>
#include "tile.h"
#include "location.h"


// This class stores a grid of Tile pointers, which are NULL if the
// grid location does not (yet) contain a tile

class Board {
public:

  // CONSTRUCTOR
  // takes in the dimensions (height & width) of the board
  Board(int i, int j);

  

  // ACCESSORS
  int numRows() const { return board.size(); }
  int numColumns() const { return board[0].size(); }
  Tile* getTile(int i, int j) const;
  std::vector<Location> getLocations() const;
  int getRotation(int i, int j) const;

  // MODIFIERS
  void setTile(int i, int j, Tile* t);
  void setTile(int i, int j, Tile* t, int rot);
  void removeTile(int i, int j);
  void setLocations(int i, int j, int rotation);
  void removeLocation(int i, int j);
  void removeLocation(int i, int j, int rot);
  void sortLocations();
  // FOR PRINTING
  void Print() const;
  void PrintLocations() const;

private:
  std::vector<Location> locations;
  // REPRESENTATION
  std::vector<std::vector<Tile*> > board;
};


#endif