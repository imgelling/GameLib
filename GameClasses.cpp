// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameClasses.h"

// METHODS /////////////////////////////////////////////////////////////////////

// Color Struct ////////////////////////////////////////////////////////////////
Color::Color()
{
    r = g = b = a = 255;
    rf = gf = bf = af = 1.0f;
    packed = 0;
}

Color::Color(float r, float g, float b, float a)
{
    this->r = (unsigned int)(r * 255.0f);
    this->g = (unsigned int)(g * 255.0f);
    this->b = (unsigned int)(b * 255.0f);
    this->a = (unsigned int)(a * 255.0f);
    this->rf = r;
    this->gf = g;
    this->bf = b;
    this->af = a;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = this->a << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

Color::Color(double r, double g, double b, double a)
{
    this->r = (unsigned int)(r * 255.0f);
    this->g = (unsigned int)(g * 255.0f);
    this->b = (unsigned int)(b * 255.0f);
    this->a = (unsigned int)(a * 255.0f);
    this->rf = (float)r;
    this->gf = (float)g;
    this->bf = (float)b;
    this->af = (float)a;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = this->a << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    this->rf = r * Convert256toF;
    this->gf = g * Convert256toF;
    this->bf = b * Convert256toF;
    this->af = a * Convert256toF;

    unsigned int cR = r << 0;
    unsigned int cG = g << 8;
    unsigned int cB = b << 16;
    unsigned int cA = a << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

Color::Color(float r, float g, float b)
{
    this->r = (unsigned int)(r * 255.0f);
    this->g = (unsigned int)(g * 255.0f);
    this->b = (unsigned int)(b * 255.0f);
    this->a = (unsigned int)(1.0 * 255.0f);
    rf = r;
    gf = g;
    bf = b;
    af = 1.0f;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = 255 << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

Color::Color(double r, double g, double b)
{
    this->r = (unsigned int)(r * 255.0f);
    this->g = (unsigned int)(g * 255.0f);
    this->b = (unsigned int)(b * 255.0f);
    this->a = (unsigned int)(1.0 * 255.0f);
    rf = (float)r;
    gf = (float)g;
    bf = (float)b;
    af = 1.0f;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = 255 << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

Color::Color(unsigned int r, unsigned int g, unsigned int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
    this->rf = r * Convert256toF;
    this->gf = g * Convert256toF;
    this->bf = b * Convert256toF;
    this->af = 1.0f;

    unsigned int cR = r << 0;
    unsigned int cG = g << 8;
    unsigned int cB = b << 16;
    unsigned int cA = 255 << 24;
    packed = ((cA) | (cR) | (cG) | (cB));

}

void Color::Set(float r, float g, float b, float a)
{
    this->r = (unsigned int)(r * 255.0f);
    this->g = (unsigned int)(g * 255.0f);
    this->b = (unsigned int)(b * 255.0f);
    this->a = (unsigned int)(a * 255.0f);
    this->rf = r;
    this->gf = g;
    this->bf = b;
    this->af = a;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = this->a << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}


void Color::Set(double r, double g, double b, double a)
{
    this->r = (unsigned int)(r * 255.0f);
    this->g = (unsigned int)(g * 255.0f);
    this->b = (unsigned int)(b * 255.0f);
    this->a = (unsigned int)(a * 255.0f);
    this->rf = (float)r;
    this->gf = (float)g;
    this->bf = (float)b;
    this->af = (float)a;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = this->a << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

void Color::Set(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    rf = r * Convert256toF;
    gf = g * Convert256toF;
    bf = b * Convert256toF;
    af = a * Convert256toF;

    unsigned int cR = r << 0;
    unsigned int cG = g << 8;
    unsigned int cB = b << 16;
    unsigned int cA = a << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

void Color::Set(unsigned char r, unsigned char b, unsigned char g)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
    rf = r * Convert256toF;
    gf = g * Convert256toF;
    bf = b * Convert256toF;
    af = 1.0f;

    unsigned int cR = r << 0;
    unsigned int cG = g << 8;
    unsigned int cB = b << 16;
    unsigned int cA = 255 << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

void Color::Set(float r, float g, float b)
{
    this->r = (unsigned int)(r * 255.0f);
    this->g = (unsigned int)(g * 255.0f);
    this->b = (unsigned int)(b * 255.0f);
    this->a = (unsigned int)(1.0 * 255.0f);
    rf = r;
    gf = g;
    bf = b;
    af = 1.0f;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = 255 << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

void Color::Set(double r, double g, double b)
{
    this->r = (unsigned int)(r * 255.0f);
    this->g = (unsigned int)(g * 255.0f);
    this->b = (unsigned int)(b * 255.0f);
    this->a = (unsigned int)(1.0 * 255.0f);
    rf = (float)r;
    gf = (float)g;
    bf = (float)b;
    af = 1.0f;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = 255 << 24;
    packed = ((cA) | (cR) | (cG) | (cB));
}

void Color::Set(unsigned int r, unsigned int g, unsigned int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
    rf = r * Convert256toF;
    gf = g * Convert256toF;
    bf = b * Convert256toF;
    af = 1.0f;

    unsigned int cR = this->r << 0;
    unsigned int cG = this->g << 8;
    unsigned int cB = this->b << 16;
    unsigned int cA = 255 << 24;
    packed = ((cA) | (cR) | (cG) | (cB));

}


Color::~Color()
{
}
