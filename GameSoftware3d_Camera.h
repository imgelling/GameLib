#pragma once
#include "GameMath.h"

class Camera
{
public:
	Vector3f pos;
	Vector3f right;
	Vector3f up;
	Vector3f forward;
	Vector3f rotation;
	Camera();
	void SetRotation(const double&, const double&, const double&);
	void SetPosition(const Vector3f&);
	void SetUp(const Vector3f&);
	void SetForward(const Vector3f&);
	void SetRight(const Vector3f&);


	Vector3f modRight;
	Vector3f modUp;
	Vector3f modForward;
};
