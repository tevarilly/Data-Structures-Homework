#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>

#include "tile.h"
#include "location.h"
#include "board.h"


//Global variable that is set in main.cpp and is adjustable from the command line
int GLOBAL_TILE_SIZE = 11;

//FUNCTION PROTOTYPES
bool CheckSolution(const Board& board, const std::vector<Board>& solutions);
bool isSolutionUnique(const Board& board, const std::vector<Board>& solutions);
bool isMoveValid(Board board, Tile* tile, int i, int j, int rot);
bool isCorrectSize(const Board& board, int tileVectorSize);
bool checkTile(const Tile* tile1, const Tile* tile2, int rot1, int rot2);
bool checkBoard(const Board& board1, const Board& board2);
bool touchingBorders(Board board);


// ==========================================================================
// Helper function that is called when an error in the command line
// arguments is detected.
void usage(int argc, char* argv[])
{
    std::cerr << "USAGE: " << std::endl;
    std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>" << std::endl;
    std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -all_solutions" << std::endl;
    std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -allow_rotations" << std::endl;
    std::cerr << "  " << argv[0] << " <filename>  -all_solutions  -allow_rotations" << std::endl;
    std::cerr << "  " << argv[0] << " <filename>  -tile_size <odd # >= 11>" << std::endl;
    exit ( 1 );
}
// ==========================================================================

