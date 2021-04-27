#pragma once
// INCLUDES ////////////////////////////////////////////////////////////////////

#ifdef __linux__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include "GameLog.h"
#include "GameKeyboard.h"
#include "GameMouse.h"
#include "GameFPS.h"
#include "GamePerf.h"



// LIBRARIES ///////////////////////////////////////////////////////////////////
#ifdef __linux__

#else
#pragma comment(lib,"SDL2")
#pragma comment(lib,"OpenGL32")
#pragma comment(lib,"SDL2_image")
//#pragma comment(lib,"SDL2_mixer")
#endif




// DECLARATIONS ////////////////////////////////////////////////////////////////

struct GameAttributes
{
	int GL_ContextMajor;
	int GL_ContextMinor;
	int GL_RedSize;
	int GL_BlueSize;
	int GL_GreenSize;
	int GL_AlphaSize;
	int GL_DoubleBuffer;
	int GL_DepthSize;
	int GL_MultiSamples;
	int GL_Debug;
	double Framelock;
	unsigned int Framework;

	GameAttributes()
	{
		GL_ContextMajor = 0;
		GL_ContextMinor = 0;
		GL_RedSize = 0;
		GL_BlueSize = 0;
		GL_GreenSize = 0;
		GL_AlphaSize = 0;
		GL_DoubleBuffer = -1;
		GL_DepthSize = 0;
		GL_MultiSamples = 0;
		GL_Debug = 0;
		Framelock = 0;
		Framework = SDL_WINDOW_OPENGL; // Defaults to OpenGL
	}
};

struct VideoInfo
{
	GLint internalPixelFormat = 0;
	GLint internalPixelType = 0;
};


class Game
{
    public:
        Game();
        ~Game();
        void ToggleFullscreen(bool);
		void SetFramelock(double);
        void Run();
		// Sets the window info, Title, Width, Height, Fullscreen, Borderless
        void SetWindowInfo(std::string, int, int, bool, bool);
		void SetAttributes(GameAttributes);
        void ExitGame();
		void SetLog(GameLog *);
        virtual void Initialize()=0;
        virtual void LoadContent()=0;
        virtual void Shutdown()=0;
        virtual void Render(const double &MSelapsed)=0;
        virtual void Update(const double &MSelapsed)=0;

		// VideoStuff
		VideoInfo videoInfo;
		int GameWidth() { return gameWidth; }
		int GameHeight() { return gameHeight; }

		void Present();
        GameLog *Logger;

		// Input
		GameKeyboard Keyboard;
		GameMouse Mouse;


		bool WindowChanged()
		{
			return windowChanged;
		}
		void WindowChangedHandled()
		{
			windowChanged = false;
		}

		PerfTimer perf;

    private:
        bool CreateTheWindow();
		void LogWindowInfo(bool);
		void LogGraphicsCard();
		void LogOpenGLInfo();
		void LogSDLInfo();


		SDL_Window* window;
		SDL_GLContext glcontext;
		const float MAX_FRAME_TIME = 5000.0f / 60.0f;


        bool Quit;
        std::string WindowTitle;
        int gameWidth;
        int gameHeight;
		int framelock;
        bool isFullScreen;
		bool isBorderless;
		bool isLogSet;
		bool windowChanged;
		GameFPSLock fpslock;
		GameTimer RenderTime;
		GameTimer UpdateTime;
		GameAttributes WindowAttrib;
};

