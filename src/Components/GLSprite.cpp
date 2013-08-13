#include "GLSprite.h"
#include "GL/glew.h"

GLSprite::GLSprite( const int entityID /*= 0*/ ) : IComponent(entityID), offsetX(0.0f), offsetY(0.0f) { }

unsigned int GLSprite::VertBuf() const {
	return this->vertBuf;
}

void GLSprite::VertBuf(unsigned int vb) {
	this->vertBuf = vb;
}

unsigned int GLSprite::ColBuf() const {
	return this->colBuf;
}

void GLSprite::ColBuf(unsigned int cb) {
	this->colBuf = cb;
}

unsigned int GLSprite::Vao() const {
	return this->vao;
}

void GLSprite::Vao(unsigned int v) {
	this->vao = v;
}

unsigned int GLSprite::ElemBuf() const {
	return this->elemBuf;
}

void GLSprite::ElemBuf(unsigned int eb) {
	this->elemBuf = eb;
}

float GLSprite::OffsetX() const {
	return this->offsetX;
}

void GLSprite::OffsetX(float x) {
	if (this->offsetX + x > 1.0f) {
		this->offsetX = -1.0f;
	}
	this->offsetX += x;
}

float GLSprite::OffsetY() const {
	return this->offsetY;
}

void GLSprite::OffsetY(float y) {
	if (this->offsetY + y > 1.0f) {
		this->offsetY = -1.0f;
	}
	this->offsetY += y;
}

GLSprite* GLSprite::Factory(int entityID) {
	GLSprite* spr = new GLSprite(entityID);

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
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	GLuint vertbuf;
	glGenBuffers(1, &vertbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vertbuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	GLint posLocation = glGetAttribLocation(GLSprite::shader.GetProgram(), "in_Position");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posLocation);

	GLuint elembuf;
	static const GLushort elem[] = { 0, 1, 2, 3 };
	glGenBuffers(1, &elembuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elembuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elem), elem, GL_STATIC_DRAW);

	GLuint colorbuf;
	glGenBuffers(1, &colorbuf);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
	GLint colLocation = glGetAttribLocation(GLSprite::shader.GetProgram(), "in_Color");
	glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colLocation);
	glBindVertexArray(0);

	spr->Vao(vaoID);
	spr->VertBuf(vertbuf);
	spr->ColBuf(colorbuf);
	spr->ElemBuf(elembuf);

	return spr;
}

void GLSprite::LoadShader() {
	GLSprite::shader.LoadFromFile(GL_VERTEX_SHADER, "..\\..\\shaders\\vert.shade");
	GLSprite::shader.LoadFromFile(GL_FRAGMENT_SHADER, "..\\..\\shaders\\frag.shade");
	GLSprite::shader.CreateAndLinkProgram();
}

GLSLShader GLSprite::shader;
