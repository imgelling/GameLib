#pragma once
// INCLUDES ////////////////////////////////////////////////////////////////////
#include <string>


// DEFINES /////////////////////////////////////////////////////////////////////


// DECLARATIONS ////////////////////////////////////////////////////////////////
class Shader
{
	friend class GameContent;
	friend class GameSpriteBatch;
	public:
		Shader();
		~Shader();
		void Bind();
		void UnBind();
		void UnLoad();
		unsigned int Id();

	private:
		std::string Load(std::string vertex, std::string fragment);
		std::string validateShader(unsigned int shader, const char* file = 0);
		std::string validateProgram(unsigned int program);
		char *LoadTextFile(const char *fileName);
		unsigned int shaderId;
		unsigned int vertexId;
		unsigned int fragmentId;
		bool loaded;
};