#include "GLSprite.h"
#include "GL/glew.h"
#include "SOIL/SOIL.h"

GLSprite::GLSprite( const int entityID /*= 0*/ ) : IGLComponent(entityID)  {
	this->drawMode = GL_TRIANGLES;
	this->ElemBufIndex = 2;
	this->ColorBufIndex = 1;
	this->VertBufIndex = 0;
	this->UVBufIndex = 3;
}


void GLSprite::Initialize() {
	LoadTexture();
	static const GLfloat vert[] = {
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f,  -1.0f, 0.0f,
		1.0f,  -1.0f, 0.0f,
	};

	static const GLfloat col[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	};

	static const GLfloat uv[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	// We must create a vao and then store it in our GLSprite.
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->buffers[this->VertBufIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	GLint posLocation = glGetAttribLocation(GLSprite::shader.GetProgram(), "in_Position");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posLocation);

	static const GLushort elem[] = { 0, 1, 2, 1, 3, 2 }; 
	glGenBuffers(1, &this->buffers[this->ElemBufIndex]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elem), elem, GL_STATIC_DRAW);

	glGenBuffers(1, &this->buffers[this->ColorBufIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->ColorBufIndex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
	GLint colLocation = glGetAttribLocation(GLSprite::shader.GetProgram(), "in_Color");
	glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colLocation);

	glGenBuffers(1, &this->buffers[this->UVBufIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->UVBufIndex]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);
	GLint uvlocation = glGetAttribLocation(GLSprite::shader.GetProgram(),"in_UV");
	glVertexAttribPointer(uvlocation, 2, GL_FLOAT, GL_TRUE, 0, NULL);
	glEnableVertexAttribArray(uvlocation);
	glBindVertexArray(0);
}

void GLSprite::LoadShader() {
	GLSprite::shader.LoadFromFile(GL_VERTEX_SHADER, "..\\..\\shaders\\vert.shade");
	GLSprite::shader.LoadFromFile(GL_FRAGMENT_SHADER, "..\\..\\shaders\\frag.shade");
	GLSprite::shader.CreateAndLinkProgram();
}

unsigned int GLSprite::LoadTexture() {

	GLuint textureid = SOIL_load_OGL_texture("test.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	return textureid;
}

GLSLShader GLSprite::shader;
