
#include "mesh.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <cmath>
#include <iostream>
#include <vector>

//using namespace std;
using namespace glm;
using namespace agl;

Mesh::Mesh() 
{
}

Mesh::~Mesh()
{
	delete[] m_Positions;
	delete[] m_Normals;
	delete[] m_Indices;
}

bool Mesh::loadPLY(const std::string& filename)
{
	delete[] m_Positions;
	delete[] m_Normals;
	delete[] m_Indices;

	FILE* file;
	fopen_s(&file, filename.c_str(), "rb");
	if (file == NULL)
	{
		printf("Failed to open file!\n");
		return false;
	}

	const int line_size = 255;
	char line[line_size];
	char ignore[16];
	std::vector<std::string> formats;
	while(!feof(file))
	{
		fgets(line, line_size, file);
		if (strstr(line, "end_header"))	
			break;
		else if (strstr(line, "element"))
		{
			formats.push_back("");
			if (strstr(line, "vertex"))
			{
				int num_verts = 0;
				sscanf(line, "%*s %*s %d", &num_verts);
				m_NumVertices = num_verts;
				m_Positions = new float[m_NumVertices * 3]; // todo: remove 3
				m_Normals = new float[m_NumVertices * 3]; // todo: remove 3
			}
			else if (strstr(line, "face"))
			{
				int num_faces = 0;
				sscanf(line, "%*s %*s %d", &num_faces);
				m_NumTriangles = num_faces;
				m_Indices = new unsigned int[m_NumTriangles * 3]; // todo: remove 3
			}
			else
			{
				printf("keyword not supported\n");
				return false;
			}
		}
		else if (strstr(line, "property"))
		{
			if (strstr(line, "float"))
				formats[formats.size() - 1].append("%f ");
			else if (strstr(line, "uchar"))
				formats[formats.size() - 1].append("%u %u %u"); // not handling all cases here
			else
			{
				printf("type name not recognized\n");
				return false;
			}
		}
		else
		{
		}
	}

	if (formats.size() > 2)
	{
		printf("does not support more than 2 elements\n");
		return false;
	}
	else if (formats[0].length() > 18 || formats[1].length() > 9)
	{
		printf("no support for this formatting\n");
		return false;
	}	

	formats[0].pop_back();
	formats[1] = "%*u " + formats[1];
	for (int i = 0; i < m_NumVertices * 3 && !feof(file); i+=3)
	{
		fgets(line, line_size, file);
		float a = 0;
		sscanf(line, formats[0].c_str(),
			&m_Positions[i], &m_Positions[i + 1], &m_Positions[i + 2],
			&m_Normals[i], &m_Normals[i + 1], &m_Normals[i + 2]
		);

		m_AvgPos = m_AvgPos + vec3(m_Positions[i], m_Positions[i + 1], m_Positions[i + 2]);

		if (i == 0)
		{
			m_MinBounds = vec3(m_Positions[i], m_Positions[i + 1], m_Positions[i + 2]);
			m_MaxBounds = vec3(m_Positions[i], m_Positions[i + 1], m_Positions[i + 2]);
		}
		else
		{
			m_MinBounds.x = m_Positions[i] < m_MinBounds.x ? m_Positions[i] : m_MinBounds.x;
			m_MinBounds.y = m_Positions[i + 1] < m_MinBounds.y ? m_Positions[i + 1] : m_MinBounds.y;
			m_MinBounds.z = m_Positions[i + 2] < m_MinBounds.z ? m_Positions[i + 2] : m_MinBounds.z;
			m_MaxBounds.x = m_Positions[i] > m_MaxBounds.x ? m_Positions[i] : m_MaxBounds.x;
			m_MaxBounds.y = m_Positions[i + 1] > m_MaxBounds.y ? m_Positions[i + 1] : m_MaxBounds.y;
			m_MaxBounds.z = m_Positions[i + 2] > m_MaxBounds.z ? m_Positions[i + 2] : m_MaxBounds.z;
		}
	}
	m_AvgPos = m_AvgPos / (float)m_NumVertices;
	for (int i = 0; i < m_NumTriangles * 3 && !feof(file); i+=3)
	{
		fgets(line, line_size, file);
		sscanf(line, formats[1].c_str(),
			&m_Indices[i], &m_Indices[i + 1], &m_Indices[i + 2]
		);
	}

	return true;
}

glm::vec3 Mesh::getMinBounds() const
{
	return m_MinBounds;
}

glm::vec3 Mesh::getMaxBounds() const
{
	return m_MaxBounds;
}

glm::vec3 agl::Mesh::getAvgPosition() const
{
	return m_AvgPos;
}

int Mesh::numVertices() const
{
   return m_NumVertices;
}

int Mesh::numTriangles() const
{
   return m_NumTriangles;
}

float* Mesh::positions() const
{
   return m_Positions;
}

float* Mesh::normals() const
{
   return m_Normals;
}

unsigned int* Mesh::indices() const
{
   return m_Indices;
}

