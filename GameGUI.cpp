#include "GameGUI.h"

GameGUI::GameGUI() :
guiSBatch(NULL)
{
}
GameGUI::GameGUI(GameSpriteBatch *batch, GameTexture2D tex, GameSpriteSheet sheet)
{
	guiSBatch = batch;
	guiSheet = sheet;
	guiTex = tex;
}
void GameGUI::Initialize(GameSpriteBatch *batch, GameTexture2D tex, GameSpriteSheet sheet)
{
	guiSBatch = batch;
	guiSheet = sheet;
	guiTex = tex;
}
void GameGUI::SetSpriteBatch(GameSpriteBatch *batch)
{
	guiSBatch = batch;
}
void GameGUI::SetSpriteSheet(GameSpriteSheet sheet)
{
	guiSheet = sheet;
}
void GameGUI::SetTexture(GameTexture2D tex)
{
	guiTex = tex;
}
void GameGUI::Box(Recti rect, Color color)
{
	int rx = rect.right;
	int ry = rect.bottom;
	int x = rect.left;
	int y = rect.top;
	int width;
	int height;
	int t;

	if (rx < x)
	{
		t = x;
		x = rx;
		rx = t;
	}
	if (ry < y)
	{
		t = y;
		y = ry;
		ry = t;
	}

	width = rx - x;
	height = ry - y;

	if (width < 16) width = 16;
	if (height < 16) height = 16;

	// TL
	guiSBatch->Draw(guiTex, Recti(x, y, 16, 16), guiSheet.GetRectFromId(11), color);
	// TR
	guiSBatch->Draw(guiTex, Recti(x + width - 16, y, 16, 16), guiSheet.GetRectFromId(12), color);
	// Top
	guiSBatch->Draw(guiTex, Recti(x + 16, y, width - 32, 16), guiSheet.GetRectFromId(16), color);

	// BL
	guiSBatch->Draw(guiTex, Recti(x, y + height - 16, 16, 16), guiSheet.GetRectFromId(13), color);
	// BR
	guiSBatch->Draw(guiTex, Recti(x + width - 16, y + height - 16, 16, 16), guiSheet.GetRectFromId(14), color);
	// Bottom
	guiSBatch->Draw(guiTex, Recti(x + 16, y + height - 16, width - 32, 16), guiSheet.GetRectFromId(18), color);

	// Left
	guiSBatch->Draw(guiTex, Recti(x, y + 16, 16, height - 16), guiSheet.GetRectFromId(15), color);

	// Right
	guiSBatch->Draw(guiTex, Recti(x + width - 16, y + 16, 16, height - 16), guiSheet.GetRectFromId(17), color);

}
void GameGUI::HScrollBar(int x, int y, int big, float percent, Color color)
{
	int modbig = big - 32 - 16;
	int bar = int(modbig * percent);
	// 4 5 6 8 left right bar scroll
	// left arrow
	guiSBatch->Draw(guiTex, Recti(x, y, 16, 16), guiSheet.GetRectFromId(4), color);
	// right arrow
	guiSBatch->Draw(guiTex, Recti(x + big - 16, y, 16, 16), guiSheet.GetRectFromId(5), color);
	// bar
	guiSBatch->Draw(guiTex, Recti(x + 16, y, big - 32, 16), guiSheet.GetRectFromId(6), color);

	// draw scroll
	//                            x+16+bay-8 (half)
	guiSBatch->Draw(guiTex, Recti(x + 16 + bar, y, 16, 16), guiSheet.GetRectFromId(8), color);
}
void GameGUI::VScrollBar(int x, int y, int big, float percent, Color color)
{
	int modbig = big - 32 - 16;
	int bar = int(modbig * percent);
	// 2 3 7 9 left right bar scroll
	// left arrow
	guiSBatch->Draw(guiTex, Recti(x, y, 16, 16), guiSheet.GetRectFromId(2), color);
	// right arrow
	guiSBatch->Draw(guiTex, Recti(x, y + big - 16, 16, 16), guiSheet.GetRectFromId(3), color);
	// bar
	guiSBatch->Draw(guiTex, Recti(x, y + 16, 16, big - 32), guiSheet.GetRectFromId(7), color);

	// draw scroll
	guiSBatch->Draw(guiTex, Recti(x, y + 16 + bar, 16, 16), guiSheet.GetRectFromId(8), color);
}
void GameGUI::Radial(int x, int y, bool checked, Color color)
{
	if (checked)
		guiSBatch->Draw(guiTex, Recti(x, y, 16, 16), guiSheet.GetRectFromId(1), color);
	else
		guiSBatch->Draw(guiTex, Recti(x, y, 16, 16), guiSheet.GetRectFromId(0), color);
}
