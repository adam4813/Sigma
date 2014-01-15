#pragma once

#ifndef SPATIALCOMPONENT_H
#define SPATIALCOMPONENT_H

#include <memory>

#include "IComponent.h"
#include "GLTransform.h"
#include "Sigma.h"
#include "components/PhysicalWorldLocation.h"

namespace Sigma {
	class SpatialComponent : public IComponent {
	public:
		SpatialComponent(const id_t id = 0) : transform(PhysicalWorldLocation::GetTransform(id)), IComponent(id) {
			if (! transform) {
				transform = std::make_shared<GLTransform>();
			}
		}
		virtual ~SpatialComponent() {}

		SET_COMPONENT_TYPENAME("SpatialComponent");

		/**
		 * \brief Retrieves the transform object for this component.
		 *
		 * \return GLTransform& The transform object.
		 */
		GLTransform* Transform() { return transform.get(); }

		void SetTransform(std::shared_ptr<GLTransform> t) {
			this->transform = t;
		}

	protected:
		std::shared_ptr<GLTransform> transform; // The transform of this component.
	};
};

#endif
