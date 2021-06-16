// INCLUDES ////////////////////////////////////////////////////////////////////
#include <gl\glew.h>
#include <SDL.h>
#include <string>
#include "GameWindow.h"
#include "GameDebug.h"


SystemInfo systemInfo;

static void GLAPIENTRY openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	// Some debug messages are just annoying informational messages
	switch (id)
	{
	case 131169: // The driver allocated storage for renderbuffer
	case 131185: // glBufferData
	case 131184: // VBO buffer data
	case 131218: // NVIDIA recompile msg
		return;
	}

	printf("Message: %s\n", message);
	printf("Source: ");

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		printf("API");
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		printf("Window System");
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		printf("Shader Compiler");
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		printf("Third Party");
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		printf("Application");
		break;
	case GL_DEBUG_SOURCE_OTHER:
		printf("Other");
		break;
	}

	printf("\n");
	printf("Type: ");

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		printf("Error");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		printf("Deprecated Behaviour");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		printf("Undefined Behaviour");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		printf("Portability");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		printf("Performance");
		break;
	case GL_DEBUG_TYPE_MARKER:
		printf("Marker");
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		printf("Push Group");
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		printf("Pop Group");
		break;
	case GL_DEBUG_TYPE_OTHER:
		printf("Other");
		break;
	}

	printf("\n");
	printf("ID: %d\n", id);
	printf("Severity: ");

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		printf("High");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		printf("Medium");
		break;
	case GL_DEBUG_SEVERITY_LOW:
		printf("Low");
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		printf("Notification");
		break;
	}
	printf("\n\n");
	return;
}

void Game::SetLog(GameLog * log)
{
	Logger = log;
	isLogSet = true;
}

Game::Game()
{
    Quit = false;
    WindowTitle = "Game";
    gameWidth = 640;
    gameHeight = 480;
    isFullScreen = false;
	window = NULL;
	glcontext = NULL;
	isBorderless = false;
	isLogSet = false;
	windowChanged = false;
	framelock = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		Logger->Error("Could not initialize SDL");
	}

}


Game::~Game()
{
	// Turn off keyboard input
	if (Keyboard.TextInputIsOn())
	{
		Keyboard.EndInputText();
	}
	//Destroy window
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
}

void Game::ExitGame()
{
    Quit = true;
}

void Game::SetWindowInfo(std::string Title, int Width, int Height, bool Fullscreen, bool Borderless)
{
    WindowTitle = Title;
    gameWidth = Width;
    gameHeight = Height;
    isFullScreen = Fullscreen;
	isBorderless = Borderless;
}

void Game::LogGraphicsCard()
{
	std::stringstream str;
	// Graphics card info
	str << "Renderer is " << glGetString(GL_RENDERER) << ".";
	systemInfo.gpuInfo.renderer = str.str();
	Logger->Write(str.str());
	str.str(std::string());
	str << glGetString(GL_VENDOR);
	std::string test = str.str();
	systemInfo.gpuInfo.vendor = test;
	str.str(std::string());
	GLint total_mem_kb[4] = { 0 };
	GLint cur_avail_mem_kb[4] = { 0 };
	if (test.find("NVIDIA") != std::string::npos)
	{
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
			&total_mem_kb[0]);

		
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
			&cur_avail_mem_kb[0]);
		systemInfo.gpuInfo.totalMemory = total_mem_kb[0] / 1024;
		systemInfo.gpuInfo.freeMemory = cur_avail_mem_kb[0] / 1024;
		str << "GPU total memory is " << total_mem_kb[0] / 1024.0f << "MB and has " << cur_avail_mem_kb[0] / 1024.0f << "MB available.";
		Logger->Write(str.str());
		str.str(std::string());
#undef GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX
#undef GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 
	}
	else
	{
		str << "Can not retrieve video RAM on AMD.";
		Logger->Write(str.str());
	}
	GLint pixelFormat, pixelType;
	glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &pixelFormat);
	glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_TYPE, 1, &pixelType);
	systemInfo.gpuInfo.internalPixelFormat = pixelFormat;
	systemInfo.gpuInfo.internalPixelType = pixelType;
	str.str(std::string());
	str << "Internal texture format is " << pixelFormat << " and type is " << pixelType;
	Logger->Write(str.str());
}

