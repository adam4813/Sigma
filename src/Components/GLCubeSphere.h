#pragma once

#include <vector>

#include "../Systems/GLSLShader.h"
#include "../GLTransform.h"
#include "../IGLComponent.h"

class GLCubeSphere : public IGLComponent {
public:
	// We have a private ctor so the factory method must be used.
	GLCubeSphere(const int entityID = 0);
	/**
	 * \brief Creates a new GLSprite
	 *
	 * This is the factory method to create a new GLIcoSphere. This method creates all the required buffers and fills them.
	 * \param[in] int entityID The entity this component belongs to
	 * \returns   GLIcoSphere* The newly creates GLIcoSphere
	 */
	void Initialize();
	virtual void Update(glm::mediump_float *view, glm::mediump_float *proj);

	// The shader used for rendering GLCubeSphere.
	static void LoadShader();
	static GLSLShader shader;

	/**
	 * \brief Returns the number of elements to draw for this component.
	 *
	 * \returns unsigned int The number of elements to draw.
	 */
	unsigned int NumberElements(const int group = 0) const { 
		if (group > 0) {
			return 0;
		}
		return this->faces.size() * 3;
	}
private:
	void SubDivide(int levels);
	std::vector<face> faces; // The faces for this IcoSphere. Can be used for later refinement.
	std::vector<vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
	std::vector<color> colors;
	std::vector<vertex> vertNorms;

	GLuint _cubeMap;
};
