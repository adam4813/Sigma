#pragma once

#include <string>
#include <queue>

#include "../IComponent.h"

class GLSprite : public IComponent {
public:
	GLSprite(const int entityID = 0);

	unsigned int VertBuf() const { return vertBuf; }
	void VertBuf(unsigned int val) { vertBuf = val; }
	unsigned int ColBuf() const { return colBuf; }
	void ColBuf(unsigned int val) { colBuf = val; }
	unsigned int Vao() const { return vao; }
	void Vao(unsigned int val) { vao = val; }
private:
	unsigned int vertBuf;    // will point to where GL put the buffer
	unsigned int colBuf;    // will point to where GL put the buffer
	unsigned int vao;
};
