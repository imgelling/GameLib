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
#include <SDL_vulkan.h>
#include <SDL_opengl.h>
//#include <SDL_mixer.h>
#endif


#include "GameAnimation.h"
#include "GameCamera.h"
#include "GameClasses.h"
#include "GameCollision.h"
#include "GameContent.h"
#include "GameDebug.h"
#include "GameErrors.h"
#include "GameFPS.h"
#include "GameGUI.h"
#include "GameLog.h"
#include "GameMath.h"
#ifdef GAME_DO_NETWORK
#include "GameNetwork.h"
#endif
#include "GamePerf.h"
#include "GamePixelMode.h"
#include "GameRandom.h"
#include "GameSoftware3d_Camera.h"
#include "GameSoftware3d_Data.h"
#include "GameSpriteBatch.h"
#include "GameSpriteSheet.h"
#include "GameThreadPool.h"
#include "GameTimer.h"
#include "GameWindow.h"






