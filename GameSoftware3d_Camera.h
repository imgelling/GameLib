#pragma once
#include "GameMath.h"

class Camera
{
public:
	Vector3f pos;
	Vector3f forward;
	Vector3f right;
	Vector3f up;
	Vector3f rotation;
	Camera();
	void SetRotation(const float, const float, const float);
	void SetPosition(const Vector3f&);
	Matrix4x4f GenerateView();
	Matrix4x4f GenerateLookAtView(Vector3f&);

	float yaw;
	float pitch;
	float roll;
};
