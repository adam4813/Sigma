#pragma once

#include <vector>

#include "../systems/GLSLShader.h"
#include "../GLTransform.h"
#include "../IGLComponent.h"

class GLCubeSphere : public IGLComponent {
public:
	// We have a private ctor so the factory method must be used.
	GLCubeSphere(const int entityID = 0);
	virtual ~GLCubeSphere();

	/**
	 * \brief Generates new cube and subdivides, loads textures
	 *
	 * This is the factory method to create a new GLCubeSphere. This method creates all the required buffers and fills them.
	 * \param[in] None
	 * \returns   void
	 */
	virtual void InitializeBuffers();
	
	/**
	 * \brief Updates the rotation and renders the mesh
	 *
	 * \param[in] None
	 * \returns   void
	 */
	virtual void Update(glm::mediump_float *view, glm::mediump_float *proj);

	/// The shader used for rendering GLCubeSphere.
	static void LoadShader();
	static GLSLShader shader;

	/**
	 * \brief Returns the number of elements to draw for this component.
	 *
	 * \params[in] Group - the index of the mesh group to count
	 * \returns unsigned int The number of elements to draw.
	 */
	virtual unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const { 
		if (group > 0) {
			return 0;
		}
		return this->faces.size() * 3;
	}
	
	virtual void InitializeBuffers();
private:

	/**
	 * \brief Helper method to subdivde the cube recursively
	 *
	 * \params[in] levels - the number of levels of subdivision to apply
	 */
	void SubDivide(int levels);

	std::vector<Sigma::Face> faces;			// The faces for this IcoSphere. Can be used for later refinement.
	std::vector<Sigma::Vertex> verts;		// The verts that the faces refers to. Can be used for later refinement.

	GLuint _cubeMap, _cubeNormalMap;
};
