// INCLUDES ////////////////////////////////////////////////////////////////////

#ifdef __linux__
#include <GL/glew.h>
#else
#include <GL/glew.h>
#endif


#include "GameSpriteBatch.h"







// METHODS /////////////////////////////////////////////////////////////////////

GameSpriteBatch::GameSpriteBatch()
{
	lastTextureId = 0;
	vertexId = 0;
	indexId = 0;
	vertexBufferUsed = 0;
	setUp = false;

	clientVertex = NULL;
    clientVertex=new Vertex[vertexBufferCount];


	oMat[0][0] = 1.0f;
	oMat[1][1] = 1.0f;
	oMat[2][2] = 1.0f;
	oMat[3][3] = 1.0f;

}


GameSpriteBatch::~GameSpriteBatch()
{
	// Free client mem
    if (clientVertex!=NULL)
    {
        delete[] clientVertex;
        clientVertex = NULL;
    }

	// Free video mem
	if (vertexId)
	{
		glDeleteBuffers(1, &vertexId);
	}
	if (indexId)
	{
		glDeleteBuffers(1, &indexId);
	}

}

void GameSpriteBatch::Enable2D()
{
   float Right = (float)renderViewPort.right;
   float Left = 0.0;
   float Top = 0.0f;
   float Bottom = (float)renderViewPort.bottom;
   float Near = -1.0f;
   float Far = 1.0f;
   
   oMat[0][0] = 2.0f / (Right - Left);

   oMat[1][1] = 2.0f / (Top - Bottom);
   oMat[2][2] = 2.0f / (Near - Far);

   oMat[3][0] = (Left + Right) / (Left - Right);
   oMat[3][1] = (Bottom + Top) / (Bottom - Top);

   oMat[3][2] = (Far + Near) / (Near - Far);
}

void GameSpriteBatch::Disable2D()
{
}

void GameSpriteBatch::Setup(Recti RenderViewPort)
{
	lastTextureId = 0;
	vertexId = 0;
	indexId = 0;
	vertexBufferUsed = 0;
	GLsizei indexBufferUsed = 0;

	if (setUp)
	{
		glDeleteBuffers(1,&vertexId);
		glDeleteBuffers(1,&indexId);
	}
	
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Store the actual size of rendering area
	renderViewPort = RenderViewPort;

	// Create an empty vertex buffer
	glGenBuffers(1,&vertexId);
	glBindBuffer(GL_ARRAY_BUFFER,vertexId);
	glBufferData(GL_ARRAY_BUFFER,
					vertexBufferSize,
					NULL,
					GL_DYNAMIC_COPY);

	// Indices will be the exact same every time.
	// Create the buffer and fill it.
	// Free memory afterwards.
	glGenBuffers(1,&indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexId);

	GLushort *clientIndex = NULL;
	clientIndex = new GLushort[indexBufferCount];

	// Add the indices to the array
	for (int i = 0; i < vertexBufferCount; i+=4)
	{

		clientIndex[indexBufferUsed] = i;
		indexBufferUsed++;
		clientIndex[indexBufferUsed] = i+1;
		indexBufferUsed++;
		clientIndex[indexBufferUsed] = i+2;
		indexBufferUsed++;
		clientIndex[indexBufferUsed] = i+2;
		indexBufferUsed++;
		clientIndex[indexBufferUsed] = i+1;
		indexBufferUsed++;
		clientIndex[indexBufferUsed] = i+3;
		indexBufferUsed++;
	}

	// Send to the card
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferSize,
		clientIndex,
		GL_DYNAMIC_COPY);

	// Free client memory
	if (clientIndex != NULL)
	{
		delete[] clientIndex;
		clientIndex = NULL;
	}

	// Load the shader
	std::string str;
	if (!shader.loaded)
	{
		str = shader.Load("Content/SpriteBatch_vert.shader", "Content/SpriteBatch_frag.shader");
		
	}

	texture_location = glGetUniformLocation(shader.Id(), "tex");
	ortho_location = glGetUniformLocation(shader.Id(), "ortho");




	setUp = true;
}

