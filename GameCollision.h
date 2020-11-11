#pragma once
#include "GameClasses.h"

#define randf() ((float)(rand() / (float)RAND_MAX))
#define gameABS(x) ((x) >= 0 ? (x) : -(x))
//inline int absi(int x)
//{
//	return (x >= 0 ? x : -x);
//}

inline float absf(float x)
{
	return (x >= 0 ? x : -x);
}

class GameCollision
{
	public:
		GameCollision();
		bool RectIntersect(Rect, Rect);
		bool PointRectIntersect(Point, Rect);
		Point RectCollPushBack(Rect, Rect);
	private:
};
