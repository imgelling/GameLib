#include "GameCamera.h"

GameCamera::GameCamera()
{
	Pos = Point(0,0);
	Bounds = Rect(0,0,0,0);
	ViewPort = Rect(0,0,0,0);
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

void GameCamera::SetBounds(Rect bounds)
{
	Bounds = bounds;
}

Rect GameCamera::GetViewPort()
{
	//Rect view;
	Rect view (Pos.x / 64, Pos.y / 64,16+1,12+1);
	return view;
}
