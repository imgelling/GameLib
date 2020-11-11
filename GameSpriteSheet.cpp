#include "GameSpriteSheet.h"

GameSpriteSheet::GameSpriteSheet() : tileWidth(0) , tileHeight(0), texture(0), tilePerRow(0)
{
}

void GameSpriteSheet::Initialize(GameTexture2D *tex, int width, int height)
{
	texture = tex;
	tileWidth = width;
	tileHeight = height;
	tilePerRow = texture->width / width;// ((float)texture->width / (float)width);
}
void GameSpriteSheet::Initialize(GameTexture2D *tex, Point dim)
{
	texture = tex;
	tileWidth = dim.x;
	tileHeight = dim.y;
	tilePerRow = texture->width / tileWidth;// int((float)texture->width / (float)tileWidth);
}


Rect GameSpriteSheet::GetRectFromId(int id)
{
	Rect ret(0,0,0,0);
	if (texture == NULL) return ret;
	ret.left = id % tilePerRow * tileWidth;
	ret.top = id / tilePerRow * (tileHeight); //tileHeight;
	ret.right = ret.left + tileWidth;
	ret.bottom = ret.top + tileHeight;
	return ret;
}
