#ifndef SLICE_H
#define SLICE_H

#include <geotypes.h>
#include <unordered_map>
#include <boost/unordered_map.hpp>
#include <assert.h>

//DEBUG
#include <iostream>

typedef std::pair<int, int> Edge;
typedef boost::unordered::unordered_map<Edge, int > SegmentMap;

Point2 Intersect(const VertexBuffer& vb, const Edge& e, double zHeight)
{
	double zDiff, sliceOffset;

	sliceOffset = zHeight - vb[e.first].z;
	zDiff = vb[e.second].z - vb[e.first].z;

	assert(zDiff != 0.0);

	return Point2({ sliceOffset*(vb[e.second].x - vb[e.first].x)/zDiff + vb[e.first].x,
					sliceOffset*(vb[e.second].y - vb[e.first].y)/zDiff + vb[e.first].y});
}

void MapSegments(SegmentLinkSet& sls, PointBuffer& pb, SegmentMap& sm, const VertexBuffer& vb, const Triangle& t, double zHeight) {
	Edge a, b;
	int index_a, index_b;
	SegmentMap::iterator e;
	bool new_a, new_b;

	if (vb[t[0]].z < zHeight && vb[t[1]].z >= zHeight ||
	  vb[t[1]].z == zHeight && vb[t[2]].z == zHeight && vb[t[0]].z != zHeight) {
		a = {t[0], t[1]};
		b = {t[0], t[2]};
	} else if (vb[t[1]].z < zHeight && vb[t[2]].z > zHeight || 
	  vb[t[0]].z == zHeight && vb[t[1]].z == zHeight && vb[t[0]].z != zHeight) {
		a = {t[1], t[2]};
		b = {t[0], t[2]};
	} else {
		return;
	}

	e = sm.find(a);
	if (e == sm.end()) {
		index_a = pb.size();
		pb.push_back(Intersect(vb, a, zHeight));
		sm[a] = index_a;
		new_a = true;
	} else {
		index_a = e->second;
		new_a = false;
	}

	e = sm.find(b);
	if (e == sm.end()) {
		index_b = pb.size();
		pb.push_back(Intersect(vb, b, zHeight));
		sm[b] = index_b;
		new_b = true;
	} else {
		index_b = e->second;
		new_b = false;
	}

	if (new_a) {
		sls.push_back({index_b, -1});
	} else {
		sls[index_a].second = index_b;
	}

	if (new_b) {
		sls.push_back({index_a, -1});
	} else {
		sls[index_b].second = index_a;
	}
}

#endif
