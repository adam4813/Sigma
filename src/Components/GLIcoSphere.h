#pragma once

#include <vector>

#include "../Systems/GLSLShader.h"
#include "../GLTransform.h"
#include "../IGLComponent.h"

class GLIcoSphere : public IGLComponent {
public:
	// We have a private ctor so the factory method must be used.
	GLIcoSphere(const int entityID = 0);
	/**
	 * \brief Creates a new GLSprite
	 *
	 * This is the factory method to create a new GLIcoSphere. This method creates all the required buffers and fills them.
	 * \param[in] int entityID The entity this component belongs to
	 * \returns   GLIcoSphere* The newly creates GLIcoSphere
	 */
	void Initialize();
	virtual void Update(glm::mediump_float *view, glm::mediump_float *proj);
	/**
	 * \brief Refines a given set of faces into 4 smaller faces.
	 *
	 * Given a set of verts and faces, divide each face into 4, append the new verts to the verts array, and add the faces to a temporary array.
	 * Then replace the input array of faces with the new array of faces.
	 * \param[in/out] std::vector<vertex> & verts The verts that will be used as a starting point. Any new verts created will be added to this.
	 * \param[in/out] std::vector<face> & faces The faces to refine. The set of faces will be stored here when complete.
	 * \param[in] int level The number of refinements to apply.
	 * \returns   void 
	 * \exception  
	 */
	void Refine(std::vector<vertex> &verts, std::vector<face> &faces, int level);

	// The shader used for rendering GLIcoSphere.
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
	std::vector<face> faces; // The faces for this IcoSphere. Can be used for later refinement.
	std::vector<vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
	std::vector<color> colors;
	std::vector<vertex> vertNorms;
};
