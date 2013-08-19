#pragma once

#include "../Systems/GLSLShader.h"
#include "../IGLComponent.h"

class GLSprite : public IGLComponent {
public:
	// We have a private ctor so the factory method must be used.
	GLSprite(const int entityID = 0);
	/**
	 * \brief Creates a new GLSprite
	 *
	 * This is the factory method to create a new GLSprite. This method creates all the required buffers and fills them.
	 * \param[in] int entityID The entity this component belongs to
	 * \returns   GLSprite* The newly creates GLSprite
	 */
	void Initialize();

	// The shader used for rendering GLSprites.
	static void LoadShader();
	static GLSLShader shader;

	/**
	 * \brief Returns the number of elements to draw for this component.
	 *
	 * \returns unsigned int The number of elements to draw.
	 */
	unsigned int NumberElements() const { return 4; }
private:
};