void GameSpriteBatch::Begin()
{
	vertexBufferUsed = 0;
	lastTextureId = 0;

	// needs to be fixed send matrix to shader
	Enable2D();
	shader.Bind();
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,vertexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexId);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),
		(void*)(2*sizeof(GL_FLOAT))            // array buffer offset
		);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),
		(const GLvoid*)(4*sizeof(GL_FLOAT))
		);

	// Tell shader what texture to use
	//glActiveTexture(GL_TEXTURE0);
	//int texture_location = glGetUniformLocation(shader.Id(),"tex");
	glUniform1i(texture_location,0);
	//int ortho_location = glGetUniformLocation(shader.Id(), "ortho");
	glUniformMatrix4fv(ortho_location, 1, GL_FALSE, &oMat[0][0]);

}

void GameSpriteBatch::Render()
{

	// kills amd performance
	//glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STREAM_DRAW);


	// old way, tiny, tiny bit slower
	//GLvoid* buff = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// GOES MUCH FASTER with invalidate buffer bit
	GLvoid* buff = glMapBufferRange(GL_ARRAY_BUFFER, 0, vertexBufferUsed * sizeof(Vertex), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	memcpy(buff, clientVertex, vertexBufferUsed*sizeof(Vertex));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	
	glBindTexture(GL_TEXTURE_2D, lastTextureId);

	// Draw it
	glDrawRangeElements(GL_TRIANGLES,
		0,
		vertexBufferUsed,
		vertexBufferUsed+(vertexBufferUsed>>1),
		GL_UNSIGNED_SHORT,
		0);

	//currentPBO = 1 - currentPBO;
}

void GameSpriteBatch::End()
{
	// Final Draw
	if (vertexBufferUsed > 0) Render();

	Disable2D();

	// Bind no texture
	glBindTexture(GL_TEXTURE_2D,0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	// Turn off shader
	shader.UnBind();
	return;

}

// The simplest Draw method
void GameSpriteBatch::Draw(GameTexture2D tex, int x, int y, Color color)
{
	Draw(tex,
		Recti(x,y,tex.width,tex.height),
		Recti(0,0,tex.width,tex.height),
		color);
}

void GameSpriteBatch::Draw(GameTexture2D tex, Recti dest, Recti src, Color color)
{
	if ((tex.bind != lastTextureId))
	{
		if (vertexBufferUsed > 0) Render();
		vertexBufferUsed = 0;
		lastTextureId = tex.bind;

	}

	if (vertexBufferUsed + 4 >= vertexBufferCount)
	{
		Render();
		vertexBufferUsed = 0;
	}

	// Destination variables
	dWidth = (float)dest.right;
	dHeight = (float)dest.bottom;
	x = (float)dest.left;
	y = (float)dest.top;

	// Color variables
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;

	// Source variables
	widthDiv = tex.widthDiv;
	heightDiv = tex.heightDiv;
	tw = widthDiv*0.375f;
	th = heightDiv*0.375f;

	//sX = src.left * widthDiv;// + 0.3750f/32.0f;
	//sY = ((src.top) * heightDiv);// + 0.3750f/64.0f);
	//sWidth = (src.right) * widthDiv;
	//sHeight = (src.bottom) * heightDiv;
	sX = (src.left) * widthDiv;// +tw;// widthDiv*0.375f;					// ditto  changed 0.3 to 0.1
	sY = (src.top) * heightDiv;// +th;// heightDiv*0.375f;              // added -0.375f to fix pixel mode // this breaks fake fullscreen tilemap
	sWidth = (src.right) * widthDiv;// -tw;// widthDiv*0.375f;;			// ditto, seems crisper
	sHeight = (src.bottom) * heightDiv;// -th;// heightDiv*0.375f;	// added +0.375f to fix pixel mode

	// Add the vertices to the array
	// TL
	Vertex * temp = &clientVertex[vertexBufferUsed];
	temp->x = x;
	temp->y= y;
	temp->u= sX;
	temp->v= sY;
	temp->r= r;
	temp->g= g;
	temp->b= b;
	temp->a= a;
	vertexBufferUsed++;

	//clientVertex[vertexBufferUsed].x = x;
	//clientVertex[vertexBufferUsed].y = y;
	//clientVertex[vertexBufferUsed].u = sX;
	//clientVertex[vertexBufferUsed].v = sY;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	temp++;// = &clientVertex[vertexBufferUsed];
	temp->x = dWidth;
	temp->y = y;
	temp->u = sWidth;
	temp->v = sY;
	temp->r = r;
	temp->g = g;
	temp->b = b;
	temp->a = a;
	vertexBufferUsed++;
	// TR
	//clientVertex[vertexBufferUsed].x = dWidth;
	//clientVertex[vertexBufferUsed].y = y;
	//clientVertex[vertexBufferUsed].u = sWidth;
	//clientVertex[vertexBufferUsed].v = sY;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	temp++;// = &clientVertex[vertexBufferUsed];
	temp->x = x;
	temp->y = dHeight;
	temp->u = sX;
	temp->v = sHeight;
	temp->r = r;
	temp->g = g;
	temp->b = b;
	temp->a = a;
	vertexBufferUsed++;
	// BL
	//clientVertex[vertexBufferUsed].x = x;
	//clientVertex[vertexBufferUsed].y = dHeight;
	//clientVertex[vertexBufferUsed].u = sX;
	//clientVertex[vertexBufferUsed].v = sHeight;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	temp++;// = &clientVertex[vertexBufferUsed];
	temp->x = dWidth;
	temp->y = dHeight;
	temp->u = sWidth;
	temp->v = sHeight;
	temp->r = r;
	temp->g = g;
	temp->b = b;
	temp->a = a;
	vertexBufferUsed++;
	////BR
	//clientVertex[vertexBufferUsed].x = dWidth;
	//clientVertex[vertexBufferUsed].y = dHeight;
	//clientVertex[vertexBufferUsed].u = sWidth;
	//clientVertex[vertexBufferUsed].v = sHeight;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;
}

void GameSpriteBatch::Draw(GameTexture2D tex, Point2i dest, Color color)
{
	Draw(tex,
		Recti(dest.x,dest.y,tex.width,tex.height),
		Recti(0,0,tex.width,tex.height),
		color);
}


void GameSpriteBatch::DrawString(GameSpriteFont font, std::string Str, int x, int y, Color color)
{
		int CurX = x;
		int CurY = y;
		int Width, Height;
		Recti src, dest;
		short ch;

		for( unsigned int i = 0; i < Str.size(); ++i )
		{
			ch = Str[i];
			Width = font.set.chars[ch].width;
			Height = font.set.chars[ch].height;

			src.left = font.set.chars[ch].x;
			src.top =  font.set.chars[ch].y;
			src.right = src.left+Width;
			src.bottom = (src.top + Height);

			dest.left = CurX + font.set.chars[ch].xOffset;
			dest.top = CurY + font.set.chars[ch].yOffset;
			dest.right = Width + dest.left;
			dest.bottom = Height + dest.top;

			Draw(font.Texture(),dest,src,color);

			CurX += font.set.chars[ch].xAdvance;
		}
}

void GameSpriteBatch::Draw(GameTexture2D tex, Recti dest, Recti src, Point2i origin, Color color)
{
	dest.left -= origin.x;
	dest.top -= origin.y;
	dest.right -= origin.x;
	dest.bottom -= origin.y;
	Draw(tex,dest,src,color);
}

Vector2f GameSpriteBatch::RotatePoint(Vector2f toRot, Point2i around, const float &cosr, const float &sinr)
{
	//float x,y;
	Vector2f ret;

	// Translate to origin
	toRot.x = (float)(toRot.x - around.x);
	toRot.y = (float)(toRot.y - around.y);
	//x = (float)(toRot.x -around.x);
	//y = (float)(toRot.y -around.y);

	// Rotate
	ret.x = (toRot.x * cosr - toRot.y * sinr);
	ret.y = (toRot.x * sinr + toRot.y * cosr);

	// Translate back
	ret.x = ret.y + around.y;
	ret.y = ret.y + around.y;
	//ret.x += around.x;
	//ret.y += around.y;

	//return ret;
	return ret;
}

void GameSpriteBatch::Draw(GameTexture2D tex, Recti dest, Recti src, Point2i origin, float rotation, Color color)
{
	float cosr = cosf(rotation);
	float sinr = sinf(rotation);
	Vector2f tl,tr,bl,br;
	Point2i around;

	around.x = dest.left + origin.x;
	around.y = dest.top + origin.y;

	tl = RotatePoint(Vector2f(float(dest.left), float(dest.top)),
					around,
					cosr,sinr);
	br = RotatePoint(Vector2f(float(dest.right), float(dest.bottom)),
					around,
					cosr,sinr);
	tr = RotatePoint(Vector2f(float(dest.right), float(dest.top)),
					around,
					cosr,sinr);
	bl = RotatePoint(Vector2f(float(dest.left), float(dest.bottom)),
					around,
					cosr,sinr);

	if ((tex.bind != lastTextureId))
	{
		if (vertexBufferUsed > 0) Render();
		vertexBufferUsed = 0;
		lastTextureId = tex.bind;
		glBindTexture(GL_TEXTURE_2D, lastTextureId);
	}

	if (vertexBufferUsed+4 >= vertexBufferCount)
	{
		Render();
		vertexBufferUsed = 0;
	}

	// Color variables
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;

	// Source variables
	float widthDiv = 1.0f /  (float)tex.width;
	float heightDiv = 1.0f / (float)tex.height;

	sX = src.left * widthDiv + widthDiv*0.375f;
	sY = src.top * heightDiv + heightDiv *0.375f;
	sWidth = src.right * widthDiv - widthDiv*0.375f;;
	sHeight = src.bottom * heightDiv - heightDiv *0.375f;


	// Test
	// Add the vertices to the array
	// TL
	Vertex * temp = &clientVertex[vertexBufferUsed];
	temp->x = tl.x;
	temp->y = tl.y;
	temp->u = sX;
	temp->v = sY;
	temp->r = r;
	temp->g = g;
	temp->b = b;
	temp->a = a;
	vertexBufferUsed++;

	//clientVertex[vertexBufferUsed].x = x;
	//clientVertex[vertexBufferUsed].y = y;
	//clientVertex[vertexBufferUsed].u = sX;
	//clientVertex[vertexBufferUsed].v = sY;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	
	temp++;// = sizeof(float) * 8;// &clientVertex[vertexBufferUsed];
	temp->x = tr.x;// dWidth;
	temp->y = tr.y;// y;
	temp->u = sWidth;
	temp->v = sY;
	temp->r = r;
	temp->g = g;
	temp->b = b;
	temp->a = a;
	vertexBufferUsed++;
	// TR
	//clientVertex[vertexBufferUsed].x = dWidth;
	//clientVertex[vertexBufferUsed].y = y;
	//clientVertex[vertexBufferUsed].u = sWidth;
	//clientVertex[vertexBufferUsed].v = sY;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	temp++;// = &clientVertex[vertexBufferUsed];
	temp->x = bl.x;// tl.x;
	temp->y = bl.y;// dHeight;
	temp->u = sX;
	temp->v = sHeight;
	temp->r = r;
	temp->g = g;
	temp->b = b;
	temp->a = a;
	vertexBufferUsed++;
	// BL
	//clientVertex[vertexBufferUsed].x = x;
	//clientVertex[vertexBufferUsed].y = dHeight;
	//clientVertex[vertexBufferUsed].u = sX;
	//clientVertex[vertexBufferUsed].v = sHeight;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	temp++;// = &clientVertex[vertexBufferUsed];
	temp->x = br.x;// dWidth;
	temp->y = br.y;// dHeight;
	temp->u = sWidth;
	temp->v = sHeight;
	temp->r = r;
	temp->g = g;
	temp->b = b;
	temp->a = a;
	vertexBufferUsed++;
	////BR
	//clientVertex[vertexBufferUsed].x = dWidth;
	//clientVertex[vertexBufferUsed].y = dHeight;
	//clientVertex[vertexBufferUsed].u = sWidth;
	//clientVertex[vertexBufferUsed].v = sHeight;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;


	//// Add the vertices to the array
	//// TL
	//clientVertex[vertexBufferUsed].x = tl.x;
	//clientVertex[vertexBufferUsed].y = tl.y;
	//clientVertex[vertexBufferUsed].u = sX;
	//clientVertex[vertexBufferUsed].v = sY;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	//// TR
	//clientVertex[vertexBufferUsed].x = tr.x;
	//clientVertex[vertexBufferUsed].y = tr.y;
	//clientVertex[vertexBufferUsed].u = sWidth;
	//clientVertex[vertexBufferUsed].v = sY;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	//// BL
	//clientVertex[vertexBufferUsed].x = bl.x;
	//clientVertex[vertexBufferUsed].y = bl.y;
	//clientVertex[vertexBufferUsed].u = sX;
	//clientVertex[vertexBufferUsed].v = sHeight;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;

	////BR
	//clientVertex[vertexBufferUsed].x = br.x;
	//clientVertex[vertexBufferUsed].y = br.y;
	//clientVertex[vertexBufferUsed].u = sWidth;
	//clientVertex[vertexBufferUsed].v = sHeight;
	//clientVertex[vertexBufferUsed].r = r;
	//clientVertex[vertexBufferUsed].g = g;
	//clientVertex[vertexBufferUsed].b = b;
	//clientVertex[vertexBufferUsed].a = a;
	//vertexBufferUsed++;
}
