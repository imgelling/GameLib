// INCLUDES ////////////////////////////////////////////////////////////////////
#ifdef __linux__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#include <Windows.h>
#endif

#include "GameMouse.h"

#ifdef _WIN32

#endif

// METHODS /////////////////////////////////////////////////////////////////////
GameMouse::GameMouse()
{
    isShown = true;
    scrollX = 0;
    scrollY = 0;
    posx = 0;
    posy = 0;
    rposx = 0;
    rposy = 0;
    clamp = false;
	for (int i = 0; i < 5; i++)
	{
		buttons[i].pressed = false;
		buttons[i].clicks = -1;
	}
	buttMap[SDL_BUTTON_LEFT] = 0;
	buttMap[SDL_BUTTON_MIDDLE] = 1;
	buttMap[SDL_BUTTON_RIGHT] = 2;
	buttMap[SDL_BUTTON_X1] = 3;
	buttMap[SDL_BUTTON_X2] = 4;

	SetVisible(true);
}

GameMouse::~GameMouse()
{

}

// Mouse configuration methods
void GameMouse::SetVisible(bool isVisible)
{
	if (isVisible)
	{
		// THIS was REALLY wrong V
		//SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_ShowCursor(1);
		isShown = true;
	}
	else
	{
		SDL_ShowCursor(0);
		//SDL_SetRelativeMouseMode(SDL_TRUE);
		isShown = false;
	}
}

bool GameMouse::IsVisible()
{
	return isShown;
}

void GameMouse::ClampToWindow(bool clamptowin)
{
	clamp = clamptowin;
}

void GameMouse::SetPosition(int x, int y)
{
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	float scalex = (float)w / (float)gameWidth;
	float scaley = (float)h / (float)gameHeight;   // basic scaling code

	if (SDL_GetRelativeMouseMode())
	{
		rposx = int(x*scalex);
		rposy = int(y*scaley);
	}
	else
	{
		// Modified to make paddle movement work
		SDL_WarpMouseInWindow(win, int(x*scalex), int(y*scaley));
	}
}

void GameMouse::SetAcceleration(bool mouseAccel)
{
#ifdef _WIN32
	int mouseParams[3];

	// Get the current values.
	SystemParametersInfo(SPI_GETMOUSE, 0, mouseParams, 0);

	// Modify the acceleration value as directed.
	mouseParams[2] = mouseAccel;

	// Update the system setting.
	SystemParametersInfo(SPI_SETMOUSE, 0, mouseParams, SPIF_SENDCHANGE);
#endif
}

// Mouse data retrieval methods
Point2i GameMouse::GetMovement()
{
	Point2i ret;
	SDL_GetRelativeMouseState(&ret.x, &ret.y);
	return ret;
}

void GameMouse::SetRelativeMode(bool mode)
{
	if (mode)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}

Point2i GameMouse::GetPosition()
{
	int x, y;
	SDL_GetWindowSize(win, &x, &y);
	float scalex = gameWidth / (float)x;
	float scaley = gameHeight / (float)y;
	if (SDL_GetRelativeMouseMode() == SDL_TRUE)
	{
		return Point2i(int(rposx*scalex), int(rposy*scaley));
	}
	else
	{
		return Point2i(int(posx*scalex), int(posy*scaley));
	}
}

bool GameMouse::IsPressed(unsigned int key)
{
	return buttons[buttMap[key]].pressed;
}


bool GameMouse::Clicked(unsigned int butt)
{
	if (NumClicks(butt) >= 1)
	{
		buttons[buttMap[butt]].clicks =  -1;
		return true;
	}
	return false;
}
bool GameMouse::DblClicked(unsigned int butt)
{
	if (NumClicks(butt) >= 2)
	{
		buttons[buttMap[butt]].clicks = -1;
		return true;
	}
	return false;
}

Point2i GameMouse::GetWheelMovement()
{
	return Point2i(scrollX, scrollY);
}


// Internal Helper methods
int GameMouse::NumClicks(unsigned int butt)
{
	return buttons[buttMap[butt]].clicks;
}

// Following are used by the Game class to
// update the mouse
void GameMouse::SetWindow(SDL_Window* window, int w, int h)
{
	gameWidth = w;
	gameHeight = h;
	win = window;
}
void GameMouse::UpdateButtonEvent(SDL_MouseButtonEvent *event)
{
	unsigned int index = buttMap[event->button];
	event->state == SDL_PRESSED ? buttons[index].pressed = true : buttons[index].pressed = false;
	if (event->state == SDL_PRESSED)
	{
		buttons[index].pressed = true;
	}
	else
	{
		buttons[index].pressed = false;
		buttons[index].clicks = event->clicks;
	}
}

void GameMouse::UpdateMotionEvent(SDL_MouseMotionEvent *event)
{
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	float scalex = 1.0f;// (float)w / (float)gameWidth;
	float scaley = 1.0f;// (float)h / (float)gameHeight;
	// Mouse actual Position
	posx = int(event->x*scalex);
	posy = int(event->y*scaley);
	// relative

	rposx += int(event->xrel * scalex);
	rposy += int(event->yrel * scaley);
	if (clamp)
	{
		if (rposx < 0) rposx = 0;
		if (rposy < 0) rposy = 0;
		if (rposx > w) rposx = w;
		if (rposy > h) rposy = h;
	}
}

void GameMouse::ScaleMousePos(int ox, int oy)

{
	int x, y;
	SDL_GetWindowSize(win, &x, &y);

	// This finds the scalar between the old window size(ox,oy)
	// to the new window size(x,y)
	float scalex = ((float)x / (float)ox);
	float scaley = ((float)y / (float)oy);

	// Scale the relative position
	rposx = (int)(rposx * scalex);
	rposy = (int)(rposy * scaley);

	// Warp the real position
	SDL_WarpMouseInWindow(win, (int)(posx * scalex), (int)(posy * scaley));
}



void GameMouse::UpdateWheel(int x, int y)
{
	scrollX = x;
	scrollY = y;
}
