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
#endif




// DECLARATIONS ////////////////////////////////////////////////////////////////


struct GameAttributes
{
	uint8_t GL_ContextMajor;
	uint8_t GL_ContextMinor;
	uint8_t GL_RedSize;
	uint8_t GL_BlueSize;
	uint8_t GL_GreenSize;
	uint8_t GL_AlphaSize;
	uint8_t GL_DoubleBuffer;
	uint8_t GL_DepthSize;
	uint8_t GL_MultiSamples;
	bool GL_Debug;
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
		GL_Debug = false;
		Framelock = 0;
		Framework = SDL_WINDOW_OPENGL; // Defaults to OpenGL
	}
};


namespace game
{
	// Contains information about the host's system
	class SystemInfo
	{
	private:
		// Contains information about the host's cpu
		struct CPUInfo
		{
		public:
			unsigned int processorCount = 0;
		};
		// Contains information about the host's gpu being used
		struct GPUInfo
		{
			int internalPixelFormat = 0;
			int internalPixelType = 0;
			int totalMemory = 0;
			int freeMemory = 0;
			int glVersionMajor = 0;
			int glVersionMinor = 0;
			int glShaderLanguageVersion = 0;
			int glMultisampleBuffers = 0;
			int glMultisampleSamples = 0;
			Color frontBufferColorSize;
			Color backBufferColorSize;
			int depthBufferSize = 0;
			std::string renderer;
			std::string vendor;
		};
	public:
		CPUInfo cpuInfo;
		GPUInfo gpuInfo;
	};
}

class Game
{
    public:
		game::SystemInfo systemInfo;
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
        virtual void Render(const float &)=0;
        virtual void Update(const float &)=0;

		// VideoStuff
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

		// test
		static void GLAPIENTRY openglCallbackFunction(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam);
};

