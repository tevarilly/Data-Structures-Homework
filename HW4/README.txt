HOMEWORK 4: GROCERY LISTS


NAME:  < Taylor E Varilly >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< www.stackoverflow.com, cplusplus.com, Christopher Weir >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 28 >



ORDER NOTATION:
For each request/command,

i = # of different ingredients in the kitchen
u = maximum units of a single ingredient in the kitchen
r = # of different recipes
k = maximum # of different ingredients in a single recipe
v = maximum units of single ingredient in a single recipe
a = maximum # of different ingredients added with a single ’a’ command 
w = maximum units of a single ingredient added with a single ’a’ command


command 'r' / add a recipe:
O(r+(k^2)log(k))

command 'a' / add ingredients to the kitchen:
O(ai)

command 'p' / print a recipe:
O(rk)

command 'm' / make a recipe:
O(r+ki)

command 'k' / print the contents of the kitchen:
O(ilog(i))

command 's' / suggest recipes:
O(rlog(r)+rki)


EXTRA CREDIT:
Describe your implementation of the "suggest dinner menu" option.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


