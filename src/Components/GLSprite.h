#pragma once

#include <string>
#include <queue>

#include "../IComponent.h"

class GLSprite : public IComponent {
public:
	GLSprite(const int entityID = 0);

	unsigned int VERTEX_BUFFER() const { return pVERTEX_BUFFER; }
	void VERTEX_BUFFER(unsigned int val) { pVERTEX_BUFFER = val; }
	unsigned int COLOUR_BUFFER() const { return pCOLOUR_BUFFER; }
	void COLOUR_BUFFER(unsigned int val) { pCOLOUR_BUFFER = val; }
private:
	unsigned int pVERTEX_BUFFER;    // will point to where GL put the buffer
	unsigned int pCOLOUR_BUFFER;    // will point to where GL put the buffer
};
