#pragma once

#ifndef SPATIALCOMPONENT_H
#define SPATIALCOMPONENT_H

#include "IComponent.h"
#include "GLTransform.h"
#include "Sigma.h"

namespace Sigma {
	class SpatialComponent : public IComponent {
	public:
		SpatialComponent(const id_t id = 0) : IComponent(id) {}
		virtual ~SpatialComponent() {}

		SET_COMPONENT_TYPENAME("SpatialComponent");

		/**
		 * \brief Retrieves the transform object for this component.
		 *
		 * \return GLTransform& The transform object.
		 */
		GLTransform* Transform() { return &transform; }

	protected:
		GLTransform transform; // The transform of this component.
	};
};

#endif
