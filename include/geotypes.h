#ifndef GEOTYPES_H
#define GEOTYPES_H

#include <set>
#include <cassert>
#include <algorithm>
#include <iostream>

struct Point2 {
	double x;
	double y;

	Point2& operator=(const Point2& src)
	{
		x = src.x;
		y = src.y;	
	
		return *this;
	}
};

std::ostream& operator<<(std::ostream& os, const Point2& p)
{
	return os << '{' << p.x << ", " << p.y << '}';
}

struct Point3 
{
	double x;
	double y;
	double z;
};

std::ostream& operator<<(std::ostream& os, const Point3& p)
{
	return os << '{' << p.x << ", " << p.y << ", " << p.z << '}';
}

typedef std::vector<Point3> VertexBuffer;

class Triangle
{
	private:
		bool m_flipped;
		int m_vertices[3];

	public:
		int const& operator[](const int index) const
		{
			// In debug version, make sure index in in range
			assert(index < 3 && index >= 0);

			return m_vertices[index];	
		}

		int& operator[](const int index)
		{
			// In debug version, make sure index in in range
			assert(index < 3 && index >= 0);

			return m_vertices[index];	
		}

		Triangle() :
			m_flipped(false) {
		}

		void Sort(VertexBuffer& vb)
		{
			// Fast sort by Z
			if (vb[m_vertices[0]].z > vb[m_vertices[1]].z)
			{
				std::swap(m_vertices[0], m_vertices[1]);
				m_flipped = !m_flipped;
			}
			if (vb[m_vertices[0]].z > vb[m_vertices[2]].z)
			{
				std::swap(m_vertices[0], m_vertices[2]);
				m_flipped = !m_flipped;
			}
			if (vb[m_vertices[1]].z > vb[m_vertices[2]].z)
			{
				std::swap(m_vertices[1], m_vertices[2]);
				m_flipped = !m_flipped;
			}
		} 

		Triangle(const int& a, const int& b, const int& c) :
			m_flipped(false),
			m_vertices{a, b, c} {
		}

		bool isFlipped()
		{
			return m_flipped;
		}

	private:
		friend std::ostream& operator<<(std::ostream& os, const Triangle& t);
};

std::ostream& operator<<(std::ostream& os, const Triangle& t)
{
	return os << "[ " << t.m_vertices[0] << ",\n"
			  << "  " << t.m_vertices[1] << ",\n"
	          << "  " << t.m_vertices[2] << " ]";
}

//typedef struct Point3[2] Segment3;
//typedef Point2 Segment2[2];

class Segment2
{
private:
	Point2 m_points[2];

public:
	Segment2(const Point2& a, const Point2& b) :
		m_points{a, b}
	{
	}

	Point2 const& operator[](const int index) const
	{
		// In debug version, make sure index in in range
		assert(index < 2 && index >= 0);

		return m_points[index];	
	}

	Point2& operator[](const int index)
	{
		// In debug version, make sure index in in range
		assert(index < 2 && index >= 0);

		return m_points[index];	
	}

	void Sort()
	{
		if (m_points[0].y > m_points[1].y)
			std::swap(m_points[0], m_points[1]);
	}

private:
	friend std::ostream& operator<<(std::ostream& os, const Segment2& s);
};

std::ostream& operator<<(std::ostream& os, const Segment2& s)
{
	return os << "[ " << s.m_points[0] << ",\n"
				 << "  " << s.m_points[1] << " ]";
}

template <int vertex>
class CompareByZ
{
	private:
		VertexBuffer& m_vertices;

	public:
		CompareByZ(VertexBuffer& indices) : m_vertices(indices) {
		}

		bool operator() (const Triangle& a, const Triangle& b) const
		{
			return (m_vertices[a[vertex]].z < m_vertices[b[vertex]].z);
		}
};

typedef std::multiset<Triangle, CompareByZ<0> > zMinTriangleSet;
typedef std::multiset<Triangle, CompareByZ<2> > zMaxTriangleSet;

typedef std::vector<Point2> PointBuffer;
typedef std::vector<std::pair<int, int>> SegmentLinkSet;

#endif