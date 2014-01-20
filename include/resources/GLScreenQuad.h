#pragma once

#ifndef GL_SCREEN_QUAD
#define GL_SCREEN_QUAD

#include "resources/Mesh.h"
#include "resources/GLTexture.h"
#include "Sigma.h"

namespace Sigma {
	class GLScreenQuad : public Mesh {
	public:;
		DLL_EXPORT GLScreenQuad();
		DLL_EXPORT virtual ~GLScreenQuad();

		virtual void InitializeBuffers();
		//virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

		// Screen space 0.0 - 1.0
		DLL_EXPORT void SetPosition(float x, float y) { this->x=x; this->y=y; }

		// Screen space 0.0 - 1.0
		DLL_EXPORT void SetSize(float w, float h) { this->w=w; this->h=h; }

		DLL_EXPORT unsigned int GetTexture();

		DLL_EXPORT void SetTexture(resource::GLTexture* texture);

		int NearestPowerOf2(const float width, const float height) const {
			unsigned int power = 0;
			unsigned int dim = 1;

			while(dim < width && dim < height) {
				power += 1;
				dim = 1 << power;
			}

			return dim;
		}
		/**
		* \brief Set ti true if the texture is put in the buffer inverted (top to bottom), or false if it has already been flipped (botom to top).
		*
		 * \param[in] bool val Werther the Quad should have inverted UVs or not.
		 * \return void 
		 * \exception  
		 */
		DLL_EXPORT void Inverted(bool val) { inverted = val; }

	protected:
		resource::GLTexture* texture;
		float x, y, w, h;
		unsigned int texture_size;
		bool inverted;
	};
};

#endif
