// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameContent.h"





// METHODS ////////////////////////////////////////////////////////////////////
void GameContent::Clear()
{
	texMan.Clear();
}

GameContent::GameContent()
{
}

GameContent::~GameContent()
{
	Clear();
}

bool GameContent::Load(std::string filename, GameTexture2D &content)
{
	 return texMan.Load(filename, content);
}

bool GameContent::Load(std::string filename, GameSpriteFont &content)
{
	bool good;
	GameTexture2D text;
	// Fixes text burring
	//good = texMan.Load(filename, text, false, GL_NEAREST, GL_NEAREST);//NEAREST,GL_NEAREST);
	// Looks better stretched.
	good = texMan.Load(filename,text,true,GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);//NEAREST,GL_NEAREST);

	if (good) 
		good = content.Load(filename,text);
	return good;
}

bool GameContent::Load(std::string vertex, std::string fragment, Shader &shader, std::string &info)
{
	info = shader.Load(vertex,fragment);
	size_t found = info.find("error(#");
	if (found!=std::string::npos)  
		return false;

	return true;
}

void GameContent::UnLoad(GameTexture2D &content)
{
	texMan.UnLoad(content); 
	return;
}

void GameContent::UnLoad(GameSpriteFont &content)
{
	texMan.UnLoad(content.texture); 
	content.UnLoad(); 
	return;	
}

void GameContent::UnLoad(Shader &shader)
{
	shader.UnLoad();
}