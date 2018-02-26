#include<voxical.h>

std::vector<std::vector<Segment2> > makeslice(std::string filepath)
{
	StlParser stlParser;
	zMaxTriangleSet activeTriangles;
	std::vector<Segment2> segments;
	std::vector<std::vector<Segment2> > slices;

	stlParser.SetPath(filepath);

	zMinTriangleSet sortedTriangles = stlParser.Parse();

	// For each triangle	
	zMinTriangleSet::iterator nextTriangleToBeExamined = sortedTriangles.begin();
	int sliceIndex = 1; 
	while(nextTriangleToBeExamined != sortedTriangles.end())
	{
		double zSlice = sliceIndex * 1;
		Triangle dummyTriangle({0,0,zSlice + 0.00001},{0,0,zSlice},{0,0, zSlice - 0.00001});

		// Find new triangles with zMin equal to or below current zSlice
		zMinTriangleSet::iterator firstTriangleEntirelyAboveZSlice = sortedTriangles.upper_bound(dummyTriangle);
		activeTriangles.insert(nextTriangleToBeExamined, firstTriangleEntirelyAboveZSlice);	
		nextTriangleToBeExamined = firstTriangleEntirelyAboveZSlice;

		// Remove all triangles with zMax below the current zSlice
		zMaxTriangleSet::iterator firstTriangleNotEntirelyBelowZSlice = activeTriangles.lower_bound(dummyTriangle);
		activeTriangles.erase(activeTriangles.begin(), firstTriangleNotEntirelyBelowZSlice);

		segments.clear();
		for (zMaxTriangleSet::iterator i = activeTriangles.begin(); i != activeTriangles.end(); i++)
		{
			segments.push_back(Slice(*i, zSlice));
		}

		slices.push_back(std::move(segments));		

		sliceIndex++;
	}

	return slices;
}

