#pragma once

#include <vector>

#include "../Systems/GLSLShader.h"
#include "../GLTransform.h"
#include "../IGLComponent.h"

// A helper to store which index each of its verts are.
struct face {
	face(unsigned short v1, unsigned short v2, unsigned short v3) : v1(v1), v2(v2), v3(v3) { }
	unsigned short v1, v2, v3;
};

// A helper to store the location for each vertex.
struct vertex {
	vertex(float x, float y, float z) : x(x), y(y), z(z) { }
	float x,y,z;
};
struct color {
	color(float r, float g, float b) : r(r), g(g), b(b) { }
	float r,g,b;
};

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
	unsigned int NumberElements() const { return this->faces.size() * 3; }
private:
	std::vector<face> faces; // The faces for this IcoSphere. Can be used for later refinement.
	std::vector<vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
	std::vector<color> colors;
};