void Game::LogOpenGLInfo()
{
	std::stringstream str;
	GLint glMajor, glMinor;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glMajor);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glMinor);

	systemInfo.gpuInfo.glVersionMajor = glMajor;
	systemInfo.gpuInfo.glVersionMinor = glMinor;

	Logger->Write("OpenGL version " +
		(std::string)(const char *)glGetString(GL_VERSION) +
		" detected.");
	str << "OpenGL Context is version " << glMajor << "." << glMinor << ".";
	Logger->Write(str.str());
	str.str(std::string());
	str << "GLSL version ";
	systemInfo.gpuInfo.glShaderLanguageVersion = *glGetString(GL_SHADING_LANGUAGE_VERSION);
	str << systemInfo.gpuInfo.glShaderLanguageVersion;
	str << " detected.";
	Logger->Write(str.str());
	str.str(std::string());

	int MSbuff, MSsamp;
	SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &MSbuff);
	systemInfo.gpuInfo.glMultisampleBuffers = MSbuff;
	if (MSbuff > 0)
	{
		SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &MSsamp);
		systemInfo.gpuInfo.glMultisampleSamples = MSsamp;
		str << "Multisampling enabled with " << MSsamp << " samples.";
		Logger->Write(str.str());
	}
	else
		Logger->Write("Multisampling disabled.");
}

void Game::LogWindowInfo(bool verbose)
{
	int r, g, b, a;
	int br, bg, bb, ba;
	int w, h;
	int back, depth = 1;
	std::stringstream str;

	//Front Buffer
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &r);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &g);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &b);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &a);
	systemInfo.gpuInfo.frontBufferColorSize.Set((unsigned int)r, g, b, a);
	// Back Buffer
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &br);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &bg);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &bb);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &ba);
	back = br + bg + bb + ba;
	systemInfo.gpuInfo.backBufferColorSize.Set((unsigned int)br, bg, bb, ba);

	// Depth Buffer
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth);
	systemInfo.gpuInfo.depthBufferSize = depth;

	SDL_GetWindowSize(window, &w, &h);

	if (verbose)
	{
		LogGraphicsCard();
		LogOpenGLInfo();
	}
	

	// Window info
	str << "Window is " << w << "x" << h << "x" << (r + g + b + a)
		<< " with format " << r << g << b << a;
	if (isFullScreen)
		str << " and is Full Screen.";
	else
	{
		if (!isBorderless)
			str << " and is Windowed.";
		else
			str << " and is Borderless Windowed.";
	}

	Logger->Write(str.str());
	str.str(std::string());
	if (verbose)
	{
		str << "Backbuffer is " << back << "bits"
			<< " with format " << br << bg << bb << ba;
		Logger->Write(str.str());
		str.str(std::string());
		str << "Depthbuffer is " << depth << "bits.";
		Logger->Write(str.str());
	}
}

void Game::LogSDLInfo()
{
	SDL_version compiled;
	SDL_version linked;
	std::stringstream sdl;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	sdl << "Compiled against SDL version " << (int)compiled.major << "." << (int)compiled.minor << "." << (int)compiled.patch << ".";
	Logger->Write(sdl.str());
	sdl.str(std::string());
	sdl << "Linked against SDL version " << (int)linked.major << "." << (int)linked.minor << "." << (int)linked.patch << ".";
	Logger->Write(sdl.str());
}

void Game::SetAttributes(GameAttributes attributes)
{
	if (attributes.GL_Debug)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		if (attributes.GL_ContextMajor < 4) attributes.GL_ContextMajor = 4;
		if (attributes.GL_ContextMinor < 3) attributes.GL_ContextMinor = 3;
	}
	
	// set the opengl context version
	if (attributes.GL_ContextMajor)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, attributes.GL_ContextMajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, attributes.GL_ContextMinor);
		
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
	if (attributes.GL_RedSize)
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, attributes.GL_RedSize);
	if (attributes.GL_BlueSize)
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, attributes.GL_BlueSize);
	if (attributes.GL_GreenSize)
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, attributes.GL_GreenSize);
	if (attributes.GL_AlphaSize)
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, attributes.GL_AlphaSize);
	if (attributes.GL_DoubleBuffer >= 0)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, attributes.GL_DoubleBuffer);
	if (attributes.GL_DepthSize)
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, attributes.GL_DepthSize);
	if (attributes.GL_MultiSamples)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, attributes.GL_MultiSamples);
	}


	WindowAttrib = attributes;



}

