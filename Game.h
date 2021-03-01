#pragma once

#define SDL_MAIN_HANDLED


#ifdef __linux__
#include "GL/glew.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#else
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
//#include <SDL_mixer.h>
#endif




#include "GameSpriteBatch.h"
#include "GameWindow.h"
#include "GameTimer.h"
#include "GameLog.h"
#include "GameClasses.h"
#include "GameFPS.h"
#include "GameContent.h"
#include "GameSpriteSheet.h"
#include "GameCamera.h"
#include "GameCollision.h"
#include "GameAnimation.h"
#include "GameGUI.h"
#include "GamePixelMode.h"
#include "GameDebug.h"
#include "GameThreadPool.h"
#ifdef GAME_DO_NETWORK
#include "GameNetwork.h"
#endif






