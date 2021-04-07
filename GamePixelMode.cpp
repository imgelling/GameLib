#ifdef __linux__
#include <GL/glew.h>
#else

//#include <GL/glew.h>
#endif
#include "GamePixelMode.h"
#include <fstream>
#include <sstream>

void GamePixelMode::LoadPalette()
{
	std::stringstream str;
	std::ifstream in("Content/pal.txt");
	int r, g, b;
	for (int cols = 0; cols < 256; cols++)
	{
		in >> r;
		in >> g;
		in >> b;
		pal[cols].Set((unsigned char)r, (unsigned char)g, (unsigned char)b);
	}
	in.close();
}

void GamePixelMode::GetPal(Color256 *p)
{
	for (int i = 0;i < 256;i++)
	{
		p[i] = pal[i];
	}
}
Color256 GamePixelMode::GetPalLoc(int loc)
{
	return pal[loc];
}
void GamePixelMode::SetPalLoc(int loc, Color256 color)
{
	pal[loc] = color;
}
// Might be best to change flip to draw 1 triangle instead of SpriteBatch
void GamePixelMode::Flip(GameSpriteBatch *sb, bool full)
{
	UpdateWindowBuffer();

	posx = posy = 0;
	double sx = 0;
	double sy = 0;
	double temp = 0;
	if (gameHeight < gameWidth)
	{

		sy = (double)gameHeight / (double)height;
		temp = (double)gameWidth / (double)width;
		if (temp > sy)
		{
			sx = sy;
		}
		else
		{
			sx = sy = temp;
			posy = (int)((double)gameHeight / 2.0 - ((double)height*sy / 2.0)); // gameHeight / 2 - (height*sy / 2);
		}
		posx = (int)((double)gameWidth / 2.0 - ((double)width*sx / 2.0));
		
	}
	else if (gameHeight > gameWidth)
	{
		sx = (double) gameWidth / (double)width;
		sy = sx;
		posy = (int)((double)gameHeight / 2.0 - ((double)height*sy / 2.0));
	}
	else
	{
		sx = sy = 1.0;

	}

	if (full)
		sb->Draw(WindowBuffer[current], Recti(0, 0, gameWidth, gameHeight), Recti(0, 0, width, height), Colors::White);
	else
		sb->Draw(WindowBuffer[current], Recti(posx, posy, (int)(width * sx), (int)(height * sy)), Recti(0, 0, width, height), Colors::White);
}
GamePixelMode::~GamePixelMode()
{
	if (Video != NULL)
		delete[] Video;
	glDeleteTextures(1, &WindowBuffer[0].bind);
	glDeleteTextures(1, &WindowBuffer[1].bind);
}
void GamePixelMode::SetPal(Color256 palette[256])
{
	for (int i = 0; i < 256; i++)
	{
		pal[i] = palette[i];
	}
}
// Create Window Buffer does not create a pow2 texture
void GamePixelMode::CreateBuffers(int width, int height, int resW, int resH)
{
	this->width = width;
	this->height = height;
	gameWidth = resW;
	gameHeight = resH;
	if (Video != NULL)
	{
		delete[] Video;
		glDeleteTextures(1, &WindowBuffer[0].bind);
		glDeleteTextures(1, &WindowBuffer[1].bind);
	}
	GLuint t = width * height;
	Video = new GLuint[t];

	current = 0;
	

	float w = gameWidth / (float)width;
	float h = gameHeight / (float)height;
	w < h ? scale = w : scale = h;
	posx = (int)((gameWidth - (width * scale)) / 2);
	posy = (int)((gameHeight - (height * scale)) / 2);
	for (int i = 0; i < 2; i++)
	{
		glGenTextures(1, &WindowBuffer[i].bind);
		glBindTexture(GL_TEXTURE_2D, WindowBuffer[i].bind);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);

		WindowBuffer[i].width = width;
		WindowBuffer[i].height = height;
		WindowBuffer[i].widthDiv = 1.0f / (float)WindowBuffer[i].width;
		WindowBuffer[i].heightDiv = 1.0f / (float)WindowBuffer[i].height;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}
GamePixelMode::GamePixelMode()
{
	current = 0;
	scale = 0;
	posx = posy = 0;
	width = height = 0;
	gameWidth = gameHeight = 0;
	LoadPalette();
	Video = NULL;
}
void GamePixelMode::UpdateWindowBuffer()
{
	glBindTexture(GL_TEXTURE_2D, WindowBuffer[current].bind);
	// Swap texture to draw to
	current++;
	if (current > 1) current = 0;

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, (GLvoid *)Video); 
	glBindTexture(GL_TEXTURE_2D, 0);
	return;
}

void GamePixelMode::Clear(unsigned char color)
{
	std::fill_n(Video, width*height, pal[color].PackedColor);
}

void GamePixelMode::Clear()
{
	memset((void*)Video, 0, (size_t)width * (size_t)height * sizeof(GLuint));
}