// ==========================================================================
//Function that finds valid tile placements adjacent to the tiles that were already placed
//"index" identifies the tile in the list of input tiles
void PlaceTiles (Board board, uint index, std::vector<Tile*>& tiles, std::vector<Board>& solutions, bool allow_rotations, bool all_solutions)
{
    //If the user didn't specify all solutions, stop after you've found one
    if(!all_solutions && solutions.size() >= 1)
    {
        return;
    }

    //For every possible rotation
    for(int rot = 0; rot <= 270; rot+=90)
    {
        #ifdef DEBUGPLACE
        std::cout<<"Try rotation "<<rot<<"\n";
        #endif

        //For every tile that has already been placed on the board
        for(uint i = 0; i < board.getLocations().size(); ++i)
        {
            //Store each location coordinate temporarily for ease of use
            Location current = board.getLocations()[i];

            //If the location above the tile last placed is valid
            if(isMoveValid( board, tiles[index], current.row - 1, current.column, rot))
            {
                #ifdef DEBUGPLACE
                    std::cout << "Placing tile # " << index << " at " << current.row -1 << "," << current.column << " with rot " << rot << "\n";
                #endif

                //Add the next tile (indicated by the index) to the location above the last tile 
                board.setTile(current.row - 1 , current.column , tiles[index], rot);

                #ifdef DEBUGPLACE
                board.Print();
                #endif

                //If you still have more tiles left to place
                if(index + 1 < tiles.size())
                {
                    //If the user didn't specify all solutions, stop after you've found one
                    if(!all_solutions && solutions.size() >= 1)
                    {
                        return;
                    }

                    //RECURSE!
                    PlaceTiles(board, index + 1, tiles, solutions, allow_rotations, all_solutions);
                }

                //Otherwise if all tiles have been placed, check if we have a valid solution
                else
                {
                    #ifdef DEBUGPOTSOLNS
                    board.Print();
                    #endif

                    //Do we have a valid solution?
                    if(CheckSolution(board,solutions))
                    {
                        //If the user didn't specify all solutions, stop after you've found one
                        if(!all_solutions && solutions.size() >= 1)
                        {
                            return;
                        }

                        //If so, add it to the solutions vector
                        solutions.push_back(board);

                        #ifdef DEBUGPRINTSOLNS
                        board.Print();
                        #endif
                    }
                }

                //Since we are placing tiles in every valid spot, we must remove tiles as we go
                board.removeTile(current.row -1, current.column);
            }

            //If the location below the tile last placed is valid
            if(isMoveValid( board, tiles[index], current.row + 1, current.column, rot))
            {
                #ifdef DEBUGPLACE
                    std::cout << "Placing tile # " << index << " at " << current.row + 1 << "," << current.column << " with rot " << rot << "\n";
                #endif

                //Add the next tile (indicated by the index) to the location below the last tile 
                board.setTile(current.row + 1 , current.column , tiles[index], rot);

                #ifdef DEBUGPLACE
                board.Print();
                #endif

                //If you still have more tiles left to place
                if(index + 1 < tiles.size())
                {
                    //If the user didn't specify all solutions, stop after you've found one
                    if(!all_solutions && solutions.size() >= 1)
                    {
                        return;
                    }

                    //RECURSE!
                    PlaceTiles(board, index + 1, tiles, solutions, allow_rotations, all_solutions);
                }

                //Otherwise if all tiles have been placed, check if we have a valid solution
                else
                {
                    #ifdef DEBUGPOTSOLNS
                    board.Print();
                    #endif

                    //Do we have a valid solution?
                    if(CheckSolution(board,solutions))
                    {
                        //If the user didn't specify all solutions, stop after you've found one
                        if(!all_solutions && solutions.size() >= 1)
                        {
                            return;
                        }

                        //If so, add it to the solutions vector
                        solutions.push_back(board);

                        #ifdef DEBUGPRINTSOLNS
                        board.Print();
                        #endif
                    }
                }

                //Since we are placing tiles in every valid spot, we must remove tiles as we go
                board.removeTile( current.row + 1, current.column );
            }

            //If the location to the left of the last tile placed is valid
            if(isMoveValid(board, tiles[index], current.row, current.column - 1, rot))
            {
                #ifdef DEBUGPLACE
                    std::cout << "Placing tile # " << index << " at " << current.row << "," << current.column - 1 << " with rot " << rot << "\n";
                #endif

                //Add the next tile (indicated by the index) to the location to the left of the last tile 
                board.setTile(current.row , current.column - 1 , tiles[index], rot);

                #ifdef DEBUGPLACE
                board.Print();
                #endif

                //If you still have more tiles left to place
                if(index + 1 < tiles.size())
                {
                    //If the user didn't specify all solutions, stop after you've found one
                    if(!all_solutions && solutions.size() >= 1)
                    {
                        return;
                    }

                    //RECURSE!
                    PlaceTiles(board, index + 1, tiles, solutions, allow_rotations, all_solutions);
                }

                //Otherwise if all tiles have been placed, check if we have a valid solution
                else
                {
                    #ifdef DEBUGPOTSOLNS
                    board.Print();
                    #endif

                    //Do we have a valid solution?
                    if(CheckSolution(board,solutions))
                    {
                        //If the user didn't specify all solutions, stop after you've found one
                        if(!all_solutions && solutions.size() >= 1)
                        {
                            return;
                        }

                        //If so, add it to the solutions vector
                        solutions.push_back(board);

                        #ifdef DEBUGPRINTSOLNS
                        board.Print();
                        #endif
                    }
                }

                //Since we are placing tiles in every valid spot, we must remove tiles as we go
                board.removeTile(current.row, current.column - 1);
            }

            //If the location to the right of the last tile placed is valid
            if(isMoveValid( board, tiles[index], current.row, current.column + 1, rot))
            {
                #ifdef DEBUGPLACE
                    std::cout << "Placing tile # " << index << " at " << current.row << "," << current.column + 1 << " with rot " << rot << "\n";
                #endif

                //Add the next tile (indicated by the index) to the location to the right of the last tile 
                board.setTile(current.row , current.column + 1 , tiles[index], rot);

                #ifdef DEBUGPLACE
                board.Print();
                #endif

                //If you still have more tiles left to place
                if(index + 1 < tiles.size())
                {
                    //If the user didn't specify all solutions, stop after you've found one
                    if(!all_solutions && solutions.size() >= 1)
                    {
                        return;
                    }

                    //RECURSE!
                    PlaceTiles(board, index + 1, tiles, solutions, allow_rotations, all_solutions);
                }

                //Otherwise if all tiles have been placed, check if we have a valid solution
                else
                {
                    #ifdef DEBUGPOTSOLNS
                    board.Print();
                    #endif

                    //Do we have a valid solution?
                    if(CheckSolution(board,solutions))
                    {
                        //If the user didn't specify all solutions, stop after you've found one
                        if(!all_solutions && solutions.size() >= 1)
                        {
                            return;
                        }

                        //If so, add it to the solutions vector
                        solutions.push_back(board);

                        #ifdef DEBUGPRINTSOLNS
                        board.Print();
                        #endif
                    }
                }

                //Since we are placing tiles in every valid spot, we must remove tiles as we go
                board.removeTile( current.row , current.column + 1 );
            }
        }

        //If we aren't accounting for rotations
        if(!allow_rotations)
        {
            //Don't increment through the outer for loop anymore
            break;
        }
    }
}
// ==========================================================================

