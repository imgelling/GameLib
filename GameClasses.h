#pragma once
// INCLUDES ////////////////////////////////////////////////////////////////////


// DEFINES /////////////////////////////////////////////////////////////////////

#define DEG2RAD(x)  x*0.0174532925f


// DECLARATIONS ////////////////////////////////////////////////////////////////
struct Color
{
    public:
        float r;
        float g;
        float b;
        float a;
        void Set(float r, float g, float b, float a);
        void Set(int r, int g, int b, int a);
        void Set(float r, float g, float b);
        void Set(int r, int g, int b);
		void Set(unsigned char r, unsigned char b, unsigned char g);
        Color();
        Color(float r, float g, float b, float a);
        Color(int r, int g, int b, int a);
        Color(float r, float g, float b);
        Color(int r, int g, int b);
        ~Color();
    private:
		float Convert256toF = (1.0f / 255.0f);
		float Convert64toF = (1.0f / 63.0f);
};

struct Color256
{
	public:
		unsigned char r = 0;
		unsigned char b = 0;
		unsigned char g = 0;
		unsigned int PackedColor = 0;
		void Set(unsigned char, unsigned char, unsigned char);
	private:
		float Convert64toF = (1.0f / 63.0f);
};



namespace Colors
{
	const Color White(255, 255, 255, 255);
	const Color Red(255, 0, 0, 255);
	const Color Green(0, 255, 0, 255);
	const Color Blue(0, 0, 255, 255);
	const Color Black(0, 0, 0, 255);
	const Color Pink(255, 0, 255, 255);
	const Color Gray(128, 128, 128, 255);
	const Color DarkGray(64, 64, 64, 255);
}

// Ill come back to this

class Point
{
    public:
        int x;
        int y;
        Point();
        Point(int, int);
    private:
};

class Vector2f
{
public:
	float x;
	float y;
	Vector2f();
	Vector2f(float, float);
	Vector2f &operator+ (const Vector2f &in)
	{
		this->x = this->x + in.x;
		this->y = this->y + in.y;
		return *this;
	}
	Vector2f &operator+= (const Vector2f &in)
	{
		this->x = this->x + in.x;
		this->y = this->y + in.y;
		return *this;
	}
	Vector2f &operator- (const Vector2f &in)
	{
		this->x = this->x - in.x;
		this->y = this->y - in.y;
		return *this;
	}
	Vector2f &operator-= (const Vector2f &in)
	{
		this->x = this->x - in.x;
		this->y = this->y - in.y;
		return *this;
	}
	Vector2f &operator* (const float &scalar)
	{
		this->x = this->x * scalar;
		this->y = this->y * scalar;
		return *this;
	}
	Vector2f &operator/ (const float &scalar)
	{
		this->x = this->x / scalar;
		this->y = this->y / scalar;
		return *this;
	}
	Vector2f &operator- (const Point &p)
	{
		this->x = this->x - p.x;
		this->y = this->y - p.y;
		return *this;
	}
	Vector2f &operator-= (const Point &p)
	{
		this->x = this->x - p.x;
		this->y = this->y - p.y;
		return *this;
	}
	Vector2f &operator+ (const Point &p)
	{
		this->x = this->x + p.x;
		this->y = this->y + p.y;
		return *this;
	}
	Vector2f &operator+= (const Point &p)
	{
		this->x = this->x + p.x;
		this->y = this->y + p.y;
		return *this;
	}

private:
};

class Rect
{
    public:
        int left;
        int top;
        int right;
        int bottom;
        int Width();
        int Height();
        Rect();
        Rect(int, int, int, int);
    private:
};

