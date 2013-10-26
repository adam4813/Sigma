#pragma once

#ifndef GL_SCREEN_QUAD
#define GL_SCREEN_QUAD

#include "GLMesh.h"

namespace Sigma {
class GLScreenQuad : public GLMesh {
public:
	SET_COMPONENT_ID("GLScreenQuad");
	GLScreenQuad(int entityID);
	virtual ~GLScreenQuad();

	virtual void InitializeBuffers();
	virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

	void SetPosition(float x, float y) { this->x=x; this->y=y; }
	void SetSize(float w, float h) { this->w=w; this->h=h; }

	unsigned int GetTexture() { return this->texture_id; }

	int nearestPowerOf2(const float width, const float height) const {
		unsigned int power = 0;
		unsigned int dim = 1;

		while(dim < width && dim < height) {
			power += 1;
			dim = (int)pow(2, power);
		}

		return dim;
	}

protected:
	unsigned int texture_id;
	float x, y, w, h;
};
};

#endif