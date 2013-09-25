#pragma once

#include <vector>

#include "../systems/GLSLShader.h"
#include "../GLTransform.h"
#include "../IGLComponent.h"

class GLCubeSphere : public Sigma::IGLComponent {
public:
    SET_COMPONENT_ID("GLCubeSphere");
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
	void LoadShader(std::string shader_name);
	GLSLShader _shader;

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

	/**
	 * \brief Helper method to subdivde the cube recursively
	 *
	 * \params[in] levels - the number of levels of subdivision to apply
	 */
	void SubDivide(int levels);
	
	/**
	 * \brief Loads the cube map texture based on a base texture_name
	 *
	 * \params[in] texture_name - the base name of all 6 cube map textures,
	 *							  i.e. for "texture": texture1, texture2, ... texture6
	 */
	void LoadTexture(std::string texture_name);

	void SetSubdivisions(int levels) { this->_subdivisionLevels = levels; }
	void SetRotationSpeed(float rot_speed) { this->rotation_speed = rot_speed; }
	void SetFixToCamera(bool fix_to_camera) { this->fix_to_camera = fix_to_camera; }
private:

	std::vector<Sigma::Face> faces;			// The faces for this IcoSphere. Can be used for later refinement.
	std::vector<Sigma::Vertex> verts;		// The verts that the faces refers to. Can be used for later refinement.

	GLuint _cubeMap, _cubeNormalMap;
	int _subdivisionLevels;
	float rotation_speed;

	// Special setting for skyboxes
	bool fix_to_camera;
};
