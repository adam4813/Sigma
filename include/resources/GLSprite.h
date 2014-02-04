#pragma once
#ifndef GLSPRITE_H
#define GLSPRITE_H

#include "systems/GLSLShader.h"
#include "components/Renderable.h"
#include "Sigma.h"

namespace Sigma{
	namespace resource {
		class Texture;
	}

	class GLSprite : public Renderable {
	public:
		SET_COMPONENT_TYPENAME("GLSprite");
		// We have a private ctor so the factory method must be used.
		GLSprite(const id_t entityID = 0);

		/**
		 * \brief Initializes the sprite.
		 *
		 * Registers with the current OpenGL context, allocates space, etc..
		 */
		void InitializeBuffers();

		/**
		 * \brief Renders a GLSprite.
		 *
		 *
		 * \param[in] glm::mediump_float * view The current view matrix.
		 * \param[in] glm::mediump_float * proj The current projection matrix.
		 * \exception
		 */
		virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

		/**
		 * \brief Set the Texture resource
		 *
		 * \param[in] resource::Texture * texture
		 * \return    void
		 */
		void SetTexture(std::shared_ptr<resource::Texture> texture);


		// Load the default shader, "shaders/vert"
		void LoadShader();
		static const std::string DEFAULT_SHADER;

		/**
		 * \brief Returns the number of elements to draw for this component.
		 *
		 *  Because this is a component with only one part, only the 0th group has any elements
		 * \param Group the index of the mesh group to count
		 * \return unsigned int The number of elements to draw. Always 6 for a GLSprite.
		 */
		unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const {
			if (group > 0) {
				return 0;
			}
			return 6;
		}
	private:
		std::shared_ptr<resource::Texture> texture;
	}; // class GLSprite

} // namespace Sigma

#endif // GLSPRITE_H
