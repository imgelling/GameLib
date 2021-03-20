#pragma once
#ifdef __linux__
#include <SDL2/SDL_opengl.h>
#else
#include <GL/glew.h>
#include <SDL_opengl.h>
#endif
#include "GameSpriteBatch.h"




class GamePixelMode
{
public:
	void CreateBuffers(int, int, int, int);
	void UpdateWindowBuffer();
	void SetPal(Color256[256]);
	void GetPal(Color256 *);
	Color256 GetPalLoc(int);
	void SetPalLoc(int, Color256);
	void Clear(unsigned char);
	void Flip(GameSpriteBatch *, bool);
	void LoadPalette();
	GamePixelMode();
	~GamePixelMode();
	GLuint *Video;
	Color256 pal[256];
private:

	GameTexture2D WindowBuffer[2];
	//GLuint pbo[2];
	int current;
	int width;
	int height;
	int gameWidth;
	int gameHeight;
	float scale;
	int posx, posy;
};