#pragma once
#include <cmath>

#define DEG2RAD(x)  x*0.0174532925f


// Lerp
template <typename T>
T Lerp(T first, T second, T by)
{
	T ret = first * (1 - by) + second * by;
	return ret;
}


// Point 
template <typename T>
class Point
{
public:
	T x;
	T y;
	Point() : x((T)0.0), y((T)0.0) {};
	Point(T x, T y)
	{
		this->x = x;
		this->y = y;
	}
private:
};
typedef Point<int> Point2i;
typedef Point<float> Point2f;
typedef Point<double> Point2d;

// Vector2
template <typename T>
class Vector2
{
public:
	T x;
	T y;
	Vector2() : x(0.0), y(0.0){}
	Vector2(T x, T y)
	{
		this->x = x;
		this->y = y;
	}
	Vector2 operator+ (const Vector2 &in)
	{
		Vector2<T> c;
		c.x = x + in.x;
		c.y = y + in.y;
		return c;
	}
	Vector2 &operator+= (const Vector2 &in)
	{
		x = x + in.x;
		y = y + in.y;
		return *this;
	}
	Vector2 operator- (const Vector2 &in)
	{
		Vector2<T> c;
		c.x = x - in.x;
		c.y = y - in.y;
		return c;
	}
	Vector2 &operator-= (const Vector2 &in)
	{
		x = x - in.x;
		y = y - in.y;
		return *this;
	}
	Vector2 operator* (const T &scalar)
	{
		Vector2<T> c(x,y);
		c.x = x * scalar;
		c.y = y * scalar;
		return c;
	}
	Vector2 operator/ (const T &scalar)
	{
		Vector2<T> c(x,y);
		c.x = x / scalar;
		c.y = y / scalar;
		return c;
	}
	Vector2 operator- (const Point<T> &p)
	{
		Vector2<T> c;
		c.x = x - p.x;
		c.y = y - p.y;
		return c;
	}
	Vector2 &operator-= (const Point<T> &p)
	{
		//Vector2<T> c;
		x = x - p.x;
		y = y - p.y;
		return *this;
	}
	Vector2 operator+ (const Point<T> &p)
	{
		Vector2<T> c;
		c.x = x + p.x;
		c.y = y + p.y;
		return c;
	}
	Vector2 &operator+= (const Point<T> &p)
	{
		//Vector2<T> c;
		x = x + p.x;
		y = y + p.y;
		return *this;
	}

	T Mag2() { return ((x * x) + (y * y)); }
	T Mag() { return sqrt((x * x) + (y * y)); }
	Vector2 Normalize() 
	{
		Vector2<T> c(x,y);
		c = c / Mag();
		return c;
	}
	double Dot(const Vector2<T>&rhs) 
	{
		return (x * rhs.x) + (y * rhs.y);
	}
	Vector2 LerpV2(const Vector2<T>& second, const T by)
	{
		Vector2<T> ret;
		ret.x = Lerp(x, second.x, by);
		ret.y = Lerp(y, second.y, by);
		return ret;
	}
private:
};
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;

