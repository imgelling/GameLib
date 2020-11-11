#pragma once
// INCLUDES ////////////////////////////////////////////////////////////////////

#ifdef __linux__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include "GameClasses.h"
#include <map>
// Implement a get mouse position/movement scaled?

//#define GameButtonLeft  SDL_BUTTON_LEFT
//#define GameButtonMiddle SDL_BUTTON_MIDDLE
//#define GameButtonRight SDL_BUTTON_RIGHT
//#define GameButton4 SDL_BUTTON_X1
//#define GameButton5 SDL_BUTTON_X2


// DECLARATIONS ///////////////////////////////////////////////////////////////

class GameMouse
{
	friend class Game;
	struct Button
	{
		bool pressed;
		int clicks;
	};

public:
	GameMouse();
	~GameMouse();
	Point GetPosition();
	Point GetMovement();
	Point GetWheelMovement();
	bool IsPressed(unsigned int);
	bool Clicked(unsigned int);
	bool DblClicked(unsigned int);
	void SetPosition(int, int);
	void SetAcceleration(bool);
	void SetRelativeMode(bool);

	void SetVisible(bool);
	bool IsVisible();
	void ClampToWindow(bool);

private:
	void UpdateWheel(int, int);
	void ScaleMousePos(int, int);
	void UpdateButtonEvent(SDL_MouseButtonEvent *);
	void UpdateMotionEvent(SDL_MouseMotionEvent *);
	void SetWindow(SDL_Window*, int, int);
	inline int NumClicks(unsigned int);
	bool clamp;
	bool isShown;
	int scrollX;
	int scrollY;
	int posx, posy;
	int rposx, rposy;
	int gameWidth;
	int gameHeight;
	std::map<unsigned int, unsigned int> buttMap;
	Button buttons[5];
	SDL_Window *win;
};

