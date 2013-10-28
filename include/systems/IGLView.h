#pragma once
#ifndef IGL_VIEW_H
#define IGL_VIEW_H

//#include "glm/glm.hpp"
//#include "glm/ext.hpp"

#include "GLTransform.h"
#include "IComponent.h"

namespace Sigma{

    struct IGLView : public Sigma::IComponent {

        IGLView(int entityID) : IComponent(entityID) {}

        GLTransform transform;

        virtual const glm::mat4 GetViewMatrix() = 0;
    }; // stuct IGLView
} // namespace Sigma

#endif // IGL_VIEW_H
