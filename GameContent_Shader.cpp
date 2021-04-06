

#ifdef __linux__
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#else
#include <GL/glew.h>
#include <SDL_opengl.h>
#endif
#include "GameContent_Shader.h"

#include <sstream>



Shader::Shader()
{
	shaderId = 0;
	vertexId = 0;
	fragmentId = 0;
	loaded = false;
}

char * Shader::LoadTextFile(const char *fileName)
{
    char* text = NULL;

    if (fileName != NULL) {
		FILE *file;
		fopen_s(&file,fileName, "rt");

        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            size_t count = ftell(file);
            rewind(file);

            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
				if (text == NULL) return NULL;
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';
            }
            fclose(file);
        }
    }
    return text;
}

std::string Shader::validateShader(unsigned int shader, const char* file) {
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
	std::stringstream err;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        err << "Shader " << shader << " (" << (file?file:"") << ") compile: " << buffer << std::endl;
    }
	return err.str();
}

std::string Shader::validateProgram(unsigned int program) {
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
	std::stringstream err;

    memset(buffer, 0, BUFFER_SIZE);
    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
    if (length > 0)
        err << "Program " << program << " link: " << buffer << std::endl;

    glValidateProgram(program);
    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
        err << "Error validating shader " << program << std::endl;
	return err.str();
}

std::string Shader::Load(std::string vertex, std::string fragment)
{
	std::string ret;
	std::stringstream err;

	if (loaded)
	{
		glDetachShader(shaderId, fragmentId);
		glDetachShader(shaderId, vertexId);

		glDeleteShader(fragmentId);
		glDeleteShader(vertexId);
		glDeleteProgram(shaderId);
		loaded = false;
	}

	vertexId = glCreateShader(GL_VERTEX_SHADER);
	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexCode = LoadTextFile(vertex.c_str());
	std::string fragmentCode = LoadTextFile(fragment.c_str());

	if (vertexCode.length() == 0)
	{
		ret = "Vertex Shader not found.";
		return ret;
	}
	if (fragmentCode.length() == 0)
	{
		ret = "Fragment Shader not found.";
		return ret;
	}

	const char *vs = (const char *)vertexCode.c_str();
	const char *fs = (const char *)fragmentCode.c_str();
	glShaderSource(vertexId,1, &vs,0);
	glShaderSource(fragmentId,1, &fs,0);

	glCompileShader(vertexId);
	err << validateShader(vertexId, vertex.c_str());
	glCompileShader(fragmentId);
	err << validateShader(fragmentId, fragment.c_str());

	shaderId = glCreateProgram();
	glAttachShader(shaderId,fragmentId);
	glAttachShader(shaderId,vertexId);
	glLinkProgram(shaderId);
	err << validateProgram(shaderId);

	loaded = true;
	return err.str();
}

Shader::~Shader()
{
	UnLoad();
}

void Shader::UnLoad()
{
	if (loaded)
	{
		glDetachShader(shaderId, fragmentId);
		glDetachShader(shaderId, vertexId);

		glDeleteShader(fragmentId);
		glDeleteShader(vertexId);
		glDeleteProgram(shaderId);
		loaded = false;
	}
}

void Shader::Bind()
{
    glUseProgram(shaderId);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

unsigned int Shader::Id()
{
	return shaderId;
}
