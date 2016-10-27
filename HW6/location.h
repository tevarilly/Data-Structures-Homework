#ifndef _LOCATION_H_
#define _LOCATION_H_

#include <iostream>


// Tiny all-public class to store the grid coordinates and rotation
// for placing a tile onto the board
class Location {
public:
  Location (int r, int c, int rot);
  int row;
  int column;
  int rotation; // should be 0, 90, 180, or 270

  // ACCESSORS
  int getRow() const { return row; }
  int getCol() const { return column; }
  int getRot() const { return rotation; }
};


// Check of these two locations are the same
bool operator==(const Location &loc1, const Location &loc2);

bool operator<(const Location &a, const Location &b); 


// Helper function to output a location as part of a puzzle solution
std::ostream& operator<<(std::ostream &ostr, const Location &loc);


#endif