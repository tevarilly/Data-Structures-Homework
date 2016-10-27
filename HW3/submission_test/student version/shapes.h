//shapes.h
//Class that defines global shape matrices (kept constant so they may not be altered)
//Should only need to be accessed from block class

#ifndef __shapes_h_
#define __shapes_h_

//Default array for 'I' shape
const char ROD[][4] ={{'I',' ',' ',' '},
					  {'I',' ',' ',' '},
					  {'I',' ',' ',' '},
					  {'I',' ',' ',' '}
					 };

//Default array for 'O' shape
const char SQR[][4] ={{'O','O',' ',' '},
					  {'O','O',' ',' '},
					  {' ',' ',' ',' '},
					  {' ',' ',' ',' '}
					 };

//Default array for 'T' shape
const char TEE[][4] ={{'T','T','T',' '},
					  {' ','T',' ',' '},
					  {' ',' ',' ',' '},
					  {' ',' ',' ',' '}
					 };

//Default array for 'Z' shape
const char ZEE[][4] ={{'Z','Z',' ',' '},
					  {' ','Z','Z',' '},
					  {' ',' ',' ',' '},
					  {' ',' ',' ',' '}
					 };

//Default array for 'S' shape
const char ESS[][4] ={{' ','S','S',' '},
					  {'S','S',' ',' '},
					  {' ',' ',' ',' '},
					  {' ',' ',' ',' '}
					 };

//Default array for 'L' shape
const char ELL[][4] ={{'L',' ',' ',' '},
					  {'L',' ',' ',' '},
					  {'L','L',' ',' '},
					  {' ',' ',' ',' '}
					 };

//Default array for 'J' shape
const char JAY[][4] ={{' ','J',' ',' '},
					  {' ','J',' ',' '},
					  {'J','J',' ',' '},
					  {' ',' ',' ',' '}
					 };

#endif