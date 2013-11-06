#pragma once
#ifndef GLSPRITE_H
#define GLSPRITE_H

#include "../systems/GLSLShader.h"
#include "../IGLComponent.h"

namespace Sigma{
	namespace resource {
		class GLTexture;
	}

    class GLSprite : public IGLComponent {
    public:
        SET_COMPONENT_TYPENAME("GLSprite");
        // We have a private ctor so the factory method must be used.
        GLSprite(const int entityID = 0);

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
         * \param[in/out] glm::mediump_float * view The current view matrix.
         * \param[in/out] glm::mediump_float * proj The current projection matrix.
         * \exception  
         */
        virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

		/**
		 * \brief Set the GLTexture resource
		 *
		 * \param[in/out] Sigma::resource::GLTexture * texture
		 * \return    void 
		 */
		void SetTexture(Sigma::resource::GLTexture* texture);


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
            return 6;
        }
    private:
		Sigma::resource::GLTexture* texture;
    }; // class GLSprite

} // namespace Sigma

#endif // GLSPRITE_H