// ==========================================================================
//Set up a board with the initial tiles
void SetupBoardHelp (Board board, std::vector<Tile*>& tiles, std::vector<Board>& solutions, bool allow_rotations, bool all_solutions)
{
    //For every rotation
    for(int rot = 0; rot <= 270; rot+=90)
    {
        #ifdef DEBUGSETUP
        std::cout<<"Try rotation "<<rot<<"\n";
        #endif

        //For every row in the board
        for(int i = 0; i < board.numRows(); ++i)
        {
            //For every column in the board
            for(int j = 0; j < board.numColumns(); ++j)
            {
                //If you can place the first tile at the current index
                if(isMoveValid(board, tiles[0], i, j, rot))
                {
                    #ifdef DEBUGSETUP
                    std::cout<<"Tile placed at " << i << "," << j << "\n";
                    #endif

                    //Place an initial tile
                    board.setTile(i,j,tiles[0],rot);

                    //Call the tile placer with index of 1 (the next tile) and the location at which we just placed a tile
                    PlaceTiles(board, 1, tiles, solutions, allow_rotations, all_solutions);

                    //Need to remove tiles as we progressively recurse
                    board.removeTile(i,j);
                }

                #ifdef DEBUGSETUP
                else
                {
                    std::cout<<"Placement " << i << "," << j << " is invalid\n";
                }
                #endif
            }
        }

        //If we aren't allowing for rotations
        if(!allow_rotations)
        {
            //Don't increment through the outer for loop anymore
            break;
        }
    }
}
// ==========================================================================

// ==========================================================================
//If a proposed solution doesn't touch both the left and right border in at least one place
//  then return false so we can discard this solution
//This is necessary because all configurations are found in our place tiles algorithm
//There will be instances where the solution will be the same, but will be shifted downward and/or right on the board
bool touchingBorders(Board board)
{
    bool touching_top = false;
    bool touching_left = false;

    //For every row in the board
    for(int i=0; i<board.numRows(); ++i)
    {
        //For every column in the board
        for(int j=0; j<board.numColumns(); ++j)
        {
            //If you're on the top row and you find a (non-NULL) tile
            if(i==0 && board.getTile(i,j) != NULL)
            {
                //Your solution is flush to the top!
                touching_top = true;
            }

            //If you're on the left column and you find a (non-NULL) tile
            if(j==0 && board.getTile(i,j) != NULL)
            {
                //Your solution is flush to the left!
                touching_left = true;
            }

            //If you already know you have a good solution
            if(touching_left && touching_top)
            {
                //Stop incrementing to maximize efficiency
                break;
            }
        }
    }

    //If the proposed solution is touching the top and left borders
    if(touching_top == true && touching_left == true)
    {
        //Save the solution
        return true;
    }

    return false;
}
// ==========================================================================

// ==========================================================================
//Function that checks if the solution found has the correct number of tiles
//This shouldn't actually happen ever, but we'll use it for quality assurance anyways
bool isCorrectSize(const Board& board, int tileVectorSize)
{
    int count = 0;

    //For every row in the board
    for(int i=0; i<board.numRows(); ++i)
    {
        //For every column in the board
        for(int j=0; j<board.numColumns(); ++j)
        {
            //If you find a valid tile
            if(board.getTile(i,j) != NULL)
            {
                //Count how many tiles are in your board
                count++;
            }
        }
    }

    //If your board has too few tiles
    if(count != tileVectorSize)
    {
        return false;
    }

    else
    {
        return true;
    }
}
// ==========================================================================

