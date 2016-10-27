#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "tile.h"


// Fill in characters for the ASCII art 
#define ROAD_CHAR '#'
#define CITY_CHAR '.'


// ==========================================================================
// this global variable is set in main.cpp and is adjustable from the command line
// (you are not allowed to make your own global variables)
extern int GLOBAL_TILE_SIZE;


// ==========================================================================
// CONSTRUCTOR
// takes in 4 strings, checks the legality of the labeling 
Tile::Tile(const std::string &north, const std::string &east,
           const std::string &south, const std::string &west) :
  north_(north), east_(east), south_(south), west_(west) {

  // check the input strings
  assert (north_ == "city" || north_ == "road" || north_ == "pasture");
  assert (east_  == "city" || east_  == "road" || east_  == "pasture");
  assert (south_ == "city" || south_ == "road" || south_ == "pasture");
  assert (west_  == "city" || west_  == "road" || west_  == "pasture");

  // count the number of cities and roads
  num_cities = 0;
  if (north_ == "city") num_cities++;
  if (south_ == "city") num_cities++;
  if (east_ == "city") num_cities++;
  if (west_ == "city") num_cities++;
  num_roads = 0;
  if (north_ == "road") num_roads++;
  if (south_ == "road") num_roads++;
  if (east_ == "road") num_roads++;
  if (west_ == "road") num_roads++;
  
  // For our version of Carcassonne, we put these restrictions on the
  // tile edge labeling:
  if (num_roads == 1) {
    assert (num_cities == 0 || num_cities == 3);
  }
  if (num_roads == 2 && num_cities == 2) {
    assert (north_ == east_ || north_ == west_);
  }

  // pre-compute the ASCII art center of the tile
  rotation=0;
  prepare_ascii_art(0);
}

const std::string& Tile::getNorth(int rot) const
{
  if(rot == 0)
  {
    return north_;
  }
  else if(rot == 90)
  {
    return west_;
  }
  else if(rot ==  180)
  {
    return south_;
  }
  else if(rot == 270)
  {
    return east_;
  }
  else 
  {
    return north_;
  }
}
const std::string& Tile::getSouth(int rot) const
{
  if(rot == 0)
  {
    return south_;
  }
  else if(rot == 90)
  {
    return east_;
  }
  else if(rot ==  180)
  {
    return north_;
  }
  else if(rot == 270)
  {
    return west_;
  }
  else 
  {
    return south_;
  }
}
const std::string& Tile::getEast(int rot) const
{
  if(rot == 0)
  {
    return east_;
  }
  else if(rot == 90)
  {
    return north_;
  }
  else if(rot ==  180)
  {
    return west_;
  }
  else if(rot == 270)
  {
    return south_;
  }
  else 
  {
    return east_;
  }
}
const std::string& Tile::getWest(int rot) const
{
  if(rot == 0)
  {
    return west_;
  }
  else if(rot == 90)
  {
    return south_;
  }
  else if(rot ==  180)
  {
    return east_;
  }
  else if(rot == 270)
  {
    return north_;
  }
  else 
  {
    return west_;
  }
}


// ==========================================================================
// print one row of the tile at a time 
// (allows a whole board of tiles to be printed)
void Tile::printRow(std::ostream &ostr, int row, int rot) {
  // must be a legal row for this tile size
  assert (row >= 0 && row < GLOBAL_TILE_SIZE);

  if (row == 0 || row == GLOBAL_TILE_SIZE-1) {
    ostr << '+' << std::string(GLOBAL_TILE_SIZE-2,'-') << '+';
  } else {
    if(rot!=rotation)
    {
      prepare_ascii_art(rot);
      rotation=rot;
    }
    ostr << '|' << ascii_art[row-1] << '|';
  }
}


