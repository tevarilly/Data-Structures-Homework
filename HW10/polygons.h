// shapes.h
// Header that defines all the shape classes
// THROW ERROR DEFINITIONS:
//   Each shape class has its own throw error
//	 Goes in ascending order

#ifndef _POLYGONS_H_
#define _POLYGONS_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <cmath>

#include "utilities.h"

// =========================================================================
// =========================================================================
// A closed, plane figure bounded by finite, straight line segments
class Polygon
{
public:
	bool is_polygon;

	// Constructor to define the name and points vector we need
	Polygon(const std::string shapename, const std::vector<Point> allpoints) throw(int)
	{
		is_polygon = false;

		name = shapename;
		points = allpoints;

		// Successfully makes polygon if name and points can be successfully made
		is_polygon = true;
	}

	// Some helper function declarations
	std::vector<double> MakeAngleVector();
	int NumberRight();
	int NumberParallel();
	int NumberEqualSides();
	int NumberEqualAngles();
	bool HasReflex();
	bool TwoAdjacentAngles();
	bool HasAllEqualSides();
	bool HasAllEqualAngles();
	bool HasAnAcuteAngle();
	bool HasARightAngle();
	bool HasAnObtuseAngle();
	bool IsConvex();
	bool IsConcave();

	// Accessor
	std::string getName();

	// Virtual deconstructor
	virtual ~Polygon() {}

// Private member variables
private:
	std::string name;
	std::vector<Point> points;
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A polygon with 3 sides
class Triangle: public Polygon
{
public:
	bool is_triangle;

	// Constructor
	Triangle(const std::string name, const std::vector<Point> points) throw(int) : Polygon(name, points)
	{
		is_triangle = false;

		// Triangles need to have three points
		if(points.size() != 3 || !is_polygon)
		{
			throw 1;
		}

		is_triangle = true;
	}

	// Virtual deconstructor
	virtual ~Triangle() {}
	
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A polygon with 4 sides
class Quadrilateral: public Polygon
{
public:
	bool is_quadrilateral;
	
	// Constructor
	Quadrilateral(const std::string name, const std::vector<Point> points) throw(int) : Polygon(name,points) 
	{
		is_quadrilateral = false;

		// A quadrilateral only has the requirements of having 4 sides (4 points)
		if((points.size() != 4) || (!is_polygon) )
		{
			throw 2;
		}

		is_quadrilateral = true;
	}

	// Virtual deconstructor
	virtual ~Quadrilateral() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A triangle with at least two sides of equal length
class IsoscelesTriangle: virtual public Triangle
{
public:
	bool is_isosceles;

	// We can use this variable in other inherited classes
	// AKA don't need to calculate this more than once for a triangle
	int equal_sides; 

	// Constructor
	IsoscelesTriangle(const std::string name, const std::vector<Point> points) throw(int) : Triangle(name,points)
	{
		is_isosceles = false;

		// How many equal sides does it have?
		equal_sides = NumberEqualSides();
		
		// Needs to have at least 2 sides
		// 	 and, obviously, be a triangle
		if((equal_sides < 2) || (!is_triangle))
		{
			throw 3;
		}

		else
		{
			is_isosceles = true;
		}
	}

	// Virtual deconstructor
	virtual ~IsoscelesTriangle() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A triangle with one 90 degree angle
class RightTriangle: virtual public Triangle
{
public:
	bool is_right;

	// Constructor
	RightTriangle(const std::string name, const std::vector<Point> points) throw(int) : Triangle(name,points)
	{
		is_right = false;

		// Does the shape have at least one right angle?
		bool has_right = HasARightAngle();

		// Needs to be a triangle and have a 90 degree angle
		if((!has_right) || (!is_triangle))
		{
			throw 4;
		}

		else
		{
			is_right = true;
		}
	}

	// Virtual deconstructor
	virtual ~RightTriangle() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// Both a right triangle and an isosceles triangle
// Has a right angle and at least two sides of equal length
class IsoscelesRightTriangle: public RightTriangle, public IsoscelesTriangle
{
public:
	bool is_isosceles_right;

