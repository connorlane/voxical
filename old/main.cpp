#include <geotypes.h>
#include <stlparser.h>
#include <gpurasterizer.h>
#include <slice.h>

int main()
{
	StlParser stlParser;
	GpuRasterizer rasterizer;
	zMaxTriangleSet activeTriangles;
	std::vector<Segment2> segments;

	stlParser.SetPath("yoda.stl");

	zMinTriangleSet sortedTriangles = stlParser.Parse();

	// For each triangle	
	zMinTriangleSet::iterator nextTriangleToBeExamined = sortedTriangles.begin();
	int sliceIndex = 1;
	while(nextTriangleToBeExamined != sortedTriangles.end())
	{
		// TODO: Start at the first valid slice automatically
		float zSlice = sliceIndex * 0.1;
		Triangle dummyTriangle({0,0,zSlice},{0,0,zSlice},{0,0,zSlice});

		// Find new triangles with zMin equal to or below current zSlice
		zMinTriangleSet::iterator firstTriangleEntirelyAboveZSlice = sortedTriangles.upper_bound(dummyTriangle);
		activeTriangles.insert(nextTriangleToBeExamined, firstTriangleEntirelyAboveZSlice);	
		nextTriangleToBeExamined = firstTriangleEntirelyAboveZSlice;
		
		// Remove all triangles with zMax below the current zSlice
		zMaxTriangleSet::iterator firstTriangleNotEntirelyBelowZSlice = activeTriangles.lower_bound(dummyTriangle);
		activeTriangles.erase(activeTriangles.begin(), firstTriangleNotEntirelyBelowZSlice);

		std::cout << "slice: " << sliceIndex << std::endl;
		segments.clear();		
		for(zMaxTriangleSet::iterator i = activeTriangles.begin(); i != activeTriangles.end(); i++)
		{
			// TODO: Check for degenerate segments
			segments.push_back(Slice(*i, zSlice));	
			//std::cout << Slice(*i, zSlice) << std::endl;
		}
		
		rasterizer.Rasterize(segments);

		sliceIndex++;

		// DEBUG
		//break;

		std::cin.get();
	}	
}

