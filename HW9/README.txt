HOMEWORK 9: DISTANCE FIELDS & FAST MARCHING METHOD


NAME:  < Taylor E Varilly >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< www.stackoverflow.com, www.cplusplus.com, Christopher Weir >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 15 >



NAIVE ALGORITHM:

Order Notation: O((w^2)*(h^2)) 

Timing Experiment Data: 
	running time for tiny: 0.006s
	running time for small: 0.003s
	running time for squiggle: 0.012s
	running time for lines: 0.840s
	running time for data_structures: 1m 40.24s
	running time for dots: TOO LONG. OVER AN HOUR

Discussion:
	This method was incredibly slow due to the fact that you're comparing every
		pixel to every other pixel. Note that real time was used.


IMPROVED ALGORITHM:

Order Notation: O((w*h)+(w*h*p))

Timing Experiment Data:
	running time for tiny: 0.003s
	running time for small: 0.005s
	running time for squiggle: 0.004s
	running time for lines: 0.038s
	running time for data_structures: 7.362s
	running time for dots: 26m 4.669s

Discussion:
	This method is a slight improvement over the previous in that it only compares each 
		pixel to every black pixel, which is slightly more efficient. However, if there 
		are many black pixels, this can take a while. 


FAST MARCHING METHOD (with a map):

Order Notation: O((w*h)log(w*h))

Timing Experiment Data:
	running time for tiny: 0.003s
	running time for small: 0.001s
	running time for squiggle: 0.004s
	running time for lines: 0.038s
	running time for data_structures: 0.300s
	running time for dots: 3.489s

Discussion:
	This method implements a priority queue with O(logn) search time and O(1) time to find 
		the minimum.  This is more efficient because obviously a binary search time is 
		better than O(n). Additionally, you only compare every pixel to its 8 adjacent 
		pixels. If there are many black pixels like before, this can be significantly faster.




MISC. COMMENTS TO GRADER:  
Optional, please be concise!






