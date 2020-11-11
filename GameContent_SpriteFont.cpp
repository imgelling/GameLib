// INCLUDES ////////////////////////////////////////////////////////////////////

#include "GameContent_SpriteFont.h"
#include <fstream>
#include <sstream>





// METHODS /////////////////////////////////////////////////////////////////////
GameSpriteFont::GameSpriteFont()
{
	set.base = -1;
}

bool GameSpriteFont::Load(std::string filename, GameTexture2D & text)
{
	texture = text;
	std::string Line;
	std::string Read, Key, Value;
	std::size_t i;
	std::ifstream Stream;

    std::string file = filename + ".fnt";

	Stream.open(file.c_str());
	while( !Stream.eof() )
	{
		std::stringstream LineStream;
		getline( Stream, Line );
		LineStream << Line;

		//read the line's type
		LineStream >> Read;
		if( Read == "common" )
		{
			//this holds common data
			while( !LineStream.eof() )
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr( 0, i );
				Value = Read.substr( i + 1 );

				//assign the correct value
				Converter << Value;
				if( Key == "lineHeight" )
					Converter >> set.lineHeight;
				else if( Key == "base" )
					Converter >> set.base;
				else if( Key == "scaleW" )
					Converter >> set.width;
				else if( Key == "scaleH" )
					Converter >> set.height;
				else if( Key == "pages" )
					Converter >> set.pages;
			}
		}
		else if( Read == "char" )
		{
			//this is data for a specific char
			unsigned short CharID = 0;

			while( !LineStream.eof() )
			{
				if (CharID > 255)
				{
					Stream.close();
					return false;
				}
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr( 0, i );
				Value = Read.substr( i + 1 );

				//assign the correct value
				Converter << Value;
				if( Key == "id" )
					Converter >> CharID;
				else if( Key == "x" )
					Converter >> set.chars[CharID].x;
				else if( Key == "y" )
					Converter >> set.chars[CharID].y;
				else if( Key == "width" )
					Converter >> set.chars[CharID].width;
				else if( Key == "height" )
					Converter >> set.chars[CharID].height;
				else if( Key == "xoffset" )
					Converter >> set.chars[CharID].xOffset;
				else if( Key == "yoffset" )
					Converter >> set.chars[CharID].yOffset;
				else if( Key == "xadvance" )
					Converter >> set.chars[CharID].xAdvance;
				else if( Key == "page" )
					Converter >> set.chars[CharID].page;
			}
		}
	}
	Stream.close();

	return true;
}

void GameSpriteFont::UnLoad()
{
	set.base = -1;
}

unsigned int GameSpriteFont::Length(std::string text)
{
    int length = 0;
    unsigned int ch;
	for(unsigned int i = 0; i < text.size(); i++ )
	{
		ch = text[i];
		length += set.chars[ch].xAdvance;
    }

    return length;
}