// Vector 3
template <typename T>
class Vector3
{
public:
	T x;
	T y;
	T z;
	Vector3() : x(0.0), y(0.0), z(0.0) {}
	Vector3(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3 operator+ (const Vector3& in)
	{
		Vector3<T> c;
		c.x = x + in.x;
		c.y = y + in.y;
		c.z = z + in.z;
		return c;
	}
	Vector3 &operator+= (const Vector3& in)
	{
		//Vector3<T> c;
		x = x + in.x;
		y = y + in.y;
		z = z + in.z;
		return *this;
	}
	Vector3 operator- (const Vector3& in)
	{
		Vector3<T> c;
		c.x = x + in.x;
		c.y = y + in.y;
		c.z = z + in.z;
		return c;
	}
	Vector3 &operator-= (const Vector3& in)
	{
		//Vector3<T> c;
		x = x - in.x;
		y = y - in.y;
		z = z - in.z;
		return *this;
	}
	Vector3 operator* (const T& scalar)
	{
		Vector3<T> c(x,y,z);
		c.x = c.x * scalar;
		c.y = c.y * scalar;
		c.z = c.z * scalar;
		return c;
	}
	Vector3 operator/ (const T& scalar)
	{
		Vector3<T> c(x,y,z);
		c.x = c.x / scalar;
		c.y = c.y / scalar;
		c.z = c.z / scalar;
		return c;
	}
	double Mag2() { return ((x * x) + (y * y) + (z * z)); }
	double Mag() { return sqrt((x * x) + (y * y) + (z * z)); }
	double Dot(const Vector2<T>&rhs) {
		return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
	}
	Vector3 Normalize()
	{
		Vector3<T> c(x, y, z);
		c = c / Mag();
		return c;
	}
	Vector3 Cross(const Vector3& rhs)
	{
		Vector3<T> t;
		t.x = (y * rhs.z) - (z * rhs.y);
		t.y = (z * rhs.x) - (x * rhs.z);
		t.z = (x * rhs.y) - (rhs.y * x);
		return t;
	}
	Vector3 LerpV3(const Vector3<T>& second, const T by)
	{
		Vector3<T> ret;
		ret.x = Lerp(x, second.x, by);
		ret.y = Lerp(y, second.y, by);
		ret.z = Lerp(z, second.z, by);
		return ret;
	}
private:
};
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int> Vector3i;

// Rectangle
template <typename T>
class Rect
{
public:
	T left;
	T top;
	T right;
	T bottom;
	T Width() {	return right - left; }
	T Height() { return bottom - top; }
	Rect() : left((T)0.0), top((T)0.0), right((T)0.0), bottom((T)0.0) {}
	Rect(T left, T top, T right, T bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
private:
};
typedef Rect<int> Recti;
typedef Rect<float> Rectf;
typedef Rect<double> Rectd;


template <typename T>
class Matrix4x4
{
public:
	T m[16];// = { 0.0 };
	Matrix4x4() { SetIdentity(); }
	Matrix4x4(const T& in)
	{
		for (int i = 0; i < 16; i++)
		{
			m[i] = in.m[i];
		}
	}
	Matrix4x4(const T in[])
	{
		for (int i = 0; i < 16; i++)
		{
			m[i] = in[i];
		}
	}
	void SetIdentity()
	{
		for (int i = 0; i < 16; i++)
		{
			if (i % 5 == 0) m[i] = 1.0;
			else m[i] = 0.0;
		}
	}
	Matrix4x4 operator+ (const Matrix4x4 &in)
	{
		Matrix4x4<T> ret;
		for (int i = 0; i < 16; i++)
		{
			ret.m[i] = m[i] + in.m[i];
		}
		return ret;
	}
	Matrix4x4 &operator+= (const Matrix4x4& in)
	{
		for (int i = 0; i < 16; i++)
		{
			m[i] = m[i] + in.m[i];
		}
		return *this;
	}
	Matrix4x4 operator- (const Matrix4x4& in)
	{
		Matrix4x4<T> ret;
		for (int i = 0; i < 16; i++)
		{
			ret.m[i] = m[i] - in.m[i];
		}
		return ret;
	}
	Matrix4x4 &operator-= (const Matrix4x4& in)
	{
		for (int i = 0; i < 16; i++)
		{
			m[i] = m[i] - in.m[i];
		}
		return *this;
	}
	Matrix4x4 operator* (const T& scalar)
	{
		Matrix4x4<T> ret;
		for (int i = 0; i < 16; i++)
		{
			ret.m[i] = m[i] * scalar;
		}
		return ret;
	}
	Matrix4x4 &operator*= (const T& scalar)
	{
		//Matrix4x4<T> ret;
		for (int i = 0; i < 16; i++)
		{
			m[i] = m[i] * scalar;
		}
		return *this;
	}
	Matrix4x4 operator* (const Matrix4x4& in)
	{
		Matrix4x4<T> ret;

		for (int i = 0; i < 4; i++) // row
		{
			for (int j = 0; j < 4; j++) // col
			{
				ret.m[j * 4 + i] = 0;  // zero out location in return
				for (int k = 0; k < 4; k++)
				{
					ret.m[j * 4 + i] += m[k * 4 + i] * in.m[j * 4 + k];
				}
			}
		}

		return ret;
	}
};
typedef Matrix4x4<double> Matrix4x4d;
typedef Matrix4x4<float> Matrix4x4f;
typedef Matrix4x4<int> Matrix4x4i;