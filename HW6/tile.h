#ifndef  __TILE_H__
#define __TILE_H__

#include <iostream>
#include <string>
#include <vector>


// This class represents a single Carcassonne tile and includes code
// to produce a human-readable ASCII art representation of the tile.

class Tile {
public:

  // Constructor takes in 4 strings, representing what is on the edge
  // of each tile.  Each edge string is "pasture", "road", or "city".
  Tile(const std::string &north, const std::string &east, const std::string &south, const std::string &west);

  // ACCESSORS
  const std::string& getNorth() const { return north_; }
  const std::string& getSouth() const { return south_; }
  const std::string& getEast() const { return east_; }
  const std::string& getWest() const { return west_; }
  const std::string& getNorth(int rot) const;
  const std::string& getSouth(int rot) const;
  const std::string& getEast(int rot) const;
  const std::string& getWest(int rot) const;
  int numCities() const { return num_cities; }
  int numRoads() const { return num_roads; }
  int hasAbbey() const { return (num_cities == 0 && num_roads <= 1); }

  // for ASCII art printing
  void printRow(std::ostream &ostr, int i, int rot);

private:

  // helper function called by the constructor for printing
  void prepare_ascii_art(int rot);

  // REPRESENTATION
  std::string north_;
  std::string east_;
  std::string south_;
  std::string west_;
  int rotation;
  int num_roads;
  int num_cities;
  std::vector<std::string> ascii_art;
};


#endif