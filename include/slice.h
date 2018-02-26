#ifndef SLICE_H
#define SLICE_H

#include <geotypes.h>

//DEBUG
#include <iostream>

Segment2 Slice(const Triangle& t, double zHeight)
{
	Segment2 res({0,0}, {0,0});
	double zDiff, sliceOffset;

	if (t[0].z < zHeight && t[1].z >= zHeight)
	{
		sliceOffset = zHeight - t[0].z;
		zDiff = t[2].z - t[0].z;
		res[0] = Point2({ sliceOffset*(t[2].x - t[0].x)/zDiff + t[0].x,
		                  sliceOffset*(t[2].y - t[0].y)/zDiff + t[0].y} );
			
		zDiff = t[1].z - t[0].z;
		res[1] = Point2({ sliceOffset*(t[1].x - t[0].x)/zDiff + t[0].x,
	                     sliceOffset*(t[1].y - t[0].y)/zDiff + t[0].y} );
	}
	else if (t[1].z < zHeight && t[2].z > zHeight)
	{
		sliceOffset = zHeight - t[0].z;
		zDiff = t[2].z - t[0].z;
		res[0] = Point2({ sliceOffset*(t[2].x - t[0].x)/zDiff + t[0].x,
	                     sliceOffset*(t[2].y - t[0].y)/zDiff + t[0].y} );
			
		sliceOffset = zHeight - t[1].z;
		zDiff = t[2].z - t[1].z;
		res[1] = Point2({ sliceOffset*(t[2].x - t[1].x)/zDiff + t[1].x,
	                     sliceOffset*(t[2].y - t[1].y)/zDiff + t[1].y} );
	}	

	return res;
}

#endif
