#pragma once
#include "GameClasses.h"
#include "GameMath.h"

#define gameABS(x) ((x) >= 0 ? (x) : -(x))

class GameCollision
{
	public:
		GameCollision();
		bool RectIntersect(Recti, Recti);
		bool PointRectIntersect(Point2i, Recti);
		Point2i RectCollPushBack(Recti, Recti);
	private:
};
