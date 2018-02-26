#ifndef STLPARSER_H
#define STLPARSER_H

#include <geotypes.h>

#include <set>
#include <iostream>
#include <fstream>
#include <string>

#include <cstdint>

class StlParser
{
	private:
		std::string m_filepath;

	public:
		// Default constructor
		StlParser()
		{
		}

		// Constructor with string
		StlParser(std::string filepath) :
			m_filepath(std::move(filepath))
		{
		}
	
		// Copy constructor
		StlParser(const StlParser& src) :
			m_filepath(src.m_filepath)
		{
		}

		// Move constructor
		StlParser(StlParser&& src) noexcept :
			m_filepath(std::move(src.m_filepath))
		{
		}

		// Move assignment operator
		StlParser& operator=(StlParser&& src)
		{
			m_filepath = std::move(src.m_filepath);
	
			return *this;
		}

		// Assignment operator
		StlParser& operator=(const StlParser& src)
		{
			m_filepath = src.m_filepath;	

			return *this;
		}

		// Descructor
		~StlParser()
		{
		}

		void SetPath(std::string filepath)
		{
			m_filepath = std::move(filepath);
		}

		zMinTriangleSet Parse()
		{
			zMinTriangleSet triangles;
			std::ifstream file(m_filepath, std::ios::in | std::ios::binary | std::ios::ate);
			uint32_t numFacets = 0;
			
			if (file.is_open())
			{
				char buffer[4];

				//std::streampos size = file.tellg();

				file.seekg(80, std::ios::beg);
				file.read(buffer, 4);

				for (int i = 0; i < 4; i++)
					numFacets |= ((unsigned char)buffer[i] << 8*i);

				for (uint32_t i = 0; i < numFacets; i++)
				{
					Triangle t;
					float x, y, z;

					// Skip the normal vector
					file.seekg(12, std::ios::cur);

					// Read the three verteces
					for (int vertex = 0; vertex < 3; vertex++)
					{
						file.read((char*)&x, 4);
						file.read((char*)&y, 4);
						file.read((char*)&z, 4);	

						t[vertex].x = static_cast<double>(x);
						t[vertex].y = static_cast<double>(y);
						t[vertex].z = static_cast<double>(z);
					}
					
					// Sort the verteces
					t.Sort();

					assert(t[0].z < t[1].z + 0.0001 && t[1].z < t[2].z + 0.0001);

					// Insert the triangle into the set
					triangles.insert(t);

					// Skip the attribute byte count 
					file.seekg(2, std::ios::cur);	
				}	

				file.close();
			}
			
			return triangles;
		}
};

#endif
