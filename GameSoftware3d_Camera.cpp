#include "GameSoftware3d_Camera.h"

Camera::Camera()
{
	SetPosition(Vector3f(0, 0, 0));
	SetRight(Vector3f(1, 0, 0));
	SetUp(Vector3f(0, 1, 0));
	SetForward(Vector3f(0, 0, 1));
};

void Camera::SetRotation(const float x, const float y, const float z)
{
	static bool first = true;
	Matrix4x4f rot;
	// Setup rotation matrices
	if (x)
	{
		rotation.x += x;
		rot.SetRotationX(x);
	}
	else if (y)
	{
		rotation.y += y;
		rot.SetRotationY(y);
	}
	else if (z)
	{
		rotation.z += z;
		rot.SetRotationZ(z);
	}

	if (first)
	{
		modRight = right * rot;
		modForward = forward * rot;
		modUp = up * rot;
		first = false;
	}
	else
	{
		modRight = modRight * rot;
		modForward = modForward * rot;
		modUp = modUp * rot;
	}
}
void Camera::SetPosition(const Vector3f& in)
{
	pos = in;
	//modPos = position;
}
void Camera::SetUp(const Vector3f& in)
{
	up = in;
	modUp = in;
}
void Camera::SetForward(const Vector3f& in)
{
	forward = in;
	modForward = in;
}
void Camera::SetRight(const Vector3f& in)
{
	right = in;
	modRight = in;
}
