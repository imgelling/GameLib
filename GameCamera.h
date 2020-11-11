#pragma once
#include "GameClasses.h"


// need to add accessors and box it up
class GameCamera
{
public:
	GameCamera();
	Point Pos;
	Rect Bounds;
	Rect ViewPort;
	void SetPos(int, int);
	void SetBounds(Rect);
	Rect GetViewPort();
};