bool Game::CreateTheWindow()
{
	std::stringstream str;

	unsigned int flags = 0;
	if (isFullScreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else
	{
		if (isBorderless)
			flags |= SDL_WINDOW_BORDERLESS;
		else
			flags |= SDL_WINDOW_RESIZABLE;
	}
	if (WindowAttrib.Framework == SDL_WINDOW_OPENGL)
	{
		flags |= SDL_WINDOW_OPENGL;
	}
	else if (WindowAttrib.Framework == SDL_WINDOW_VULKAN)
	{
		flags |= SDL_WINDOW_VULKAN;
	}



	window = SDL_CreateWindow(WindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameWidth, gameHeight, flags);
	if (window == NULL)
	{
		str.str(std::string());
		str << "Could not create a window. SDL Error: " << SDL_GetError() << std::endl;

		Logger->Error(str.str().c_str());
		return false;
	}
	if (WindowAttrib.Framework == SDL_WINDOW_OPENGL)
	{
		glcontext = SDL_GL_CreateContext(window);
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			Logger->Error("Could not initialize GLEW");
		}

		if (WindowAttrib.GL_Debug)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			glDebugMessageCallback(openglCallbackFunction, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}
	Mouse.SetWindow(window,gameWidth,gameHeight);

	LogSDLInfo();
	LogWindowInfo(true);
	return true;
}

void Game::Present()
{
	SDL_GL_SwapWindow(window);
}

// ToggleFullscreen will swap from windowed to fullscreen
// desktopMode : (True) will use a borderless fullscreen window
// desktopMode : (False) will use a real fullscreen
void Game::ToggleFullscreen(bool desktopMode)
{
	int w, h;
	Logger->Line();
	if (!isFullScreen)
	{
		SDL_GetWindowSize(window, &w, &h);
		if (desktopMode)
		{
			Logger->Write("Swapping to borderless full screen mode.");
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); // fake, for real remove desktop
			Mouse.ScaleMousePos(w, h);
		}
		else
		{
			Logger->Write("Swapping to full screen mode.");
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			Mouse.ScaleMousePos(w, h);
		}
	}
	else
	{
		SDL_GetWindowSize(window, &w, &h);
		Logger->Write("Swapping to windowed mode.");
		SDL_SetWindowFullscreen(window, 0);
		Mouse.ScaleMousePos(w, h);
	}

	SDL_GetWindowSize(window, &w, &h);
	gameWidth = w;
	gameHeight = h;
	windowChanged = true;
	glViewport(0, 0, w, h);
	isFullScreen = !isFullScreen;

	LogWindowInfo(false);

}

void Game::SetFramelock(double fl)
{
	fpslock.SetFrameLock(fl);
}

void Game::Run()
{
	SDL_Event e;
	float now;
	ASSERT(isLogSet);

	systemInfo.cpuInfo.processorCount = std::thread::hardware_concurrency();
	
	Initialize();
    if (!CreateTheWindow()) return;
    LoadContent();
	fpslock.SetFrameLock(WindowAttrib.Framelock);
	UpdateTime.ResetTimer();
	RenderTime.ResetTimer();
	fpslock.Reset();

	while (!Quit)
	{

		if (fpslock.TimeToRender())
		{
			perf.Start("GameWindow Render");
			Render(RenderTime.Now().SecondsElapsedf); // .MillisecondsElapsed);
			Present();
			perf.Stop("GameWindow Render");
		}


		Mouse.UpdateWheel(0, 0);
		while (SDL_PollEvent(&e) != 0)
		{

			switch (e.type)
			{
			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_MAXIMIZED:
					int w, h;
					SDL_GetWindowSize(window, &w, &h);
					gameWidth = w;
					gameHeight = h;
					windowChanged = true;
					glViewport(0, 0, w, h);
					break;
				}
				break;
			case SDL_QUIT:
				Quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			{
				Mouse.UpdateButtonEvent(&e.button);
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				Mouse.UpdateWheel(e.wheel.x, e.wheel.y);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				Mouse.UpdateMotionEvent(&e.motion);
				break;
			}
			case SDL_TEXTINPUT:
			{
				Keyboard.AppendInputText(e.text.text);
				break;
			}
			case SDL_KEYDOWN:
			{
				if (Keyboard.TextInputIsOn())
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_BACKSPACE:
					{
						Keyboard.BackspaceInputText();
						break;
					}
					case SDLK_RETURN:
					{
						Keyboard.ReturnInputText();
						break;
					}
					}
					break;
				}
			}

			}
		}

		now = UpdateTime.Now().SecondsElapsedf;//.MillisecondsElapsed;
		perf.Start("GameWindow Update");
		Update(now > MAX_FRAME_TIME ? MAX_FRAME_TIME : now);
		perf.Stop("GameWindow Update");
	}
	Shutdown();
}