	// Constructor
	IsoscelesRightTriangle(const std::string name, const std::vector<Point> points) throw(int) : Triangle(name,points), 
		RightTriangle(name,points), IsoscelesTriangle(name,points)
	{
		is_isosceles_right = false;

		// If it is not an isosceles or a right triangle
		if((!is_right) || (!is_isosceles))
		{
			// It must, by extension, not be a right isosceles
			throw 5;
		}

		else
		{
			is_isosceles_right = true;
		}
	}

	// Virtual deconstructor
	virtual ~IsoscelesRightTriangle() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A triangle with one angle greater than 90 degrees
// By extension, the other two angles are acute
class ObtuseTriangle: virtual public Triangle
{
public:
	bool is_obtuse;

	// Constructor
	ObtuseTriangle(const std::string name, const std::vector<Point> points) throw(int) : Triangle(name,points)
	{
		is_obtuse = false;

		// Does the shape have at least one obtuse angle?
		bool has_obtuse_angle = HasAnObtuseAngle();
		
		// If none of the angles is an obtuse angle
		if(!has_obtuse_angle || !is_triangle)
		{
			throw 6;
		}
	}

	// Virtual deconstructor
	virtual ~ObtuseTriangle() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// An obtuse triangle where two of the three sides are equal in length
class IsoscelesObtuseTriangle: public ObtuseTriangle, public IsoscelesTriangle
{
public:
	bool is_isosceles_obtuse;

	// Constructor. A very large constructor
	IsoscelesObtuseTriangle(const std::string name, const std::vector<Point> points) throw(int) : Triangle(name,points), 
		ObtuseTriangle(name,points), IsoscelesTriangle(name,points) 
	{
		is_isosceles_obtuse = false;

		// Must have at least two equal sides (like an isosceles)
		//   and also have one obtuse angle
		if((NumberEqualSides() < 2) | (!HasAnObtuseAngle()) )
		{
			// It must, by extension, be an isosceles obtuse triangle
			throw 7;
		}

		else
		{
			is_isosceles_obtuse = true;
		}
	}

	// Virtual deconstructor
	virtual ~IsoscelesObtuseTriangle() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// An isosceles triangle where all three sides (and angles) are equal
class EquilateralTriangle: public IsoscelesTriangle
{
public:
	bool is_equilateral;

	// Constructor
	EquilateralTriangle(const std::string name, const std::vector<Point> points) throw(int) : Triangle(name,points), 
		IsoscelesTriangle(name,points)
	{
		is_equilateral = false;

		// If all three sides must be equal length
		//   and it also must be an isosceles
		if(!HasAllEqualSides() || !is_isosceles)
		{
			// It must, by extension, be an equilateral triangle
			throw 8;
		}

		else
		{
			is_equilateral = true;
		}
	}

	// Virtual deconstructor
	virtual ~EquilateralTriangle() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A quadrilateral where (at least) two sides are parallel
class Trapezoid: virtual public Quadrilateral
{
public:
	bool is_trapezoid;
	int number_parallel_sides;

	// Constructor
	Trapezoid(const std::string name, const std::vector<Point> points) throw(int) : Quadrilateral(name,points)
	{
		is_trapezoid = false;
		number_parallel_sides = NumberParallel();

		// A trapezoid needs to have at least 2 parallel sides
		//   and also needs to be a quadrilateral
		if((number_parallel_sides < 2) || (!is_quadrilateral) )
		{
			throw 9;
		}

		else
		{
			is_trapezoid = true;
		}
	}

	// Virtual deconstructor
	virtual ~Trapezoid() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A quadrilateral whose sides can be separated into adjacent groups of 2
// The sides in these groups have the same length
// Long story short, it looks like a kite
class Kite: virtual public Quadrilateral
{
public:
	bool is_kite;

	// We can use the number of equal angles for other inherited classes
	int number_angles;

	// Constructor
	Kite(const std::string name, const std::vector<Point> points) throw(int) : Quadrilateral(name,points)
	{
		is_kite = false;

		// How many equal angles does it have?
		number_angles = NumberEqualAngles();

		// Needs to be a quadrilateral that is not concave and has
		//	 at least 2 equal angles
		if((IsConcave()) || (number_angles < 1))
		{
			throw 10;
		}

		else
		{
			is_kite = true;
		}
	}

	// Virtual deconstructor
	virtual ~Kite() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A concave kite
class Arrow: public Quadrilateral
{
public:
	bool is_arrow;

	// We can use the number of equal angles for later inherited classes
	int number_angles;

