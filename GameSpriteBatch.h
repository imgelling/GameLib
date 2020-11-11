// INCLUDES ////////////////////////////////////////////////////////////////////
#pragma once

#ifdef __linux__
#include <SDL2/SDL_opengl.h>
#else
#include <SDL_opengl.h>
#endif
#include "GameContent_SpriteFont.h"
#include "GameContent_Shader.h"
#include "GameClasses.h"


//#include <glm/gtc/matrix_transform.hpp>



// DEFINES /////////////////////////////////////////////////////////////////////


// DECLARATIONS ////////////////////////////////////////////////////////////////
class GameSpriteBatch {
	public:
		GameSpriteBatch();
		~GameSpriteBatch();
		// Need to add states/blend/etc to begin()
		void Setup(Rect RenderViewPort);
		void Begin();
		//void Begin(bool UseDefault);
		void End();

		// These are sprite drawing functions will later implement more
		// to include rotations, scaling, and origin.
		void Draw(GameTexture2D tex, int x, int y, Color color);
        void Draw(GameTexture2D tex, Point dest, Color color);
        void Draw(GameTexture2D tex, Rect dest, Rect src, Color color);
		void Draw(GameTexture2D tex, Rect dest, Rect src, Point origin, Color color);
		void Draw(GameTexture2D tex, Rect dest, Rect src, Point origin, float rotation, Color color);
		void DrawString(GameSpriteFont font, std::string Str, int x, int y, Color color);


	private:
		bool setUp;
		GLuint lastTextureId;
		GLuint vertexId;
		GLuint indexId;
		Shader shader;
		Rect renderViewPort;

		int texture_location;
		int ortho_location;


		// Pre create these as they will be used a lot
		float x,y;
		float r,g,b,a;
		float dWidth;
		float dHeight;
		float sWidth;
		float sHeight;
		float tw, th;
		float sX, sY;
		float widthDiv;
		float heightDiv;

		void Render();
		void Enable2D();
		void Disable2D();
		Vector2f RotatePoint(Vector2f toRot, Point around, const float &cosr, const float &sinr);

		struct Vertex {
			float x,y;
			float u,v;
			float r,g,b,a;
		};

		GLsizei vertexBufferUsed;	// Counter of vertices used
		Vertex *clientVertex;		// Pointer to vertex data
		// Total number of sprites to render
		static const GLsizei vertexBufferCount = 128 * 128 * 2;// *10;
		// Size of buffer
		static const GLsizei vertexBufferSize = 4*sizeof(Vertex)
										* vertexBufferCount;


		// Total number of indices used to render
		static const GLsizei indexBufferCount = vertexBufferCount * 6;
		// Size of buffer
		static const GLsizei indexBufferSize = sizeof(GLushort)
										* indexBufferCount;

		float oMat[4][4] = { 0.0f };
		//glm::mat4 OrthoMat;

		GLuint VertexArrayID;


		//pbo stuff
		//GLuint pbo[2];
		//int currentPBO = 0;
};
