#pragma once

#include "GameMath.h"
#include "GameClasses.h"

#include <vector>

struct Triangle
{
	Vector3d v[3];
	Color col;
	Vector3d fNormal;
	Vector3d vNormals[3];
	Vector2d vUV[3];
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
