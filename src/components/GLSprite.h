#pragma once

#include "../systems/GLSLShader.h"
#include "../IGLComponent.h"

class GLSprite : public Sigma::IGLComponent {
public:
    SET_COMPONENT_ID("GLSprite");
	// We have a private ctor so the factory method must be used.
	GLSprite(const int entityID = 0);
	/**
	 * \brief Creates a new GLSprite
	 *
	 * This is the factory method to create a new GLSprite. This method creates all the required buffers and fills them.
	 * \param[in] int entityID The entity this component belongs to
	 * \return   GLSprite* The newly creates GLSprite
	 */
	void InitializeBuffers();
	virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

	unsigned int LoadTexture(std::string filename);
	GLuint GetTexture() { return texture_; }

	// The shader used for rendering GLSprites.
	static void LoadShader();
	static GLSLShader shader;

	/**
	 * \brief Returns the number of elements to draw for this component.
	 *
	 * \return unsigned int The number of elements to draw.
	 */
	unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const {
		if (group > 0) {
			return 0;
		}
		return 6;
	}

	void SetTextureData(const unsigned char* data, unsigned int width, unsigned int height);
private:
	GLuint texture_;
};
