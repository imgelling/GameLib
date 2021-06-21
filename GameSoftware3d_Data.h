#pragma once

#include "GameMath.h"
#include "GameClasses.h"

#include <vector>

struct Triangle
{
	Vector3f v[3];
	Color col;
	Vector3f fNormal;
	Vector3f vNormals[3];
	Vector2f vUV[3];
};

struct Mesh
{
	std::vector<Triangle> tris;
};

enum class RasterMode
{
	WireFrame,
	Filled,
	WireFrameFilled,
	AffineTextureMapped,
	WireFrameAffTexture,
	ProjectionTextureMapped,
	WireFrameProjTexture,
	None
};
RasterMode& operator++ (RasterMode& rmode, int);
std::ostream& operator<< (std::ostream& stm, RasterMode rmode);

enum class RasterType
{
	Barycentric,
	ScanLine,
	None
};
RasterType& operator++ (RasterType& rType, int);
std::ostream& operator<< (std::ostream& stm, RasterType rmode);