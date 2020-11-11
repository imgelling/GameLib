#pragma once

// INCLUDES ////////////////////////////////////////////////////////////////////
#include <string>

#ifdef __linux__
#include <SDL2/SDL_opengl.h>
#else
#include <SDL_opengl.h>
#endif
#include "GameContent_TextureManager.h"
#include "GameContent_SpriteFont.h"
#include "GameContent_Shader.h"




// DEFINES /////////////////////////////////////////////////////////////////////



// DECLARATIONS ////////////////////////////////////////////////////////////////

class GameContent
{
    public:
        GameContent();
        ~GameContent();
		void Clear();
		bool Load(std::string, GameTexture2D &);
		bool Load(std::string, GameSpriteFont &);
		bool Load(std::string, std::string, Shader &, std::string &);
		void UnLoad(GameTexture2D &);
		void UnLoad(GameSpriteFont &);
		void UnLoad(Shader &);
	private:
		TextureManager texMan;
};


