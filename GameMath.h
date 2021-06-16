#pragma once
#include <cmath>
#include <iostream>

//#define DEG2RAD(x)  x*0.0174532925f

// Point 
template <typename T>
class Point
{
public:
	T x;
	T y;
	Point() : x((T)0.0), y((T)0.0) {};
	Point(const T& x, const T& y)
	{
		this->x = x;
		this->y = y;
	}
private:
};
typedef Point<int> Point2i;
typedef Point<float> Point2f;
typedef Point<double> Point2d;

// Matrix4x4
template <typename T>
class Matrix4x4
{
public:
	T m[16];// = { 0.0 };
	Matrix4x4() { SetIdentity(); }
	Matrix4x4(const Matrix4x4& in)
	{
		for (int i = 0; i < 16; i++)
		{
			m[i] = in.m[i];
		}
	}
	Matrix4x4(const T(&in)[16])
	{
		memcpy(m, in, sizeof(in));
	}
	Matrix4x4 operator+ (const Matrix4x4& rhs)
	{
		Matrix4x4<T> ret;
		for (int i = 0; i < 16; i++)
		{
			ret.m[i] = m[i] + rhs.m[i];
		}
		return ret;
	}
	Matrix4x4& operator+= (const Matrix4x4& rhs)
	{
		for (int i = 0; i < 16; i++)
		{
			m[i] = m[i] + rhs.m[i];
		}
		return *this;
	}
	Matrix4x4 operator- (const Matrix4x4& rhs)
	{
		Matrix4x4<T> ret;
		for (int i = 0; i < 16; i++)
		{
			ret.m[i] = m[i] - rhs.m[i];
		}
		return ret;
	}
	Matrix4x4& operator-= (const Matrix4x4& rhs)
	{
		for (int i = 0; i < 16; i++)
		{
			m[i] = m[i] - rhs.m[i];
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
	Matrix4x4& operator*= (const T& scalar)
	{
		//Matrix4x4<T> ret;
		for (int i = 0; i < 16; i++)
		{
			m[i] = m[i] * scalar;
		}
		return *this;
	}
	Matrix4x4 operator* (const Matrix4x4& rhs)
	{
		Matrix4x4<T> ret;
		for (int i = 0; i < 4; i++) // row
		{
			for (int j = 0; j < 4; j++) // col
			{
				ret.m[j * 4 + i] = 0;  // zero out location in return
				for (int k = 0; k < 4; k++)
				{
					ret.m[j * 4 + i] += m[k * 4 + i] * rhs.m[j * 4 + k];
				}
			}
		}

		return ret;
	}
	void SetIdentity()
	{
		for (int i = 0; i < 16; i++)
		{
			//if (i % 5 == 0) m[i] = 1.0;
			//else m[i] = 0.0;
			m[i] = (T)0.0;
		}
		m[0] = m[5] = m[10] = m[15] = (T)1.0;
	}
	void SetRotationX(const T &rot)
	{
		m[5] = cos(rot);
		m[6] = sin(rot);
		m[9] = -sin(rot);
		m[10] = cos(rot);
	}
	void SetRotationY(const T& rot)
	{
		m[0] = cos(rot);
		m[2] = -sin(rot);
		m[8] = sin(rot);
		m[10] = cos(rot);
	}
	void SetRotationZ(const T& rot)
	{
		m[0] = cos(rot);
		m[1] = sin(rot);
		m[4] = -sin(rot);
		m[5] = cos(rot);
	}
	void SetTranslation(const T& x, const T& y, const T& z)
	{
		m[12] = x;
		m[13] = y;
		m[14] = z;
	}
	void SetScale(const T& x, const T& y, const T& z)
	{
		m[0] = x;
		m[5] = y;
		m[10] = z;
	}

	double Determinant() {
		return
			m[12] * m[9] * m[6] * m[3] - m[8] * m[13] * m[6] * m[3] -
			m[12] * m[5] * m[10] * m[3] + m[4] * m[13] * m[10] * m[3] +
			m[8] * m[5] * m[14] * m[3] - m[4] * m[9] * m[14] * m[3] -
			m[12] * m[9] * m[2] * m[7] + m[8] * m[13] * m[2] * m[7] +
			m[12] * m[1] * m[10] * m[7] - m[0] * m[13] * m[10] * m[7] -
			m[8] * m[1] * m[14] * m[7] + m[0] * m[9] * m[14] * m[7] +
			m[12] * m[5] * m[2] * m[11] - m[4] * m[13] * m[2] * m[11] -
			m[12] * m[1] * m[6] * m[11] + m[0] * m[13] * m[6] * m[11] +
			m[4] * m[1] * m[14] * m[11] - m[0] * m[5] * m[14] * m[11] -
			m[8] * m[5] * m[2] * m[15] + m[4] * m[9] * m[2] * m[15] +
			m[8] * m[1] * m[6] * m[15] - m[0] * m[9] * m[6] * m[15] -
			m[4] * m[1] * m[10] * m[15] + m[0] * m[5] * m[10] * m[15];
	}
};
typedef Matrix4x4<double> Matrix4x4d;
typedef Matrix4x4<float> Matrix4x4f;
typedef Matrix4x4<int> Matrix4x4i;

// Vector2
template <typename T>
class Vector2
{
public:
	T x;
	T y;
	Vector2() : x((T)0.0), y((T)0.0) {}
	Vector2(const T& x, const T& y)
	{
		this->x = x;
		this->y = y;
	}
	Vector2 operator+ (const Vector2& rhs)
	{
		Vector2<T> c;
		c.x = x + rhs.x;
		c.y = y + rhs.y;
		return c;
	}
	Vector2& operator+= (const Vector2& rhs)
	{
		x = x + rhs.x;
		y = y + rhs.y;
		return *this;
	}
	Vector2 operator- (const Vector2& rhs)
	{
		Vector2<T> c;
		c.x = x - rhs.x;
		c.y = y - rhs.y;
		return c;
	}
	Vector2& operator-= (const Vector2& rhs)
	{
		x = x - rhs.x;
		y = y - rhs.y;
		return *this;
	}
	Vector2 operator* (const T& scalar)
	{
		Vector2<T> c(x, y);
		c.x = x * scalar;
		c.y = y * scalar;
		return c;
	}
	Vector2& operator*= (const T& scalar)
	{
		x = x * scalar;
		y = y * scalar;
		return *this;
	}
	Vector2 operator/ (const T& scalar)
	{
		Vector2<T> c(x, y);
		c.x = x / scalar;
		c.y = y / scalar;
		return c;
	}
	Vector2 operator- (const Point<T>& p)
	{
		Vector2<T> c;
		c.x = x - p.x;
		c.y = y - p.y;
		return c;
	}
	Vector2& operator-= (const Point<T>& p)
	{
		//Vector2<T> c;
		x = x - p.x;
		y = y - p.y;
		return *this;
	}
	Vector2 operator+ (const Point<T>& p)
	{
		Vector2<T> c;
		c.x = x + p.x;
		c.y = y + p.y;
		return c;
	}
	Vector2& operator+= (const Point<T>& p)
	{
		//Vector2<T> c;
		x = x + p.x;
		y = y + p.y;
		return *this;
	}
	T Mag2() { return ((x * x) + (y * y)); }
	T Mag() { return sqrt((x * x) + (y * y)); }
	void Normalize()
	{
		double l = Mag();
		if (l == 0) return;
		x /= l;
		y /= l;
	}
	double Dot(const Vector2<T>& rhs)
	{
		return (x * rhs.x) + (y * rhs.y);
	}
	Vector2 Lerp(const Vector2<T>& second, const T& by)
	{
		return *this * (1 - by) + (Vector2<T>)second * by;
	}
	Vector2 NLerp(const Vector2<T>& second, const T& by)
	{
		return Lerp(second, by).Normalize();
	}
	Vector2 SLerp(const Vector2<T>& second, const T& by)
	{
		double dot = Dot(second);
		if (dot < -1.0) dot = -1.0;
		else if (dot > 1.0) dot = 1.0;
		double theta = std::acos(dot) * by;
		Vector2 relative = (Vector2<T>)second - (*this * by);
		relative = relative.Normalize();
		return ((*this * std::cos(theta)) + (relative * std::sin(theta)));
	}
private:
	// Lerp
	template <typename K>
	K _Lerp(K first, K second, K by)
	{
		K ret = first * (1 - by) + second * by;
		return ret;
	}
};
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;

// Vector 3
template <typename T>
class Vector3
{
public:
	T x = 0;
	T y = 0;
	T z = 0;
	T w = 1;
	Vector3() {}
	Vector3(const T& x, const T& y, const T& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = (T)1;
	}
	Vector3 operator+ (const Vector3& rhs)
	{
		Vector3<T> c = *this;
		c.x += rhs.x;	// why no work?
		c.y += rhs.y;
		c.z += rhs.z;
		return c;
	}
	Vector3& operator+= (const Vector3& rhs)
	{
		x = x + rhs.x;
		y = y + rhs.y;
		z = z + rhs.z;
		return *this;
	}
	Vector3 operator- (const Vector3& rhs)
	{
		Vector3<T> c;
		c.x = x - rhs.x;
		c.y = y - rhs.y;
		c.z = z - rhs.z;
		return c;
	}
	Vector3& operator-= (const Vector3& rhs)
	{
		x = x - rhs.x;
		y = y - rhs.y;
		z = z - rhs.z;
		return *this;
	}
	Vector3 operator* (const T& scalar)
	{
		Vector3<T> c = *this;			// why no work?
		c.x = c.x * scalar;
		c.y = c.y * scalar;
		c.z = c.z * scalar;
		return c;
	}
	Vector3& operator*= (const T& scalar)
	{
		x = x * scalar;	
		y = y * scalar;
		z = z * scalar;
		return *this;
	}
	Vector3 operator/ (const T& scalar)
	{
		Vector3<T> c(x, y, z);
		c.x = c.x / scalar;
		c.y = c.y / scalar;
		c.z = c.z / scalar;
		return c;
	}
	Vector3& operator/= (const T& scalar)
	{
		x = x / scalar;
		y = y / scalar;
		z = z / scalar;
		return *this;
	}
	Vector3 operator* (const Matrix4x4<T>& mat)
	{
		Vector3<T> ret;
		ret.x = (x * mat.m[0] + y * mat.m[4] + z * mat.m[8] + w * mat.m[12]);
		ret.y = (x * mat.m[1] + y * mat.m[5] + z * mat.m[9] + w * mat.m[13]);
		ret.z = (x * mat.m[2] + y * mat.m[6] + z * mat.m[10] + w * mat.m[14]);
		ret.w = (x * mat.m[3] + y * mat.m[7] + z * mat.m[11] + w * mat.m[15]);
		//if (w != 0)
		//{
		//	ret.x /= w;
		//	ret.y /= w;
		//	ret.z /= w;
		//}
		return ret;
	}
	Vector3& operator*= (const Matrix4x4<T>& mat)
	{
		Vector3<T> ret;
		ret.x = (x * mat.m[0] + y * mat.m[4] + z * mat.m[8] + w * mat.m[12]);
		ret.y = (x * mat.m[1] + y * mat.m[5] + z * mat.m[9] + w * mat.m[13]);
		ret.z = (x * mat.m[2] + y * mat.m[6] + z * mat.m[10] + w * mat.m[14]);
		ret.w = (x * mat.m[3] + y * mat.m[7] + z * mat.m[11] + w * mat.m[15]);
		*this = ret;
		return *this;
	}
	double Mag2() { return ((x * x) + (y * y) + (z * z)); }
	double Mag() { return sqrt((x * x) + (y * y) + (z * z)); }
	double Dot(const Vector3<T>& rhs) {
		return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
	}
	void Normalize()
	{
		double l = Mag();
		if (l == 0) return;
		x /= l;
		y /= l;
		z /= l;
	}
	Vector3 Cross(const Vector3& rhs)
	{
		Vector3<T> t;
		t.x = (y * rhs.z) - (z * rhs.y);
		t.y = (z * rhs.x) - (x * rhs.z);
		t.z = (x * rhs.y) - (y * rhs.x);
		return t;
	}
	Vector3 Lerp(const Vector3<T>& second, const T by)
	{
		return *this * (1 - by) + (Vector3<T>)second * by;
	}
	Vector3 Lerp(const Vector3<T>& first, const Vector3<T>& second, const T by)
	{
		Vector3<T> ret;
		//out_tri1.vNormals[1].x = in_normals[0].x * (1 - res.x) + out_normals[0].x * res.x;
		//out_tri1.vNormals[1].y = in_normals[0].y * (1 - res.y) + out_normals[0].y * res.y;
		//out_tri1.vNormals[1].z = in_normals[0].z * (1 - res.z) + out_normals[0].z * res.z;
		ret.x = first.x * (1.0 - by) + second.x * by;
		ret.y = first.y * (1.0 - by) + second.y * by;
		ret.z = first.z * (1.0 - by) + second.z * by;
		return ret;
	}
	Vector3 NLerp(const Vector3<T>& second, const T& by)
	{
		return Lerp(second, by).Normalize();
	}
	Vector3 SLerp(const Vector3<T>& second, const T& by)
	{
		double dot = Dot(second);
		if (dot < -1.0) dot = -1.0;
		else if (dot > 1.0) dot = 1.0;
		double theta = std::acos(dot) * by;
		Vector3 relative = (Vector3<T>)second - (*this * by);
		relative = relative.Normalize();
		return ((*this * std::cos(theta)) + (relative * std::sin(theta)));
	}
	Vector3 InvLerp(const Vector3<T>& second, const Vector3<T>& result)
	{
		Vector3<double> ab;// = result - *this;
		ab.x = result.x - x;
		ab.y = result.y - y;
		ab.z = result.z - z;
		Vector3<double> av;// = second - *this;
		av.x = second.x - x;
		av.y = second.y - y;
		av.z = second.z - z;
		Vector3<double> ret;
		ret.x = ab.x / av.x;
		if (av.x == 0) ret.x = 0.0;
		ret.y = ab.y / av.y;
		if (av.y == 0) ret.y = 0.0;
		ret.z = ab.z / av.z;
		if (av.z == 0) ret.z = 0.0;

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
	T Width() { return right - left; }
	T Height() { return bottom - top; }
	Rect() : left((T)0.0), top((T)0.0), right((T)0.0), bottom((T)0.0) {}
	Rect(const T& left, const T& top, const T& right, const T& bottom)
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
