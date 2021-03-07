#include "GameCamera.h"

GameCamera::GameCamera()
{
	Pos = Point2i(0,0);
	Bounds = Recti(0,0,0,0);
	ViewPort = Recti(0,0,0,0);
}

void GameCamera::SetPos(int x, int y)
{
	if (x < Bounds.left) x = Bounds.left;
	if (x > Bounds.right) x = Bounds.right;
	if (y < Bounds.top) y = Bounds.top;
	if (y > Bounds.bottom) y = Bounds.bottom;
	Pos.x = x;
	Pos.y = y;
}

void GameCamera::SetBounds(Recti bounds)
{
	Bounds = bounds;
}

Recti GameCamera::GetViewPort()
{
	//Rect view;
	Recti view (Pos.x / 64, Pos.y / 64,16+1,12+1);
	return view;
}
