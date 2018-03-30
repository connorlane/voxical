#include <geotypes.h>
#include <objparser.h>
#include <gpurasterizer.h>
#include <slice.h>
#include <cassert>
#include <chrono>

int main()
{	
	std::clock_t start;
    double duration;
	start = std::clock();

	ObjParser objParser;
	GpuRasterizer rasterizer;

	VertexBuffer vb;
	zMaxTriangleSet activeTriangles(vb);
	zMinTriangleSet sortedTriangles(vb);

	std::vector< std::vector<Segment2> > slices;

	objParser.SetPath("models/yoda.obj");

	double zMin, zMax;
	objParser.Parse(vb, sortedTriangles, zMin, zMax);

	// Add a dummy point to the end of the vertex buffer for comparison
	int dummyPoint = vb.size();
	vb.push_back(Point3());

	// For each triangle	
	zMinTriangleSet::iterator nextTriangleToBeExamined = sortedTriangles.begin();
	double zSlice = zMin;
	double sliceSpacing = (zMax - zMin) / 5000;

	while(nextTriangleToBeExamined != sortedTriangles.end())
	{
		// TODO: Start at the first valid slice automatically
		vb[dummyPoint] = {0, 0, zSlice};
		Triangle dummyTriangle(dummyPoint, -1, dummyPoint);

		// Find new triangles with zMin equal to or below current zSlice
		zMinTriangleSet::iterator firstTriangleEntirelyAboveZSlice = sortedTriangles.upper_bound(dummyTriangle);
		activeTriangles.insert(nextTriangleToBeExamined, firstTriangleEntirelyAboveZSlice);	
		nextTriangleToBeExamined = firstTriangleEntirelyAboveZSlice;

		// Remove all triangles with zMax below the current zSlice
		zMaxTriangleSet::iterator firstTriangleNotEntirelyBelowZSlice = activeTriangles.lower_bound(dummyTriangle);
		activeTriangles.erase(activeTriangles.begin(), firstTriangleNotEntirelyBelowZSlice);

		SegmentMap sm;
		PointBuffer pb;
		SegmentLinkSet sls;
		for (zMaxTriangleSet::iterator i = activeTriangles.begin(); i != activeTriangles.end(); i++)
			MapSegments(sls, pb, sm, vb, *i, zSlice);

		std::vector<std::vector<int> > loops;

		for (int it = 0; it < sls.size(); it++) {
			std::vector<int> indices;

			int prev_idx, idx;

			prev_idx = it;
			idx = sls[it].first;

			while (idx != -1 && idx != it){
				indices.push_back(idx);
				if (sls[idx].first == prev_idx) {
					prev_idx = idx;
					idx = sls[idx].second;
				} else {
					prev_idx = idx;
					idx = sls[idx].first;
				}
				sls[prev_idx] = {-1, -1};
			}

			std::reverse(indices.begin(), indices.end());

			prev_idx = it;
			idx = sls[it].second;

			if (idx != -1) {
				indices.push_back(it);
				sls[it] = {-1, -1};
			}

			while (idx != -1 && idx != it) {
				indices.push_back(idx);
				if (sls[idx].first == prev_idx) {
					prev_idx = idx;
					idx = sls[idx].second;
				} else {
					prev_idx = idx;
					idx = sls[idx].first;
				}
				sls[prev_idx] = {-1, -1};
			}

			if (indices.size() > 0) {
				loops.push_back(std::move(indices));
			}
		}

		int numBadLoops = 0;
		int numGoodLoops = 0;
		for (auto it = loops.begin(); it != loops.end(); it++) {
			if (it->back() == -1) {
				numBadLoops++;
			}
			if (it->back() == *(it->begin())) {
				numGoodLoops++;
			}
		}

		if (numBadLoops > 0) {
			std::cout << "Number of contours: " << loops.size() << std::endl;
			std::cout << "Number of bad: " << numBadLoops << std::endl;
			std::cout << "Number of good: " << numGoodLoops << std::endl;
		}

		rasterizer.Rasterize(pb, loops);
		//slices.push_back(std::move(segments));		

		zSlice = zSlice + sliceSpacing;

		// DEBUG
		//break;

		//std::cin.get();
	}

	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Duration: "<< duration << " seconds" << std::endl;
}