// ==========================================================================
//Check if we have a valid total board configuration
//This function is only useful once ALL the tiles are placed 
//  and does not account for the running placement of tiles
bool CheckSolution(const Board& board, const std::vector<Board>& solutions)
{
    //Remains true if we have a valid solution
    bool valid = true;

    int count = 0;

    //For every row in the board
    for ( int i = 0; i < board.numRows(); ++i )
    {
        //For every column in the board
        for ( int j = 0; j < board.numColumns(); ++j)
        {
            //If you are on an actual tile
            if(board.getTile(i,j) != NULL)
            {
                //Count how many total tiles you have for later use
                count++;

                //If you have a tile above
                //And if the south of the top tile
                //  does not match the north of the tile you're on
                if(i != 0 && board.getTile(i-1,j) != NULL && board.getTile(i,j)->getNorth(board.getRotation(i,j)) != board.getTile(i-1,j)->getSouth(board.getRotation(i-1,j)))
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you have a tile below
                //And if the north of the bottom tile
                //  does not match the south of the tile you're on,
                if(i != board.numRows()-1 && board.getTile(i+1,j) != NULL && board.getTile(i,j)->getSouth(board.getRotation(i,j)) != board.getTile(i+1,j)->getNorth(board.getRotation(i+1,j)))
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you have a tile to the left
                //And if the east of the left tile
                //  does not match the west of the tile you're on
                if(j != 0 && board.getTile(i,j-1) != NULL && board.getTile(i,j)->getWest(board.getRotation(i,j)) != board.getTile(i,j-1)->getEast(board.getRotation(i,j-1)))
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you have a tile to the right
                //And if the west of the right tile
                //  does not match the east of the tile you're on
                if(j != board.numColumns()-1 && board.getTile(i,j+1) != NULL && board.getTile(i,j)->getEast(board.getRotation(i,j)) != board.getTile(i,j+1)->getWest(board.getRotation(i,j+1)))
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you have blank board space above
                //  and if the north of your tile is not a pasture
                if(i != 0 && board.getTile(i-1,j) == NULL && board.getTile(i,j)->getNorth(board.getRotation(i,j)) != "pasture")
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you are on an upper boundary
                //  and if the north of your tile is not a pasture
                if(i == 0 && board.getTile(i,j)->getNorth(board.getRotation(i,j)) != "pasture")
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you have blank board space to the left
                //  and if the west of your tile is not a pasture
                if(j != 0 && board.getTile(i,j-1) == NULL && board.getTile(i,j)->getWest(board.getRotation(i,j)) != "pasture")
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you are on a leftmost boundary
                //  and if the left of your tile is not a pasture
                if(j == 0 && board.getTile(i,j)->getWest(board.getRotation(i,j)) != "pasture")
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you have blank board space below
                //  and if the south of your tile is not a pasture
                if(i != board.numRows()-1 && board.getTile(i+1,j) == NULL && board.getTile(i,j)->getSouth(board.getRotation(i,j)) != "pasture")
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you are on a bottommost boundary
                //  and if the south of your tile is not a pasture
                if(i == board.numRows()-1 && board.getTile(i,j)->getSouth(board.getRotation(i,j)) != "pasture")
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you have blank board space to the right
                //  and if the east of your tile is not a pasture
                if(j != board.numColumns()-1 && board.getTile(i,j+1) == NULL && board.getTile(i,j)->getEast(board.getRotation(i,j)) != "pasture")
                {
                    //Then you do not have a valid solution
                    valid = false;
                }

                //If you are on a rightmost boundary
                //  and it the east of your tile is not a pasture
                if(j == board.numColumns()-1 && board.getTile(i,j)->getEast(board.getRotation(i,j)) != "pasture")
                {
                    //Then you do not have a valid solution
                    valid = false;
                }
            }
        }
    }

    //A "lonely" tile is one which is not attached to any other tiles
    //Will use this to check for stragglers
    bool lonelyTile = true;

    //For every row in the board
    for ( int i = 0; i < board.numRows(); ++i )
    {
        //For every column in the board
        for ( int j = 0; j < board.numColumns(); ++j)
        {
            //If you are on a valid tile and if there are more than one tiles on the board
            if(board.getTile(i,j) != NULL && count > 1)
            {
                //Reset bool for every tile you find
                lonelyTile = true;

                //If it has a neighbor tile below
                if(i != board.numRows()-1 && board.getTile(i+1,j) != NULL)
                {
                    //It is not a lonely tile
                    lonelyTile = false;
                }

                //If it has a neighbor tile to the right
                if(j != board.numColumns()-1 && board.getTile(i,j+1) != NULL)
                {
                    //It is not a lonely tile
                    lonelyTile = false;
                }

                //If it has a neighbor tile above
                if(i != 0 && board.getTile(i-1,j) != NULL)
                {
                    //It is not a lonely tile
                    lonelyTile = false;
                }

                //If it has a neighbor tile to the left
                if(j != 0 && board.getTile(i,j-1) != NULL)
                {
                    //It is not a lonely tile
                    lonelyTile = false;
                }

                //If you found a lonely tile
                if(lonelyTile == true)
                {
                    //Stop looping because the board is not a valid solution
                    valid = false;

                    break;
                }
            }
        }
    }

    //If the solution you found is a valid one
    if(valid)
    {
        //If the solution you found is the same configuration as another solution you found
        if(!isSolutionUnique(board, solutions))
        {
            //Throw the solution out
            return false;
        }

        //If your solution is not flush to the top left
        if(!touchingBorders(board))
        {
            //Throw the solution out
            return false;
        }
    }

    return valid;
}
// ==========================================================================

