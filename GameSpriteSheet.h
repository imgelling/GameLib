#pragma once
#include "GameClasses.h"
#include "GameContent_TextureManager.h"

class GameSpriteSheet
{
	public:
		GameSpriteSheet();
		void Initialize(GameTexture2D *tex, int width, int height);
		void Initialize(GameTexture2D *tex, Point dim);
		void SetTexture(GameTexture2D *tex);
		Rect GetRectFromId(int id);
	private:
		int tileWidth;
		int tileHeight;
		int tilePerRow;
		GameTexture2D *texture;
};
