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
	// Clears the buffer to a color
	void Clear(unsigned int);
	// Clears the buffer to a color
	void Clear(Color);
	// Clears the buffer to 0
	void Clear();
	void Flip(GameSpriteBatch *, bool);
	//void LoadPalette();
	GamePixelMode();
	~GamePixelMode();
	GLuint *Video;
private:

	GameTexture2D WindowBuffer[2];
	int current;
	int width;
	int height;
	int gameWidth;
	int gameHeight;
};