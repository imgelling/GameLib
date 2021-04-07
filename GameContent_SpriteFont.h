#pragma once
// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameContent_TextureManager.h"

#include <string>

// DEFINES ////////////////////////////////////////////////////////////////////



// DECLARATIONS ///////////////////////////////////////////////////////////////
struct FontDescriptor
{
	unsigned short x, y;
	unsigned short width, height;
	short xOffset, yOffset;
	unsigned short xAdvance;
	unsigned short page;

	FontDescriptor()
	{
		y = x = 0;
		width = height = 0;
		xOffset = yOffset = 0;
		xAdvance = 0;
		page = 0;
	}
};

struct Charset
{
	unsigned short lineHeight = 0;
	unsigned short base = 0;
	unsigned short width = 0, height = 0;
	unsigned short pages = 0;
	FontDescriptor chars[256];
};

class GameSpriteFont
{
	friend class GameContent;
	friend class GameSpriteBatch;
	public:
		GameSpriteFont();
		unsigned int Length(std::string text);


	private:
		bool Load(std::string filename, GameTexture2D & text);
		void UnLoad();
		GameTexture2D Texture() { return texture; }
		GameTexture2D texture;
		Charset set;
};
