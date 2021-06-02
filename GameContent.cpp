// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameContent.h"
#include <fstream>
#include <sstream>





// METHODS ////////////////////////////////////////////////////////////////////
void GameContent::Clear()
{
	texMan.Clear();
}

GameContent::GameContent()
{
}

GameContent::~GameContent()
{
	Clear();
}

bool GameContent::Load(std::string filename, GameTexture2D &content)
{
	 return texMan.Load(filename, content);
}

bool GameContent::Load(std::string filename, GameSpriteFont &content)
{
	bool good;
	GameTexture2D text;
	// Fixes text burring
	//good = texMan.Load(filename, text, false, GL_NEAREST, GL_NEAREST);//NEAREST,GL_NEAREST);
	// Looks better stretched.
	good = texMan.Load(filename,text,true,GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);//NEAREST,GL_NEAREST);

	if (good) 
		good = content.Load(filename,text);
	return good;
}

bool GameContent::Load(std::string file, Mesh& mesh)
{
	std::ifstream f(file.c_str());
	std::vector<Vector3d> verts;
	std::vector<Vector3d> norms;
	std::vector<double> vcount;
	std::vector<Vector3i> fcount;
	std::vector<Vector3d> vnorms;

	std::vector<Vector2d> uvs;
	Vector3d vert;
	bool hasNormals = false;
	bool hasUVs = false;
	char line[256];

	// Check to see if file has normals
	if (f.is_open())
	{
		while (!f.eof())
		{
			f.getline(line, 256);
			if (line[0] == 'v' && line[1] == 'n') hasNormals = true;
			if (line[0] == 'v' && line[1] == 't') hasUVs = true;
		}
	}
	// Reset file
	f.clear();
	f.seekg(0);

	// Parse the file
	if (f.is_open())
	{
		unsigned char junk = 0;
		unsigned int p1 = 0, p2 = 0, p3 = 0;
		unsigned int n1 = 0, n2 = 0, n3 = 0;
		unsigned int uv1 = 0, uv2 = 0, uv3 = 0;
		while (!f.eof())
		{
			junk = 0;
			f.getline(line, 256);
			std::stringstream ss;
			ss << line;

			if (line[0] == 'v')
			{
				if (line[1] == 'n') // Vertex normals
				{
					ss >> junk >> junk >> vert.x >> vert.y >> vert.z;
					norms.emplace_back(vert);
					continue;
				}
				else if (line[1] == 't')  // texture uvs
				{
					ss >> junk >> junk >> vert.x >> vert.y;
					uvs.emplace_back(Vector2d(vert.x, vert.y));
					continue;
				}
				else
				{
					ss >> junk >> vert.x >> vert.y >> vert.z;
					verts.emplace_back(vert);
					// start counting verts
					vcount.emplace_back(1);
					// if it has no normals make temporary ones
					if (!hasNormals)
					{
						norms.emplace_back(Vector3d(0, 0, 0));
					}
					continue;
				}
			}
			if (line[0] == 'f')
			{
				if (hasUVs && hasNormals)
				{
					ss >> junk >> p1 >> junk >> uv1 >> junk >> n1;
					ss >> p2 >> junk >> uv2 >> junk >> n2;
					ss >> p3 >> junk >> uv3 >> junk >> n3;
				}
				else if (hasNormals)
				{
					ss >> junk >> p1 >> junk >> junk >> n1;
					ss >> p2 >> junk >> junk >> n2;
					ss >> p3 >> junk >> junk >> n3;
				}
				else if (hasUVs)
				{
					// may have to get rid of the ns as junk
					ss >> junk >> p1 >> junk >> uv1 >> junk >> n1;
					ss >> p2 >> junk >> uv2 >> junk >> n2;
					ss >> p3 >> junk >> uv3 >> junk >> n3;
				}
				else
				{
					ss >> junk >> p1 >> p2 >> p3;
				}
				Triangle tri;
				// Vertices
				tri.v[0] = verts[p1 - 1];
				tri.v[1] = verts[p2 - 1];
				tri.v[2] = verts[p3 - 1];
				// UV (texture) coords
				if (hasUVs)
				{
					tri.vUV[0] = uvs[uv1 - 1];// Vector2d(uvs[uv1 - 1].x, uvs[uv1 - 1].y);
					tri.vUV[1] = uvs[uv2 - 1];// Vector2d(uvs[uv2 - 1].x, uvs[uv2 - 1].y);
					tri.vUV[2] = uvs[uv3 - 1];// Vector2d(uvs[uv3 - 1].x, uvs[uv3 - 1].y);
				}
				else
				{
					tri.vUV[0] = Vector2d(0, 0);
					tri.vUV[1] = Vector2d(0, 0);
					tri.vUV[2] = Vector2d(0, 0);
				}


				// count the vertices
				if (!hasNormals)
				{
					vcount[p1 - 1]++;
					vcount[p2 - 1]++;
					vcount[p3 - 1]++;
					Vector3i t;
					t.x = p1 - 1;
					t.y = p2 - 1;
					t.z = p3 - 1;
					fcount.emplace_back(t);
				}


				Vector3d a, b;
				// Calculate the face normal of the triangle
				a = tri.v[1] - tri.v[0];
				b = tri.v[2] - tri.v[0];
				tri.fNormal = a.Cross(b);


				if (hasNormals)
				{
					// Add the face normal to the vertex normals
					tri.fNormal.Normalize();
					tri.vNormals[0] = norms[n1 - 1];
					tri.vNormals[1] = norms[n2 - 1];
					tri.vNormals[2] = norms[n3 - 1];
				}
				else
				{
					// Sum the normals
					norms[p1 - 1] += tri.fNormal;
					norms[p2 - 1] += tri.fNormal;
					norms[p3 - 1] += tri.fNormal;
					tri.fNormal.Normalize();

				}

				mesh.tris.emplace_back(tri);

				continue;
			}
		}

		if (!hasNormals)
		{
			for (int i = 0; i < mesh.tris.size(); i++)
			{
				mesh.tris[i].vNormals[0] = norms[fcount[i].x] / vcount[fcount[i].x];
				mesh.tris[i].vNormals[1] = norms[fcount[i].y] / vcount[fcount[i].y];
				mesh.tris[i].vNormals[2] = norms[fcount[i].z] / vcount[fcount[i].z];
				mesh.tris[i].vNormals[0].Normalize();
				mesh.tris[i].vNormals[1].Normalize();
				mesh.tris[i].vNormals[2].Normalize();
			}
		}
		return true;
	}
	else return false;

}

bool GameContent::Load(std::string vertex, std::string fragment, Shader &shader, std::string &info)
{
	info = shader.Load(vertex,fragment);
	size_t found = info.find("error(#");
	if (found!=std::string::npos)  
		return false;

	return true;
}

void GameContent::UnLoad(GameTexture2D &content)
{
	texMan.UnLoad(content); 
	return;
}

void GameContent::UnLoad(GameSpriteFont &content)
{
	texMan.UnLoad(content.texture); 
	content.UnLoad(); 
	return;	
}

void GameContent::UnLoad(Shader &shader)
{
	shader.UnLoad();
}