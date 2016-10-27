HOMEWORK 6: CARCASSONNE RECURSION


NAME:  < Taylor E Varilly >



COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< www.stackoverflow.com, cplusplus.com, Christopher Weir >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 40 >



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)
The dimensions of the board (h and w)? 
The number of tiles (t)? 
The number of road (r) and city (c) edges? 
The number of duplicate tiles?
Whether rotations are allowed?  
Etc. 

The order notation for the recursive "PlaceTiles" function is O((h*w*t!).  This is why the allowed
board sizes greatly affect the speed of the algorithm, but the number of tiles used is the most important 
factor.  The number of road and city edges have almost no affect on the speed of the algorithm simply because 
they are overshadowed by the (h*w*t!) factor. Since allowing rotation is effectively adding a constant multiplier
to this, it 


SUMMARY OF PERFORMANCE OF YOUR PROGRAM ON THE PROVIDED PUZZLES:
Puzzle 1:
	Using minumum board dimensions: 
		0 seconds
	-all_solutions and -allow_rotations:
		0 seconds
	No additional command line arguments:
		0 seconds
	
Puzzle 2:
	Using minumum board dimensions: 
		0 seconds
	No additional command line arguments:
		0.27 seconds

Puzzle 3:
	Using minumum board dimensions: 
		0 seconds
	-all_solutions and -allow_rotations:
		0.03 seconds

Puzzle 4:
	Using minumum board dimensions: 
		0.7 seconds
	Using minimum board dimensions -all_solutions and -allow_rotations:
		36.16 seconds

Puzzle 5:
	Using minumum board dimensions: 
		0 seconds
	Using minimum board dimensions -all_solutions and -allow_rotations:
		55.57 seconds

Puzzle 6:
	Using minumum board dimensions: 
		0.05 seconds
	-all_solutions:
		0.05 seconds

Puzzle 7:
	Using minumum board dimensions: 
		2.31 seconds
	-all_solutions and -allow_rotations:
		Too long

Puzzle 8:
	Using minumum board dimensions: 
		0 seconds
	-all_solutions and -allow_rotations:
		Too long

Puzzle 9:
	Using minumum board dimensions: 
		50.03 seconds
	-all_solutions and -allow_rotations:
		Too long


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)