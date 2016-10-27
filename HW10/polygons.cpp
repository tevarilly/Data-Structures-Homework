#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>

#include "polygons.h"

// Make and return vector storing the three (or four) angles of the shape
std::vector<double> Polygon::MakeAngleVector()
{
	std::vector<double> angles;

	// If you have a triangle
	if(points.size() == 3)
	{
		double Angle1;
		double Angle2;
		double Angle3;

		// All three angles
		Angle1 = Angle(points[2],points[0],points[1]);
		Angle2 = Angle(points[0],points[1],points[2]);
		Angle3 = Angle(points[1],points[2],points[0]);

		// Add the angles to the vector
		angles.push_back(Angle1);
		angles.push_back(Angle2);
		angles.push_back(Angle3);
	}

	// If you have a quadrilateral
	if(points.size() == 4)
	{
		double Angle1;
		double Angle2;
		double Angle3;
		double Angle4;

		// All four angles
		Angle1 = Angle(points[3],points[0],points[1]);
		Angle2 = Angle(points[0],points[1],points[2]);
		Angle3 = Angle(points[1],points[2],points[3]);
		Angle4 = Angle(points[2],points[3],points[0]);

		// Add the angles to the vector
		angles.push_back(Angle1);
		angles.push_back(Angle2);
		angles.push_back(Angle3);
		angles.push_back(Angle4);
	}

	return angles;
}

// Function that returns the number of right angles in the shape
int Polygon::NumberRight()
{
	int right_angle_count = 0;
	std::vector<double> angles = MakeAngleVector();

	// Go through angles vector
	for(uint i=0; i<angles.size(); ++i)
	{
		// If any of the angles is a right angle
		if(RightAngle(angles[i]))
		{
			// Add the number of right angles
			right_angle_count += 1;
		}
	}

	return right_angle_count;
}

// Function that returns the number of sides that are parallel to each other
// (Can only be 0, 2, or 4)
int Polygon::NumberParallel()
{
	// No triangle will have parallel sides
	assert (points.size() == 4);

	int parallel_count = 0;

	// Construct all the sides between the points
	Vector side_1(points[0],points[1]);
	Vector side_2(points[1],points[2]);
	Vector side_3(points[2],points[3]);
	Vector side_4(points[3],points[0]);

	// A whole lot of 'if' statements that compare every side length to every other side length
	if(Parallel(side_1,side_2))
	{
		parallel_count += 1;
	}

	if(Parallel(side_1,side_3))
	{
		parallel_count += 1;
	}

	if(Parallel(side_1,side_4))
	{
		parallel_count += 1;
	}

	if(Parallel(side_2,side_3))
	{
		parallel_count += 1;
	}

	if(Parallel(side_2,side_4))
	{
		parallel_count += 1;
	}

	if(Parallel(side_3,side_4))
	{
		parallel_count += 1;
	}

	// No sides are parallel
	if(parallel_count == 0)
	{
		return 0;
	}

	// Two sides are parallel to each other
	else if(parallel_count == 1)
	{
		return 2;
	}

	// Two sets of sides are parallel
	else
	{
		assert (parallel_count == 2);
		return 4;
	}

	// We shouldn't get to this
	return 0;
}

// Function that returns the number of sides that are equal in length
int Polygon::NumberEqualSides()
{
	int side_count = 0;

	// If you have a triangle
	if(points.size() == 3)
	{
		// The three sides of the triangle
		Vector side_1(points[0],points[1]);
		Vector side_2(points[1],points[2]);
		Vector side_3(points[2],points[0]);

		if( EqualSides(side_1.Length(), side_2.Length()) )
		{
			side_count += 1;
		}

		if( EqualSides(side_2.Length(), side_3.Length()) )
		{
			side_count += 1;
		}

		if( EqualSides(side_3.Length(), side_1.Length()) )
		{
			side_count += 1;
		}

		// None of the sides are equal
		if(side_count == 0)
		{
			return 0;
		}

		// If two sides are equal to each other
		else if(side_count == 1)
		{
			return 2;
		}

		// Note: side_count should never be 2
		else
		{
			assert (side_count == 3);
			return 3;
		}
	}

	// If you have a quadrilateral
	else if (points.size() == 4)
	{
		// The four sides of the quadrilateral
		Vector side_1(points[0],points[1]);
		Vector side_2(points[1],points[2]);
		Vector side_3(points[2],points[3]);
		Vector side_4(points[3],points[0]);

		if(side_1.Length() == side_2.Length())
		{
			side_count += 1;
		}

		if(side_1.Length() == side_3.Length())
		{
			side_count += 1;
		}

		if(side_1.Length() == side_4.Length())
		{
			side_count += 1;
		}

		if(side_2.Length() == side_3.Length())
		{
			side_count += 1;
		}

		if(side_2.Length() == side_4.Length())
		{
			side_count += 1;
		}

		if(side_3.Length() == side_4.Length())
		{
			side_count += 1;
		}

		// None of the sides are equal
		if(side_count == 0)
		{
			return 0;
		}

		// If one pair of sides are equal
		else if(side_count == 1)
		{
			return 1;
		}

		else if (side_count == 2)
		{
			return 2;
		}

		// If three sides are equal, half the cases will pass
		else if(side_count == 3)
		{
			return 3;
		}

		// If all the sides are equal
		else
		{
			assert (side_count == 6);
			return 4;
		}
	}

	// We shouldn't get to this
	return 0;
}

