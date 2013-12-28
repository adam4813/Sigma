#pragma once

#ifndef GL_SCREEN_QUAD
#define GL_SCREEN_QUAD

#include "GLMesh.h"
#include "resources/GLTexture.h"
#include "Sigma.h"

namespace Sigma {

class GLScreenQuad : public GLMesh {
public:
	SET_COMPONENT_TYPENAME("GLScreenQuad");
	GLScreenQuad(const id_t entityID);
	virtual ~GLScreenQuad();

	virtual void InitializeBuffers();
	virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

	// Screen space 0.0 - 1.0
	void SetPosition(float x, float y) { this->x=x; this->y=y; }

	// Screen space 0.0 - 1.0
	void SetSize(float w, float h) { this->w=w; this->h=h; }

	unsigned int GetTexture();

	void SetTexture(resource::GLTexture* texture);

	int NearestPowerOf2(const float width, const float height) const {
		unsigned int power = 0;
		unsigned int dim = 1;

		while(dim < width && dim < height) {
			power += 1;
			dim = 1 << power;
		}

		return dim;
	}

protected:
	resource::GLTexture* texture;
	float x, y, w, h;
	unsigned int texture_size;
};
};

#endif
