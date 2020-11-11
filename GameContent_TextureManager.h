#pragma once

// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameContent_Common.h"
#include <map>
#include <string>


// DEFINES ////////////////////////////////////////////////////////////////////



// DECLARATIONS ///////////////////////////////////////////////////////////////
class GameTexture2D
{
	friend class TextureManager;
	private:
		ContentType type;
		bool Load(std::string filename, GameTexture2D text) { return true; };
		void UnLoad() { };
		bool isCopy;
	public:
		unsigned int bind;
		int width;
		int height;
		float widthDiv;
		float heightDiv;
		std::string name;
		GameTexture2D() { type = TEXTURE2D; isCopy = false; }
		ContentType Type() { return type; }
};

class TextureManager
{
	friend class GameContent;
    public:
        TextureManager();
        ~TextureManager();
    private:
        bool Load(std::string filename, GameTexture2D &tex);
        bool Load(std::string filename, GameTexture2D &tex, bool mipmaps,
                                  unsigned int min, unsigned int mag);
        void UnLoad(GameTexture2D &name);
		void Clear();
        std::map<std::string,GameTexture2D> Textures;
};