// Function that returns the number of equal angles in a shape
// The return value will vary based on the shape type (triangle, quadrilateral)
// 		and the number of equal angles
int Polygon::NumberEqualAngles()
{
	std::vector<double> angles = MakeAngleVector();
	int equal_angle_count = 0;

	// Compare every angle
	for(uint i=0; i<angles.size(); ++i)
	{
		// to every other angle
		for(uint j=0; j<angles.size(); ++j)
		{
			// If they are equal
			if(EqualAngles(angles[i],angles[j]))
			{
				// Note: the end value of this will
				// 		seem weird because you're 
				//		comparing every angle to 
				//		every other angle, including itself
				equal_angle_count += 1;
			}
		}
	}

	// Triangle
	if(points.size() == 3)
	{
		// No angles are equal
		if(equal_angle_count == 3)
		{
			return 0;
		}

		// Two angles equal
		if(equal_angle_count == 5)
		{
			return 2;
		}

		// All angles equal
		if(equal_angle_count == 9)
		{
			return 3;
		}
	}
 
	// Quadrilateral
	if(points.size() == 4)
	{
		// No angles are equal
		if(equal_angle_count == 4)
		{
			return 0;
		}

		// Two angles are equal
		if(equal_angle_count == 6)
		{
			return 1;
		}

		// Two sets of angles are equal
		if(equal_angle_count == 8)
		{
			return 2;
		}

		// Three angles are equal
		if(equal_angle_count == 10)
		{
			return 3;
		}

		// All angles are equal
		if(equal_angle_count == 16)
		{
			return 4;
		}
	}

	return 0;
}

// Function that returns whether the shape has over a 180 degree angle
bool Polygon::HasReflex()
{
	std::vector<double> angles = MakeAngleVector();

	for(uint i=0; i<angles.size(); ++i)
	{
		if(ReflexAngle(angles[i]))
		{
			return true;
		}
	}

	return false;
}

// Function that checks whether the quadrilateral has at least two equal and ajacent angles
bool Polygon::TwoAdjacentAngles()
{
	assert (points.size() == 4);
	std::vector<double> angles = MakeAngleVector();

	bool adjacent = false;

	if((angles[0] == angles[1]) || (angles[1] == angles[2])
		|| (angles[2] == angles[3]) || (angles[3] == angles[0]))
	{
		adjacent = true;
	}

	return adjacent;
}

// Returns true if a shape has sides of all equal length
bool Polygon::HasAllEqualSides()
{
	if((points.size() == 4) && (NumberEqualSides() == 4))
	{
		return true;
	}

	else if((points.size() == 3) && (NumberEqualSides() == 3))
	{
		return true;
	}

	else
	{
		return false;
	}
}

// Returns true if a shape has angles of all equal values
bool Polygon::HasAllEqualAngles()
{
	// Quadrilateral
	if(points.size() == 4)
	{
		if(NumberEqualAngles() == 4 && !IsConcave())
		{
			return true;
		}
	}

	// Triangle
	else if(points.size() == 3)
	{
		if(NumberEqualAngles() == 3)
		{
			return true;
		}
	}

	return false;
}

// Returns true if a shape has at least one right angle
bool Polygon::HasARightAngle()
{
	if(NumberRight() > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

// Returns true if a shape has at least one acute angle
bool Polygon::HasAnAcuteAngle()
{
	std::vector<double> angles = MakeAngleVector();

	for(uint i=0; i<angles.size(); ++i)
	{
		if(AcuteAngle(angles[i]))
		{
			return true;
		}
	}

	return false;
}

// Function that checks whether a triangle has an obtuse angle
bool Polygon::HasAnObtuseAngle()
{
	std::vector<double> angles = MakeAngleVector();

	for(uint i=0; i<angles.size(); ++i)
	{
		if(ObtuseAngle(angles[i]))
		{
			return true;
		}
	}

	return false;
}

// Returns true if a shape is convex
bool Polygon::IsConvex()
{
	if(!HasReflex())
	{
		return true;
	}

	else
	{
		return false;
	}
}

// Returns true if a shape is concave
bool Polygon::IsConcave()
{
	if(HasReflex())
	{
		return true;
	}

	else
	{
		return false;
	}
}

// Accessor
std::string Polygon::getName()
{
	return name;
}