#pragma once

#include "IComponent.h"
#include "GLTransform.h"

class IGLComponent : IComponent {
public:
	IGLComponent() : IComponent(0) { } // Default ctor setting entity ID to 0.
	IGLComponent(const int entityID) : IComponent(entityID) { } // Ctor that sets the entity ID.
	/**
	 * \brief Creates a new IGLComponent.
	 *
	 * \param[in] int entityID The entity this component belongs to.
	 * \returns IGLCompoent* the newly created component.
	 */
	virtual void Initialize() = 0;


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
	virtual unsigned int NumberElements() const = 0;

	/**
	 * \brief Returns the draw type for this component.
	 *
	 * \returns unsigned int The draw type (ex. GL_TRIANGLES, GL_TRIANGLE_STRIP).
	 */
	unsigned int DrawMode() const { return this->drawMode; }

	/**
	 * \brief Retrieves the transform object for this component.
	 *
	 * \returns GLTransform& The transform object.
	 */
	GLTransform& Transform() { return transform; }

	unsigned int Vao() const { return this->vao; }

	int ElemBufIndex;
	int VertBufIndex;
	int UVBufIndex;
	int ColorBufIndex;
protected:
	unsigned int buffers[10];
	unsigned int vao; // The VAO that describes this component's data.
	unsigned int drawMode;
	GLTransform transform;
};