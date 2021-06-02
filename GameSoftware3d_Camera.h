#pragma once
#include "GameMath.h"

class Camera
{
public:
	Vector3d pos;
	Vector3d right;
	Vector3d up;
	Vector3d forward;
	Vector3d rotation;
	Camera();
	void SetRotation(const double&, const double&, const double&);
	void SetPosition(const Vector3d&);
	void SetUp(const Vector3d&);
	void SetForward(const Vector3d&);
	void SetRight(const Vector3d&);


	Vector3d modRight;
	Vector3d modUp;
	Vector3d modForward;
};
