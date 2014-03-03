#pragma once

#ifndef IGLCOMPONENT_H
#define IGLCOMPONENT_H

#ifndef __APPLE__
#include "GL/glew.h"
#endif

#include "components/SpatialComponent.h"
#include <unordered_map>
#include <memory>
#include "Sigma.h"

namespace Sigma {
	namespace resource {
		class Mesh;
		class Shader;
	}

	class Renderable : public SpatialComponent {
	public:
		SET_COMPONENT_TYPENAME("Renderable");

		Renderable();
		Renderable(const id_t entityID); // Ctor that sets the entity ID.

		void SetMesh(std::shared_ptr<resource::Mesh> m);

		//typedef std::unordered_map<std::string, std::shared_ptr<resource::Shader>> ShaderMap;

		/**
		 * \brief Retrieves the specified buffer.
		 *
		 * \param const int index The index of the buffer to retrieve
		 * \return   unsigned int THe ID of the buffer
		 */
		unsigned int GetBuffer(const int index);

		/**
		 * \brief Returns the draw mode for this component.
		 *
		 * \return unsigned int The draw mode (ex. GL_TRIANGLES, GL_TRIANGLE_STRIP).
		 */
		unsigned int DrawMode() const;

		/**
		 * \brief Initializes the Renderable.
		 *
		 * \param entityID The entity this component belongs to.
		 */
		void InitializeBuffers();

		/**
		 * \brief Renders the component.
		 *
		 * \param view The current view matrix
		 * \param proj The current project matrix
		 */
		void Render(glm::mediump_float *view, glm::mediump_float *proj); // function Render

		/**
		 * \brief Return the VAO ID of this component.
		 *
		 * \return   unsigned int The VAO ID.
		 */
		unsigned int Vao() const;

		/**
		 * \brief Sets the face culling mode
		 *
		 */
		virtual void SetCullFace(std::string cull_face);;


		/**
		 * \brief Sets the face culling mode
		 *
		 */
		virtual void SetDepthFunc(std::string depthFunc);;

		std::shared_ptr<resource::Shader> GetShader();
		void SetShader(std::shared_ptr<resource::Shader> s);

		void SetLightingEnabled(bool enabled);
		bool IsLightingEnabled();

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
		GLuint depthFunc;

		std::shared_ptr<resource::Shader> shader; // shaders are shared among components
		std::shared_ptr<resource::Mesh> mesh;

		bool lightingEnabled;
	}; // class Renderable

	namespace reflection {
		template <> inline const char* GetTypeName<Renderable>() { return "Renderable"; }
		template <> inline const unsigned int GetTypeID<Renderable>() { return 2000; }
	}
} // namespace Sigma

#endif // IGLCOMPONENT_H