// ==========================================================================
//Function that checks if the solution we found is already in the solutions vector
//By nature of how the recursive function is structured, we get up to thousands of repeat solutions
//Only configurations that haven't already been found are added to the vector
bool isSolutionUnique(const Board& board, const std::vector<Board>& solutions)
{
    //For every board in the solutions vector
    for (uint i = 0; i < solutions.size(); ++i)
    {
        //Helper function compares the given board and one other board from solutions, one at a time
        //Returns true if the boards are identical
        if (checkBoard(board, solutions[i]))
        {
            //Not a unique solution
            return false;
        }
    }

    //Otherwise, it is a unique solution
    return true;
}
// ==========================================================================

// ==========================================================================
//Helper function that determines if two boards are identical
//Is called from the isSolutionUnique function
bool checkBoard(const Board& board1, const Board& board2){

    //If the number of rows or columns in either of the boards is not the same, 
    //  then we automatically know they aren't equal
    if (board1.numRows() != board2.numRows() || board1.numColumns() != board2.numColumns())
    {
        //The boards are not identical
        return false;
    }

    Tile* t1, *t2;
    int rot1, rot2;

    //For every row in the first board
    for(int i=0; i<board1.numRows(); ++i)
    {
        //For every column in the first board
        for(int j=0; j<board1.numColumns(); ++j)
        {
            //Assign every tile to t1 and t2
            t1 = board1.getTile(i, j);
            t2 = board2.getTile(i, j);
            rot1 = board1.getRotation(i,j);
            rot2 = board2.getRotation(i,j);

            //If the tiles are not identical 
            //More helper functions!
            if (checkTile(t1, t2, rot1, rot2) == false)
            {
                //The boards are not identical
                return false;
            }
        }
    }

    //Otherwise, the two boards are identical
    return true;
}
// ==========================================================================

// ==========================================================================
//Helper function that determines whether two tiles are identical
//Is called from the checkBoard function
bool checkTile(const Tile* tile1, const Tile* tile2, int rot1, int rot2)
{
    //If both tiles are NULL
    if(tile1 == NULL && tile2 == NULL)
    {
        //They're technically the same
        return true;
    }

    //If both the tiles are not NULL
    else if(tile1 != NULL && tile2 != NULL)
    {
        //And all four sides of the tiles match
        if(tile1->getNorth(rot1) == tile2->getNorth(rot2) 
            && tile1->getSouth(rot1) == tile2->getSouth(rot2) 
            && tile1->getEast(rot1) == tile2->getEast(rot2) 
            && tile1->getWest(rot1) == tile2->getWest(rot2))
        {
            //The tiles are the same
            return true;
        }
    }

    //Otherwise, the tiles are not the same
    return false;
}
// ==========================================================================

