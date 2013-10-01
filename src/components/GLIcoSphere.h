#pragma once

#include <vector>
#include "GLMesh.h"

#include "../systems/GLSLShader.h"

struct IcoScphereFace {
	int vertexIndex[3];
	Sigma::Color colors[3];
};

class GLIcoSphere : public GLMesh {
public:
    SET_COMPONENT_ID("GLIcoSphere");
	// We have a private ctor so the factory method must be used.
	GLIcoSphere(const int entityID = 0);
	/**
	 * \brief Creates a new GLSprite
	 *
	 * This is the factory method to create a new GLIcoSphere. This method creates all the required buffers and fills them.
	 * \param[in] int entityID The entity this component belongs to
	 * \returns   GLIcoSphere* The newly creates GLIcoSphere
	 */
	void InitializeBuffers();
	virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

	/**
	 * \brief Refines a given set of faces into 4 smaller faces.
	 *
	 * Given a set of verts and faces, divide each face into 4, append the new verts to the verts array, and add the faces to a temporary array.
	 * Then replace the input array of faces with the new array of faces.
	 * \param[in/out] std::vector<Vertex> & verts The verts that will be used as a starting point. Any new verts created will be added to this.
	 * \param[in/out] std::vector<Face> & faces The faces to refine. The set of faces will be stored here when complete.
	 * \param[in] int level The number of refinements to apply.
	 * \returns   void
	 * \exception
	 */
	void Refine(std::vector<Sigma::Vertex> &verts, std::vector<Sigma::Face> &faces, int level);

	void RefineFace(const unsigned int index);

	// The shader used for rendering GLIcoSphere.
	static void LoadShader();
	static GLSLShader shader;
private:
	std::vector<Sigma::Vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
	std::vector<IcoScphereFace> icofaces;
};
