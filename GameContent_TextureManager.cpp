// INCLUDES ////////////////////////////////////////////////////////////////////

#ifdef __linux__
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#else

#include <gl\glew.h>
#include <SDL_opengl.h>
#endif

#include "GameContent_TextureManager.h"







// METHODS /////////////////////////////////////////////////////////////////////

TextureManager::TextureManager()
{
    Textures.clear();
}

TextureManager::~TextureManager()
{
	Clear();
}

void TextureManager::Clear()
{
    std::map <std::string, GameTexture2D>:: iterator itr;
    for (itr=Textures.begin(); itr != Textures.end(); itr++)
    {
        GameTexture2D temp = itr->second;
        glDeleteTextures(1,&temp.bind);
    }
    Textures.clear();
}

void TextureManager::UnLoad(GameTexture2D &name)
{
	if (!name.isCopy)
	{
		glDeleteTextures(1, &name.bind);
		Textures.erase(name.name);
	}
	name.bind = -1;
	name.name = "";
	name.height = -1;
	name.width = -1;
	name.heightDiv = -1.0f;
	name.widthDiv = -1.0f;
	name.isCopy = false;
}

bool TextureManager::Load(std::string filename, GameTexture2D &tex, bool mipmaps,
                          unsigned int min, unsigned int mag)
{
	std::string file = filename;
	file += ".png";

	// See if the texture is already loaded
	// and if so return a reference to it
	std::map<std::string, GameTexture2D>:: iterator itr;
    itr = Textures.find(file);
    if (itr != Textures.end())
    {
        GameTexture2D temp = itr->second;
        tex.bind = temp.bind;
        tex.width = temp.width;
        tex.height = temp.height;
        tex.name = temp.name;
		tex.heightDiv = temp.heightDiv;
		tex.widthDiv = temp.widthDiv;
		tex.isCopy = true;
        return true;
    }

	SDL_Surface* surf = NULL;
	surf = IMG_Load(file.c_str());
	if (surf == NULL)
		return false;

	//GLenum texture_format;
	//GLint ncolors;
	//ncolors = surf->format->BytesPerPixel;
	//texture_format = surf->format->format;

    // Not already loaded so we create it
    glGenTextures(1,&tex.bind);
    glBindTexture(GL_TEXTURE_2D,tex.bind);
    tex.name = filename;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Anisotropy
	GLfloat largest_supported_anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
	// When adding gfx options, just div by 2 down to 2x; so 16, 8, 4, 2, 0

	tex.width = surf->w;
	tex.height = surf->h;
	tex.widthDiv = 1.0f / (float)tex.width;
	tex.heightDiv = 1.0f / (float)tex.height;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, surf->pixels);
	if (mipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);
	Textures[filename] = tex;

	SDL_FreeSurface(surf);

	// record that we loaded this texture
	Textures[file] = tex;
	return true;
}

bool TextureManager::Load(std::string filename, GameTexture2D &tex)
{
	// Loads best for sprites at this point This does not affect fonts.
	// Might need to be able to pass mip map stuff
	return Load(filename, tex, false, GL_NEAREST, GL_NEAREST);// GL_LINEAR_MIPMAP_LINEAR);
}
