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

class Triangle
{
	private:
		bool m_flipped;
		Point3 m_verteces[3];

	public:
		Point3 const& operator[](const int index) const
		{
			// In debug version, make sure index in in range
			assert(index < 3 && index >= 0);

			return m_verteces[index];	
		}

		Point3& operator[](const int index)
		{
			// In debug version, make sure index in in range
			assert(index < 3 && index >= 0);

			return m_verteces[index];	
		}

		void Sort()
		{
			// Fast sort by Z
			if (m_verteces[0].z > m_verteces[1].z)
			{
				std::swap(m_verteces[0], m_verteces[1]);
				m_flipped = !m_flipped;
			}
			if (m_verteces[0].z > m_verteces[2].z)
			{
				std::swap(m_verteces[0], m_verteces[2]);
				m_flipped = !m_flipped;
			}
			if (m_verteces[1].z > m_verteces[2].z)
			{
				std::swap(m_verteces[1], m_verteces[2]);
				m_flipped = !m_flipped;
			}
		}

		Triangle(const Point3& a, const Point3& b, const Point3& c) :
			m_flipped(false),
			m_verteces{a, b, c}
		{
		}

		Triangle() :
			m_flipped(false)
		{
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
	return os << "[ " << t.m_verteces[0] << ",\n"
				 << "  " << t.m_verteces[1] << ",\n"
	          << "  " << t.m_verteces[2] << " ]";
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
struct CompareByZ
{
   bool operator() (const Triangle& a, const Triangle& b) const
	{
		return (a[vertex].z < b[vertex].z);
	}
};

template <int vertex>
struct CompareByY
{
   bool operator() (const Segment2& a, const Segment2& b) const
	{
		return (a[vertex].y < b[vertex].y);
	}
};

template <int vertex>
struct CompareByX
{
   bool operator() (const Segment2& a, const Segment2& b) const
	{
		return (a[vertex].x < b[vertex].x);
	}
};

typedef std::multiset<Triangle, CompareByZ<0> > zMinTriangleSet;
typedef std::multiset<Triangle, CompareByZ<2> > zMaxTriangleSet;

typedef std::multiset<Segment2, CompareByY<0> > yMinSegmentSet;
typedef std::multiset<Segment2, CompareByY<1> > yMaxSegmentSet;

#endif