// ==========================================================================
// long, messy, uninteresting function that
// prepares the inner block of ASCII art for the tile
void Tile::prepare_ascii_art(int rot) {

  // tiles have to be odd sized
  assert (GLOBAL_TILE_SIZE % 2 == 1);
  // tiles must be big enough to the ascii art is visible
  assert (GLOBAL_TILE_SIZE >= 11);

  // helper variables
  int inner_size = GLOBAL_TILE_SIZE-2;
  int half = inner_size/2;
  int city_depth = (half+2) / 2;
  assert (city_depth >= 3);
  int road_curve = city_depth-1;
  ascii_art = std::vector<std::string>(inner_size,std::string(inner_size,' '));

  // -------------------------------------------------------------------
  // ROADS

  // Does a road go straight vertically or horizontally through the tile?
  bool center_road = false;
  if ((getNorth(rot) == "road" && getSouth(rot) == "road") ||
       (getEast(rot) == "road" && getWest(rot) == "road")) {
    center_road = true;
    ascii_art[half][half] = ROAD_CHAR;
  }

  // Construct the road fragments from edge towards the center of the tile
  if (getNorth(rot) == "road") {
    for (int i = 0; i < half-1; i++) {
      ascii_art[i][half] = ROAD_CHAR;
    }
    if (center_road) {
      ascii_art[half-1][half] = ROAD_CHAR;
    }
  }
  if (getSouth(rot) == "road") {
    for (int i = half+2; i < inner_size; i++) {
      ascii_art[i][half] = ROAD_CHAR;
    }
    if (center_road) {
        ascii_art[half+1][half] = ROAD_CHAR;
    }
  }
  if (getWest(rot) == "road") {
    for (int i = 0; i < half-1; i++) {
      ascii_art[half][i] = ROAD_CHAR;
    }
    if (center_road) {
      ascii_art[half][half-1] = ROAD_CHAR;
    }
  }
  if (getEast(rot) == "road") {
    for (int i = half+2; i < inner_size; i++) {
      ascii_art[half][i] = ROAD_CHAR;
    }
    if (center_road) {
      ascii_art[half][half+1] = ROAD_CHAR;
    }
  }

  // Construct the curved pieces of "corner" roads
  if (!center_road) {
    if (getNorth(rot) == "road" && getEast(rot) == "road") {
      for (int c = 1; c < road_curve; c++) {
        ascii_art[half-c][half+road_curve-c] = ROAD_CHAR;
        ascii_art[half-c][half+road_curve-c] = ROAD_CHAR;
        ascii_art[half-c][half] = ' ';
        ascii_art[half][half+road_curve-c] = ' ';
      }
    }
    if (getEast(rot) == "road" && getSouth(rot) == "road") {
      for (int c = 1; c < road_curve; c++) {
        ascii_art[half+c][half+road_curve-c] = ROAD_CHAR;
        ascii_art[half+c][half+road_curve-c] = ROAD_CHAR;
        ascii_art[half+c][half] = ' ';
        ascii_art[half][half+road_curve-c] = ' ';
      }
    }
    if (getSouth(rot) == "road" && getWest(rot) == "road") {
      for (int c = 1; c < road_curve; c++) {
        ascii_art[half+c][half-road_curve+c] = ROAD_CHAR;
        ascii_art[half+c][half-road_curve+c] = ROAD_CHAR;
        ascii_art[half+c][half] = ' ';
        ascii_art[half][half-road_curve+c] = ' ';
      }
    }
    if (getWest(rot) == "road" && getNorth(rot) == "road") {
      for (int c = 1; c < road_curve; c++) {
        ascii_art[half-c][half-road_curve+c] = ROAD_CHAR;
        ascii_art[half-c][half-road_curve+c] = ROAD_CHAR;
        ascii_art[half-c][half] = ' ';
        ascii_art[half][half-road_curve+c] = ' ';
      }
    }
  }


  // -------------------------------------------------------------------
  // CITIES
  // construct the curved wedges of cities for each edge
  if (getNorth(rot) == "city") {
    int depth = city_depth;
    if (getEast(rot) == "city" || getWest(rot) == "city") {
      depth = half;
    }
    for (int i = 0; i < depth; i++) {
      for (int j = i+1; j < inner_size-i-1; j++) {
        ascii_art[i][j] = CITY_CHAR;
      }
    }
  }
  if (getSouth(rot) == "city") {
    int depth = city_depth;
    if (getEast(rot) == "city" || getWest(rot) == "city") {
      depth = half;
    }
    for (int i = 0; i < depth; i++) {
      for (int j = i+1; j < inner_size-i-1; j++) {
        ascii_art[GLOBAL_TILE_SIZE-3-i][j] = CITY_CHAR;
      }
    }
  }
  if (getWest(rot) == "city") {
    int depth = city_depth;
    if (getNorth(rot) == "city" || getSouth(rot) == "city") {
      depth = half;
    }
    for (int i = 0; i < depth; i++) {
      for (int j = i+1; j < inner_size-i-1; j++) {
        ascii_art[j][i] = CITY_CHAR;
      }
    }
  }
  if (getEast(rot) == "city") {
    int depth = city_depth;
    if (getNorth(rot) == "city" || getSouth(rot) == "city") {
      depth = half;
    }
    for (int i = 0; i < depth; i++) {
      for (int j = i+1; j < inner_size-i-1; j++) {
        ascii_art[j][GLOBAL_TILE_SIZE-3-i] = CITY_CHAR;
      }
    }
  }

  // If there are at least 3 wedges of city, fill in the middle as city
  if (num_cities >= 3) {
    for (int i = city_depth; i < inner_size-city_depth; i++) {
      for (int j = city_depth; j < inner_size-city_depth; j++) {
        ascii_art[i][j] = CITY_CHAR;
      }
    }
  } 

  // If there are 2 neighboring wedges of city, fill in the gap
  if (getNorth(rot) == "city" && getWest(rot) == "city") {
    for (int i = 0; i < half; i++) {
      ascii_art[i][i] = CITY_CHAR;
    }
  }
  if (getNorth(rot) == "city" && getEast(rot) == "city") {
    for (int i = 0; i < half; i++) {
      ascii_art[i][GLOBAL_TILE_SIZE-3-i] = CITY_CHAR;
    }
  }
  if (getSouth(rot) == "city" && getWest(rot) == "city") {
    for (int i = 0; i < half; i++) {
      ascii_art[GLOBAL_TILE_SIZE-3-i][i] = CITY_CHAR;
    }
  }
  if (getSouth(rot) == "city" && getEast(rot) == "city") {
    for (int i = 0; i < half; i++) {
      ascii_art[GLOBAL_TILE_SIZE-3-i][GLOBAL_TILE_SIZE-3-i] = CITY_CHAR;
    }
  }
  
  // -------------------------------------------------------------------
  // DRAW THE ABBEY BUILDING
  if (hasAbbey()) {
    ascii_art[half-2][half] = '^';
    ascii_art[half-1][half-1] = '/';
    ascii_art[half-1][half  ] = ' ';
    ascii_art[half-1][half+1] = '\\';
    ascii_art[half  ][half-2] = '/';
    ascii_art[half  ][half-1] = '|';
    ascii_art[half  ][half  ] = ' ';
    ascii_art[half  ][half+1] = '|';
    ascii_art[half  ][half+2] = '\\';
    ascii_art[half+1][half-1] = '|';
    ascii_art[half+1][half  ] = ' ';
    ascii_art[half+1][half+1] = '|';
    ascii_art[half+2][half-1] = '+';
    ascii_art[half+2][half  ] = '-';
    ascii_art[half+2][half+1] = '+';
  }
}

// ==========================================================================