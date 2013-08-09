#pragma once

#include <string>
#include <queue>

#include "../IComponent.h"

class GLSprite : public IComponent {
public:
	GLSprite(const int entityID = 0);

	unsigned int VertBuf() const;
	void VertBuf(unsigned int val);
	unsigned int ColBuf() const;
	void ColBuf(unsigned int val);
	unsigned int Vao() const;
	void Vao(unsigned int val);
	unsigned int ElemBuf() const;
	void ElemBuf(unsigned int val);
	void OffsetX(float x);
	float OffsetX() const;
	void OffsetY(float y);
	float OffsetY() const;
private:
	unsigned int vertBuf;    // will point to where GL put the buffer
	unsigned int colBuf;    // will point to where GL put the buffer
	unsigned int elemBuf;
	unsigned int vao;
	float offsetX, offsetY;
};
