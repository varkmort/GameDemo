#pragma once

#ifndef COORD_HPP
#define COORD_HPP

#include <cmath>

struct Coord
{
	long x;
	long y;

	long DistanceTo(const Coord& other) const
	{
		long dx = x - other.x;
		long dy = y - other.y;
		return ::sqrt(dx * dx + dy * dy); 
	}	

};




#endif
