#pragma once
// INCLUDES ////////////////////////////////////////////////////////////////////


// DEFINES /////////////////////////////////////////////////////////////////////




// DECLARATIONS ////////////////////////////////////////////////////////////////
class Color
{
public:
	float rf;
	float gf;
	float bf;
	float af;
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
	unsigned int packed;

	Color();
	Color(float r, float g, float b, float a);
	Color(double r, double g, double b, double a);
	Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	Color(float r, float g, float b);
	Color(double r, double g, double b);
	Color(unsigned int r, unsigned int g, unsigned int b);
	~Color();

	void Set(float r, float g, float b, float a);
	void Set(double r, double g, double b, double a);
	void Set(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	void Set(float r, float g, float b);
	void Set(double r, double g, double b);
	void Set(unsigned int r, unsigned int g, unsigned int b);
	void Set(unsigned char r, unsigned char b, unsigned char g);

private:
	float Convert256toF = (1.0f / 255.0f);
};


namespace Colors
{
	const Color White((unsigned int)255, 255, 255, 255);
	const Color Red((unsigned int)255, 0, 0, 255);
	const Color Green((unsigned int)0, 255, 0, 255);
	const Color Blue((unsigned int)0, 0, 255, 128);
	const Color Black((unsigned int)0, 0, 0, 255);
	const Color Pink((unsigned int)255, 0, 255, 255);
	const Color Gray((unsigned int)128, 128, 128, 255);
	const Color DarkGray((unsigned int)64, 64, 64, 255);
}