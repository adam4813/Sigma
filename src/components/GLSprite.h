#pragma once
#ifndef GLSPRITE_H
#define GLSPRITE_H

#include "../systems/GLSLShader.h"
#include "../IGLComponent.h"

namespace Sigma{

    class GLSprite : public IGLComponent {
    public:
        using IGLComponent::LoadShader;

        SET_COMPONENT_ID("GLSprite");
        // We have a private ctor so the factory method must be used.
        GLSprite(const int entityID = 0);

		/**
		 * \brief Initializes the sprite.
		 *
		 * Registers with the current OpenGL context, allocates space, etc..
		 */
        void InitializeBuffers();

        virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

        unsigned int LoadTexture();
        GLuint GetTexture() const { return texture_; }

        // Load the default shader, "shaders/vert"
        void LoadShader();
        static const std::string DEFAULT_SHADER;

        /**
         * \brief Returns the number of elements to draw for this component.
         *
         *  Because this is a component with only one part, only the 0th group has any elements
         * \param Group the index of the mesh group to count
         * \return unsigned int The number of elements to draw.
         */
        unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const {
            if (group > 0) return 0;
            return 6;
        }
    private:
        GLuint texture_;
    }; // class GLSprite

} // namespace Sigma

#endif // GLSPRITE_H
