#include "GameCollision.h"

GameCollision::GameCollision()
{
}

bool GameCollision::PointRectIntersect(Point point, Rect rect)
{
	if (point.x < rect.left) return false;
	if (point.x > rect.right) return false;
	if (point.y < rect.top) return false;
	if (point.y > rect.bottom) return false;

	return true;
}

bool GameCollision::RectIntersect(Rect one, Rect two)
{
	if (one.bottom < two.top) return false;
	if (one.top > two.bottom) return false;
	if (one.right < two.left) return false;
	if (one.left > two.right) return false;

	return true;
}

Point GameCollision::RectCollPushBack(Rect one, Rect two)
{
	Point push(0,0);

	int x1 = gameABS(one.right - two.left);
	int x2 = gameABS(one.left - two.right);
	int y1 = gameABS(one.bottom - two.top);
	int y2 = gameABS(one.top - two.bottom);

	// Solid blocks

	if (y1 < y2)
	{
		push.y = -y1;  // push up
	}
	else if (y1 > y2)
	{
		push.y = y2;  // push down
	}
	// if exactly right on top/below, just push up/down
	if (x1 == x2) return push;


	if (x1 < x2)
	{
		push.x = -x1; // push left
	}
	else if (x1 > x2)
	{
		push.x = x2;  // push right
	}

	if (y1 == y2)
		return push;


	// Shorter to push horizontal
	if (gameABS(push.y) > gameABS(push.x))
		push.y = 0;
	else
		// Shorter to push vertically
		if (gameABS(push.y) < gameABS(push.x))
			push.x = 0;
		else
			// Don't push anywhere
		{
			push.x = 0;
			push.y = 0;
		}

		return push;
}
