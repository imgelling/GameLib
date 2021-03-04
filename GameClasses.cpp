// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameClasses.h"

// METHODS /////////////////////////////////////////////////////////////////////


// Possible copy constructor to allow for
// Color newcolor =  Color.Red; etc

// Color Struct ////////////////////////////////////////////////////////////////
Color::Color()
{
    r = g = b = a = 1.0f;
}

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color::Color(int r, int g, int b, int a)
{
	this->r = r * Convert256toF;
    this->g = g * Convert256toF;
    this->b = b * Convert256toF;
    this->a = a * Convert256toF;
}

Color::Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1.0f;
}

Color::Color(int r, int g, int b)
{
	this->r = r * Convert256toF;
    this->g = g * Convert256toF;
    this->b = b * Convert256toF;
    this->a = 1.0f;

}

void Color::Set(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Color::Set(int r, int g, int b, int a)
{
    this->r = r * Convert256toF;
    this->g = g * Convert256toF;
    this->b = b * Convert256toF;
    this->a = a * Convert256toF;
}

void Color::Set(unsigned char r, unsigned char b, unsigned char g)
{
	this->r = r * Convert64toF;
	this->g = g * Convert64toF;
	this->b = b * Convert64toF;
	this->a = 1.0f;
}

void Color::Set(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1.0f;
}

void Color::Set(int r, int g, int b)
{
    this->r = r * Convert256toF;
    this->g = g * Convert256toF;
    this->b = b * Convert256toF;
    this->a = 1.0f;
}


Color::~Color()
{
}

// Color256 Struct
void Color256::Set(unsigned char r2, unsigned char g2, unsigned char b2)
{
	r = (unsigned char)(((float)r2 * Convert64toF)*255.0f);
	g = (unsigned char)(((float)g2 * Convert64toF)*255.0f);
	b = (unsigned char)(((float)b2 * Convert64toF)*255.0f);
	PackedColor = 0;
	unsigned int cR = r << 16;
	unsigned int cG = g << 8;
	unsigned int cB = b;
	unsigned int cA = 255 << 24;
	PackedColor =  ((cA) | (cR) | (cG) | (cB));
	//PackedColor =  ((cA) | (cB) | (cG) | (cR));
	//unsigned int cR = r;// << 16;
	//unsigned int cG = g << 8;
	//unsigned int cB = b << 16;
	//unsigned int cA = 255 << 24;
	//PackedColor = ((cA) | (cR) | (cG) | (cB));
	//PackedColor =  ((cA) | (cB) | (cG) | (cR));
}



