#include <geotypes.h>
#include <stlparser.h>
#include <gpurasterizer.h>
#include <slice.h>
#include <cassert>

int main()
{
	StlParser stlParser;
	GpuRasterizer rasterizer;
	zMaxTriangleSet activeTriangles;
	std::vector<Segment2> segments;
	std::vector< std::vector<Segment2> > slices;

	stlParser.SetPath("models/yoda.stl");

	zMinTriangleSet sortedTriangles = stlParser.Parse();

	// For each triangle	
	zMinTriangleSet::iterator nextTriangleToBeExamined = sortedTriangles.begin();
	int sliceIndex = 1; 
	while(nextTriangleToBeExamined != sortedTriangles.end())
	{
		// TODO: Start at the first valid slice automatically
		double zSlice = sliceIndex * 1;
		Triangle dummyTriangle({0,0,zSlice + 0.00001},{0,0,zSlice},{0,0, zSlice - 0.00001});

		// Find new triangles with zMin equal to or below current zSlice
		zMinTriangleSet::iterator firstTriangleEntirelyAboveZSlice = sortedTriangles.upper_bound(dummyTriangle);
		activeTriangles.insert(nextTriangleToBeExamined, firstTriangleEntirelyAboveZSlice);	
		nextTriangleToBeExamined = firstTriangleEntirelyAboveZSlice;

		// DEBUG
		//std::cout << "ZSlice : " << zSlice << std::endl;
		//zMinTriangleSet::iterator tri = nextTriangleToBeExamined;
		//for(int i = 0; i < 10; i++) {
		//	std::cout << *tri << std::endl;
		//	tri++;
		//}

		// Remove all triangles with zMax below the current zSlice
		zMaxTriangleSet::iterator firstTriangleNotEntirelyBelowZSlice = activeTriangles.lower_bound(dummyTriangle);
		activeTriangles.erase(activeTriangles.begin(), firstTriangleNotEntirelyBelowZSlice);

		segments.clear();
		for (zMaxTriangleSet::iterator i = activeTriangles.begin(); i != activeTriangles.end(); i++)
		{
			// TODO: Check for degenerate segments
			segments.push_back(Slice(*i, zSlice));
			//segments.push_back(Segment2({(*i)[0].x, (*i)[0].y}, {(*i)[1].x, (*i)[1].y}));
			//segments.push_back(Segment2({(*i)[1].x, (*i)[1].y}, {(*i)[2].x, (*i)[2].y}));
			//segments.push_back(Segment2({(*i)[2].x, (*i)[2].y}, {(*i)[0].x, (*i)[0].y}));
			//std::cout << Slice(*i, zSlice) << std::endl;
		}

		rasterizer.Rasterize(segments);
		//slices.push_back(std::move(segments));		

		sliceIndex++;

		// DEBUG
		//break;

		//std::cin.get();
	}
}

