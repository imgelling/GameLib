#pragma once
#include "GameClasses.h"
#include "GameMath.h"


// need to add accessors and box it up
class GameCamera
{
public:
	GameCamera();
	Point2i Pos;
	Recti Bounds;
	Recti ViewPort;
	void SetPos(int, int);
	void SetBounds(Recti);
	Recti GetViewPort();
};