// ==========================================================================
//Helper function which checks if the placement of a tile could be legal
//This helps to minimize the recursive calls and increases efficiency
bool isMoveValid(Board board, Tile* tile, int i, int j, int rot)
{
    //Don't go outside the boundried or place on a tile
    if(i >= board.numRows() || j >= board.numColumns() || i<0 || j<0 || board.getTile(i,j)!=NULL)
    {
        return false;
    }

    //Will use this for counting the total number of tiles on the board so far
    bool firstMove = true;

    //For every row in the board
    for(int k=0; k<board.numRows(); ++k)
    {
        //For every column in the board
        for(int l=0; l<board.numColumns(); ++l)
        {
            //If you find a valid tile
            if(board.getTile(k,l) != NULL)
            {
                firstMove = false;
            }
        }
    }

    //If there are no other tiles on the board
    if(firstMove == true)
    {
        //If you're on a lower boundary
        if(i == board.numRows()-1 && tile->getSouth(rot) != "pasture")
        {
            return false;
        }

        //If you're on a right boundary
        if(j == board.numColumns()-1 && tile->getEast(rot) != "pasture")
        {
            return false;
        }

        //If you're on a bottom boundary
        if(i == 0 && tile->getNorth(rot) != "pasture")
        {
            return false;
        }

        //If you're on a left boundary
        if(j == 0 && tile->getWest(rot) != "pasture")
        {
            return false;
        }

        //Any move is valid otherwise
        else
        {
            return true;
        }
    }

    //If you're on a bottom boundary
    if(i == board.numRows()-1)
    {
        //If anything but a pasture is touching the boundary
        if(tile->getSouth(rot) != "pasture")
        {
            return false;
        }
    }

    //If you're on a right boundary
    if(j == board.numColumns()-1)
    {
        //If anything but a pasture is touching the boundary
        if(tile->getEast(rot) != "pasture")
        {
            return false;
        }
    }

    //If you're on a top boundary
    if(i == 0)
    {
        //If anything but a pasture is touching the boundary
        if(tile->getNorth(rot) != "pasture")
        {
            return false;
        }
    }

    //If you're on a left boundary
    if(j == 0)
    {
        //If anything but a pasture is touching the boundary
        if(tile->getWest(rot) != "pasture")
        {
            return false;
        }
    }

    //Check up down left right for another tile
    bool adjacentToTile=false;
    bool validAdjacentTile=false;

    //Check south
    if(i != board.numRows()-1)
    {
        //If there's a tile below you
        if(board.getTile(i+1,j)!=NULL)
        {
            adjacentToTile=true;

            //Check if it's a valid match
            if(tile->getSouth(rot)==board.getTile(i+1,j)->getNorth(board.getRotation(i,j)))
            {
                validAdjacentTile=true;
            }

            else
            {
                validAdjacentTile=false;
            }
        }
    }

    //Check east
    if(j != board.numColumns()-1)
    {
        //If there's a tile to the right
        if(board.getTile(i,j+1)!=NULL)
        {
            adjacentToTile=true;

            //Check if it's a valid match
            if(tile->getEast(rot)==board.getTile(i,j+1)->getWest(board.getRotation(i,j)))
            {
                validAdjacentTile=true;
            }
            else
            {
                validAdjacentTile=false;
            }
        }
    }

    //Check north
    if(i>0)
    {
        //If there's a tile above you
        if(board.getTile(i-1,j)!=NULL)
        {
            adjacentToTile=true;

            //Check if it's a valid match
            if(tile->getNorth(rot)==board.getTile(i-1,j)->getSouth(board.getRotation(i,j)))
            {
                validAdjacentTile=true;
            }

            else
            {
                validAdjacentTile=false;
            }
        }
    }

    //Check west
    if(j>0)
    {
        //If there's a tile to the left
        if(board.getTile(i,j-1)!=NULL)
        {
            adjacentToTile=true;

            //Check if it's a valid match
            if(tile->getWest(rot)==board.getTile(i,j-1)->getEast(board.getRotation(i,j)))
            {
                validAdjacentTile=true;
            }

            else
            {
                validAdjacentTile=false;
            }
        }
    }

    return adjacentToTile && validAdjacentTile;
}
// ==========================================================================

