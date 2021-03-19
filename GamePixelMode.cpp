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
	//Color256 pal[256];
	//Color256 pal;
	for (int cols = 0; cols < 256; cols++)
	{

		in >> r;
		in >> g;
		in >> b;
		pal[cols].Set((unsigned char)r, (unsigned char)g, (unsigned char)b);
		//screen.SetPalLoc(cols, pal);
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
			sx = sy;// gameHeight / gameHeight;
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
		sb->Draw(WindowBuffer, Recti(0, 0, gameWidth, gameHeight), Recti(0, 0, width, height), Colors::White);
	else
		sb->Draw(WindowBuffer, Recti(posx, posy, (int)(width * sx), (int)(height * sy)), Recti(0, 0, width, height), Colors::White);
	// 160, 60, *3 for 720
	// 106, 40, *5 for 1080
}
GamePixelMode::~GamePixelMode()
{
	delete[] Video;
//	glDeleteBuffers(2, pbo);
	glDeleteTextures(1, &WindowBuffer.bind);
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
		glDeleteTextures(1, &WindowBuffer.bind);
	}
	Video = NULL;
	GLuint t = width * height;
	Video = new GLuint[t];

	current = 0;
	

	float w = gameWidth / (float)width;
	float h = gameHeight / (float)height;
	w < h ? scale = w : scale = h;
	posx = (int)((gameWidth - (width * scale)) / 2);
	posy = (int)((gameHeight - (height * scale)) / 2);

	glGenTextures(1, &WindowBuffer.bind);
	glBindTexture(GL_TEXTURE_2D, WindowBuffer.bind);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	GLfloat largest_supported_anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);

	WindowBuffer.width = width;
	WindowBuffer.height = height;
	WindowBuffer.widthDiv = 1.0f / (float)WindowBuffer.width;
	WindowBuffer.heightDiv = 1.0f / (float)WindowBuffer.height;

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
	//TODO: Uses 2 PBO to flip back and forth from, not sure if faster.
	//  turned off to see if stops stutter
	glBindTexture(GL_TEXTURE_2D, WindowBuffer.bind);
	// inserted for test to get rid of stutter
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, NULL);
	
	//glBindBuffer(GL_PIXEL_PACK_BUFFER, current);



	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *)Video); //old
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, (GLvoid *)Video); //new
	//glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return;
}
void GamePixelMode::Clear(unsigned char color)
{
	std::fill_n(Video, width*height, pal[color].PackedColor);
}
