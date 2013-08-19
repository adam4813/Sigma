#include "GLSprite.h"
#include "GL/glew.h"

GLSprite::GLSprite( const int entityID /*= 0*/ ) : IGLComponent(entityID)  {
	this->drawMode = GL_TRIANGLE_STRIP;
	this->ElemBufIndex = 2;
	this->ColorBufIndex = 1;
	this->VertBufIndex = 0;
}


void GLSprite::Initialize() {

	static const GLfloat vert[] = {
		-5.0f, 0.0f, 5.0f,
		5.0f, 0.0f, 5.0f,
		-5.0f,  0.0f, -5.0f,
		5.0f,  0.0f, -5.0f
	};

	static const GLfloat col[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	};

	// We must create a vao and then store it in our GLSprite.
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->buffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	GLint posLocation = glGetAttribLocation(GLSprite::shader.GetProgram(), "in_Position");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posLocation);

	static const GLushort elem[] = { 0, 1, 2, 3 };
	glGenBuffers(1, &this->buffers[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elem), elem, GL_STATIC_DRAW);

	glGenBuffers(1, &this->buffers[1]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
	GLint colLocation = glGetAttribLocation(GLSprite::shader.GetProgram(), "in_Color");
	glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colLocation);
	glBindVertexArray(0);
}

void GLSprite::LoadShader() {
	GLSprite::shader.LoadFromFile(GL_VERTEX_SHADER, "..\\..\\shaders\\vert.shade");
	GLSprite::shader.LoadFromFile(GL_FRAGMENT_SHADER, "..\\..\\shaders\\frag.shade");
	GLSprite::shader.CreateAndLinkProgram();
}

GLSLShader GLSprite::shader;
