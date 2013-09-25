#pragma once

#include "GL/glew.h"
#include "IComponent.h"
#include "GLTransform.h"

namespace Sigma {
	// A struct to store which index each of its verts are.
	struct Face {
		Face(unsigned short v1, unsigned short v2, unsigned short v3) : v1(v1), v2(v2), v3(v3) { }
		unsigned short v1, v2, v3;
	};

	// A struct to store the location for each vertex.
	struct Vertex {
		Vertex(float x, float y, float z) : x(x), y(y), z(z) { }
		float x,y,z;
	};

	// A struct to store the color value for each vertex
	struct Color {
		Color(float r, float g, float b) : r(r), g(g), b(b) { }
		float r,g,b;
	};

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
		 * \brief Returns the draw mode for this component.
		 *
		 * \returns unsigned int The draw mode (ex. GL_TRIANGLES, GL_TRIANGLE_STRIP).
		 */
		unsigned int DrawMode() const { return this->drawMode; }

		/**
		 * \brief Renders the component.
		 *
		 * \param[in/out] glm::mediump_float * view The current view matrix
		 * \param[in/out] glm::mediump_float * proj The current project matrix
		 */
		virtual void Render(glm::mediump_float *view, glm::mediump_float *proj)=0;

		/**
		 * \brief Retrieves the transform object for this component.
		 *
		 * \returns GLTransform& The transform object.
		 */
		GLTransform* Transform() { return &transform; }

		/**
		 * \brief Return the VAO ID of this component.
		 *
		 * \returns   unsigned int The VAO ID.
		 */
		unsigned int Vao() const { return this->vao; }

		/**
		 * \brief Sets the face culling mode
		 *
		 */
		virtual void SetCullFace(std::string cull_face) {
			if(cull_face == "back") {
				this->cull_face = GL_BACK;
			}
			else if (cull_face == "front") {
				this->cull_face = GL_FRONT;
			}
			else if (cull_face == "none") {
				this->cull_face = 0;
			}
			else {
				assert(0 && "Invalid cull_face parameter");
			}
		};

		// The index in buffers for each type of buffer.
		int ElemBufIndex;
		int VertBufIndex;
		int UVBufIndex;
		int ColorBufIndex;
		int NormalBufIndex;

	protected:
		unsigned int buffers[10]; // The various buffer IDs.
		unsigned int vao; // The VAO that describes this component's data.
		unsigned int drawMode; // The current draw mode (ex. GL_TRIANGLES, GL_TRIANGLE_STRIP).
		GLuint cull_face; // The current culling method for this component.
		GLTransform transform; // The transform of this component.
	};
}