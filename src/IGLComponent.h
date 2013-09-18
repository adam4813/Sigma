#pragma once

#include "IComponent.h"
#include "GLTransform.h"

namespace Sigma {
	// A helper to store which index each of its verts are.
	struct Face {
		Face(unsigned short v1, unsigned short v2, unsigned short v3) : v1(v1), v2(v2), v3(v3) { }
		unsigned short v1, v2, v3;
	};

	// A helper to store the location for each vertex.
	struct Vertex {
		Vertex(float x, float y, float z) : x(x), y(y), z(z) { }
		float x,y,z;
	};

	struct Color {
		Color(float r, float g, float b) : r(r), g(g), b(b) { }
		float r,g,b;
	};
}

class IGLComponent : public IComponent {
public:
	IGLComponent() : IComponent(0) { } // Default ctor setting entity ID to 0.
	IGLComponent(const int entityID) : IComponent(entityID) { } // Ctor that sets the entity ID.
	/**
	 * \brief Creates a new IGLComponent.
	 *
	 * \param[in] int entityID The entity this component belongs to.
	 * \returns IGLCompoent* the newly created component.
	 */
	virtual void InitializeBuffers() = 0;


	/**
	 * \brief Retrieves the specified buffer.
	 *
	 * \param[in] const int index The index of the buffer to retrieve
	 * \returns   unsigned int THe ID of the buffer
	 */
	unsigned int GetBuffer(const int index) {
		return this->buffers[index];
	}

	/**
	 * \brief Returns the number of elements to draw for this component.
	 *
	 * \returns unsigned int The number of elements to draw.
	 */
	virtual unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const = 0;

	/**
	 * \brief Returns the draw type for this component.
	 *
	 * \returns unsigned int The draw type (ex. GL_TRIANGLES, GL_TRIANGLE_STRIP).
	 */
	unsigned int DrawMode() const { return this->drawMode; }

	/**
	 * \brief Update renders the object
	 *
	 */
	virtual void Update(glm::mediump_float *view, glm::mediump_float *proj)=0;

	/**
	 * \brief Retrieves the transform object for this component.
	 *
	 * \returns GLTransform& The transform object.
	 */
	GLTransform* Transform() { return &transform; }

	unsigned int Vao() const { return this->vao; }

	int ElemBufIndex;
	int VertBufIndex;
	int UVBufIndex;
	int ColorBufIndex;
	int NormalBufIndex;

protected:
	unsigned int buffers[10];
	unsigned int vao; // The VAO that describes this component's data.
	unsigned int drawMode;
	GLTransform transform;
};