	// Constructor
	Arrow(const std::string name, const std::vector<Point> points) throw(int) : Quadrilateral(name, points)
	{
		is_arrow = false;

		// How many equal angles does it have?
		number_angles = NumberEqualAngles();

		// Needs to be a concave quadrilateral
		if((!is_quadrilateral) || (!IsConcave()))
		{
			throw 11;
		}

		else
		{
			is_arrow = true;
		}
	}

	// Virtual deconstructor
	virtual ~Arrow() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A trapezoid with two sets of parallel sides
class Parallelogram: virtual public Trapezoid
{
public:
	bool is_parallelogram;

	// Constructor
	Parallelogram(const std::string name, const std::vector<Point> points) throw(int) : Quadrilateral(name, points), 
		Trapezoid(name, points)
	{
		is_parallelogram = false;

		// How many parallel sides does it have?
		number_parallel_sides = NumberParallel();

		// Needs to have 2 sets of parallel sides (function will return 4 if all)
		if((number_parallel_sides != 4))
		{
			throw 12;
		}

		else
		{
			is_parallelogram = true;
		}
	}

	// Virtual deconstructor
	virtual ~Parallelogram() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A symmetric trapezoid
// Has two sets of equal angles
class IsoscelesTrapezoid: virtual public Trapezoid
{
public:
	bool is_isosceles_trapezoid;

	// Constructor
	IsoscelesTrapezoid(const std::string name, const std::vector<Point> points) throw(int) : Quadrilateral(name, points), 
		Trapezoid(name, points)
	{
		is_isosceles_trapezoid = false;

		// Checks whether you have two equal angles that are adjacent to each other
		bool adjacent_angles = TwoAdjacentAngles();

		// Needs to be a trapeziod and have adjacent, equal angles
		if((!adjacent_angles) || (number_parallel_sides < 2))
		{
			throw 13;
		}

		else
		{
			is_isosceles_trapezoid = true;
		}
	}

	// Virtual deconstructor
	virtual ~IsoscelesTrapezoid() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A quadrilateral with 4 right angles
class Rectangle: public IsoscelesTrapezoid, virtual public Parallelogram
{
public:
	bool is_rectangle;

	// Constructor
	Rectangle(const std::string name, const std::vector<Point> points) throw(int) : Quadrilateral(name, points), Trapezoid(name, points), IsoscelesTrapezoid(name, points), Parallelogram(name, points)
	{
		is_rectangle = false;

		// How many right angles does it have?
		int number_right_angles = NumberRight();

		// If it has four right angles, it's a rectangle
		if(number_right_angles != 4)
		{
			throw 14;
		}

		else
		{
			is_rectangle = true;
		}
	}

	// Virtual deconstructor
	virtual ~Rectangle() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A parallelogram where all four sides are the same length
class Rhombus: public Kite, virtual public Parallelogram
{
public:
	bool is_rhombus;

	// Constructor
	Rhombus(const std::string name, const std::vector<Point> points) throw(int) : Quadrilateral(name, points), 
		Trapezoid(name, points), Kite(name, points), Parallelogram(name, points) 
	{
		is_rhombus = false;

		// How many equal length sides does it have?
		int number_equals = NumberEqualSides();

		// Needs to be a parallelogram and have 4 equal length sides
		if((!is_parallelogram) || (number_equals != 4))
		{
			throw 15;
		}

		else
		{
			is_rhombus = true;
		}
	}

	// Virtual deconstructor
	virtual ~Rhombus() {}
};
// =========================================================================
// =========================================================================


// =========================================================================
// =========================================================================
// A rhombus with all right angles
class Square: public Rhombus, public Rectangle
{
public:
	bool is_square;

	// Constructor
	Square(const std::string name, const std::vector<Point> points) throw(int) : Quadrilateral(name, points), 
		Parallelogram(name, points), Trapezoid(name, points), Rhombus(name, points), Rectangle(name, points)
	{
		is_square = false;

		// Needs to have 4 equal length sides (rhombus) and have 4 right angles (rectangle)
		if(!is_rhombus || !is_rectangle)
		{
			throw 16;
		}

		else
		{
			is_square = true;
		}
	}

	// Virtual deconstructor
	virtual ~Square() {}
};
// =========================================================================
// =========================================================================

#endif