// ==========================================================================
void HandleCommandLineArguments(int argc, char* argv[], std::string& filename,
                                  int& rows, int& columns, bool& all_solutions, bool& allow_rotations)
{
    // must at least put the filename on the command line
    if ( argc < 2 )
    {
        usage ( argc, argv );
    }

    filename = argv[1];

    // parse the optional arguments
    for ( int i = 2; i < argc; i++ )
    {
        if ( argv[i] == std::string ( "-tile_size" ) )
        {
            i++;
            assert ( i < argc );
            GLOBAL_TILE_SIZE = atoi ( argv[i] );

            if ( GLOBAL_TILE_SIZE < 11 || GLOBAL_TILE_SIZE % 2 == 0 )
            {
                std::cerr << "ERROR: bad tile_size" << std::endl;
                usage ( argc, argv );
            }
        }
        else if ( argv[i] == std::string ( "-all_solutions" ) )
        {
            all_solutions = true;
        }
        else if ( argv[i] == std::string ( "-board_dimensions" ) )
        {
            i++;
            assert ( i < argc );
            rows = atoi ( argv[i] );
            i++;
            assert ( i < argc );
            columns = atoi ( argv[i] );

            if ( rows < 1 || columns < 1 )
            {
                usage ( argc, argv );
            }
        }
        else if ( argv[i] == std::string ( "-allow_rotations" ) )
        {
            allow_rotations = true;
        }
        else
        {
            std::cerr << "ERROR: unknown argument '" << argv[i] << "'" << std::endl;
            usage ( argc, argv );
        }
    }
}
// ==========================================================================

// ==========================================================================
void ParseInputFile(int argc, char* argv[], const std::string& filename, std::vector<Tile*>& tiles)
{
    // open the file
    std::ifstream istr ( filename.c_str() );

    if ( !istr )
    {
        std::cerr << "ERROR: cannot open file '" << filename << "'" << std::endl;
        usage ( argc, argv );
    }

    assert ( istr );

    // read each line of the file
    std::string token, north, east, south, west;

    while ( istr >> token >> north >> east >> south >> west )
    {
        assert ( token == "tile" );
        Tile* t = new Tile ( north, east, south, west );
        tiles.push_back ( t );
    }
}
// ==========================================================================

// ==========================================================================
int main(int argc, char* argv[])
{
    std::string filename;
    int rows = -1;
    int columns = -1;
    bool all_solutions = false;
    bool allow_rotations = false;
    bool board_specified = true;
    
    //Load in the tiles
    std::vector<Tile*> tiles;

    HandleCommandLineArguments ( argc, argv, filename, rows, columns, all_solutions, allow_rotations );
    ParseInputFile ( argc, argv, filename, tiles );
    
    if(rows < 1 && columns < 1)
    {
        board_specified = false;
    }

    //If board dimensions aren't specified, make the board the maximum size possible
    if(board_specified == false)
    {
        rows = tiles.size();
        columns = tiles.size();
    }

    //Confirm the specified board is large enough
    if(board_specified == true  &&  rows * columns < (int)(tiles.size()) )
    {
        std::cerr << "ERROR: specified board is not large enough" << rows << "X" << columns << "=" << rows* columns << " " << tiles.size() << std::endl;
        usage ( argc, argv );
    }

    Board board (rows,columns);
    std::vector<Location> locations;
    std::vector<Board> solutions;

    SetupBoardHelp(board, tiles, solutions, allow_rotations, all_solutions);

    //If you found at least one solution
    //  and all_solutions wasn't specified
    if(solutions.size()>0 && !all_solutions)
    {
        std::cout<<"Solution: ";
        solutions[0].PrintLocations();
        std::cout<<"\n";
        solutions[0].Print();
    }

    //Otherwise, if you found at least one solution and want all solutions
    else if(solutions.size()>0 && all_solutions)
    {
        //Print out all solutions in the solutions vector
        for(uint i = 0; i < solutions.size(); ++i)
        {
            std::cout<<"Solution: ";
            solutions[i].PrintLocations();
            std::cout<<"\n";
            solutions[i].Print();
        }

        std::cout<<solutions.size()<<" solution(s) found.\n";
    }

    //Otherwise, solutions.size() must == 0
    else
    {
        std::cout<<"No Solutions.\n";
    }

    //Delete the tiles
    for(unsigned int t = 0; t < tiles.size(); t++)
    {
        delete tiles[t];
    }
}
// ==========================================================================