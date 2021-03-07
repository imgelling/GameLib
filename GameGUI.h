#pragma once

#include "GameSpriteBatch.h"
#include "GameSpriteSheet.h"
#include "GameMath.h"

class GameGUI
{
public:
	GameGUI();
    GameGUI(GameSpriteBatch *batch, GameTexture2D tex, GameSpriteSheet sheet);
	void Initialize(GameSpriteBatch *batch, GameTexture2D tex, GameSpriteSheet sheet);
	void SetSpriteBatch(GameSpriteBatch *);
	void SetTexture(GameTexture2D);
	void SetSpriteSheet(GameSpriteSheet);
	void Box(Recti, Color);
	void HScrollBar(int, int, int, float, Color);
	void VScrollBar(int, int, int, float, Color);
	void Radial(int, int, bool, Color);


private:
	GameSpriteSheet guiSheet;
	GameSpriteBatch *guiSBatch;
	GameTexture2D guiTex;

};
