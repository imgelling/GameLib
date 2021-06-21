#include "GameSoftware3d_Camera.h"

Camera::Camera()
{
	SetPosition(Vector3f(0, 0, 0));
	yaw = 3.14159f / 2.0f;
	pitch = 0.0f;
	roll = 0.0f;

	forward.x = cos(yaw) * cos(pitch);
	forward.y = sin(pitch);
	forward.z = sin(yaw) * cos(pitch);
	forward.Normalize();

	Vector3f newUp(0.0f, 1.0f, 0.0f);
	right = newUp.Cross(forward);
	right.Normalize();

	up = (forward.Cross(right));
	up.Normalize();
};



void Camera::SetRotation(const float x, const float y, const float z)
{
	// Setup rotation matrices
	if (x)
	{
		rotation.x += x;
		pitch += x;
	}
	else if (y)
	{
		rotation.y += y;
		yaw += y;
	}
	else if (z)
	{
		// nothing yet
		rotation.z += z;
		roll += z;
	}

	forward.x = cos(yaw) * cos(pitch);
	forward.y = sin(pitch);
	forward.z = sin(yaw) * cos(pitch);
	forward.Normalize();

	Vector3f newUp(0.0f, 1.0f, 0.0f);
	right = newUp.Cross(forward);
	right.Normalize();

	up = (forward.Cross(right));
	up.Normalize();
}
void Camera::SetPosition(const Vector3f& in)
{
	pos = in;
	//modPos = position;
}

Matrix4x4f rotateM(const float ang, const Vector3f& axis)
{
	float c = cos(ang);
	float s = sin(ang);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	Vector3f temp(axis);
	temp.x = temp.x * (1.0f - c);
	temp.y = temp.y * (1.0f - c);
	temp.z = temp.z * (1.0f - c);


	Matrix4x4f ret;
	ret.m[0] = temp.x * x + c;
	ret.m[1] = temp.x * y + z * s;
	ret.m[2] = temp.x * z - y * s;

	ret.m[4] = temp.y * x - z * s;
	ret.m[5] = c + temp.y * y;
	ret.m[6] = temp.y * z + x * s;

	ret.m[8] = temp.z * x + y * s;
	ret.m[9] = temp.z * y - x * s;
	ret.m[10] = c + temp.z * z;

	return ret;
}

Matrix4x4f Camera::GenerateView()
{
	Matrix4x4f view;

	// Rotation stuff

	view.m[0] = -right.x;
	view.m[4] = -right.y;
	view.m[8] = -right.z;

	view.m[1] = up.x;
	view.m[5] = up.y;
	view.m[9] = up.z;

	view.m[2] = forward.x;
	view.m[6] = forward.y;
	view.m[10] = forward.z;

	Matrix4x4f r = rotateM(roll, forward);

	view = view * r;

	Matrix4x4f ct;
	ct.SetTranslation(-pos.x, -pos.y, -pos.z);
	view = view * ct;

	return view;
}

Matrix4x4f Camera::GenerateLookAtView(Vector3f& to)
{
	// Don't think it works
	Matrix4x4f view;
	Vector3f right;// (cam.modRight);
	Vector3f newForward;// (cam.modForward);
	Vector3f newUp;// (cam.modUp);
	newUp.Normalize();

	// Probably needs to be reversed
	newForward = to - pos;// -to;
	newForward.Normalize();
	right = newUp.Cross(newForward);
	newUp = newForward.Cross(right);

	view.m[0] = -right.x;
	view.m[4] = -right.y;
	view.m[8] = -right.z;

	view.m[1] = newUp.x;
	view.m[5] = newUp.y;
	view.m[9] = newUp.z;

	view.m[2] = newForward.x;
	view.m[9] = newForward.y;
	view.m[10] = newForward.z;

	Matrix4x4f ct;
	ct.SetTranslation(-pos.x, -pos.y, -pos.z);
	view = view * ct;

	return view;
}