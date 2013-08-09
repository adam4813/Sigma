#include "GLSprite.h"

GLSprite::GLSprite( const int entityID /*= 0*/ ) : IComponent(entityID), offsetX(0.0f), offsetY(0.0f) {

}

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
