#ifndef ORIENTATION_H_INCLUDED
#define ORIENTATION_H_INCLUDED

#include "IECSComponentData.h"
#include "IECSComponent.h"

namespace Sigma {
    /** \brief A component for orientation
     */
	class Orientation : public Sigma::IECSComponentData<orientation_type>, public Sigma::IECSComponent {
	public:
		Orientation(const WeakPtrWrapper<orientation_type>& ptr) : IECSComponentData<orientation_type>(ptr) {};
		virtual ~Orientation() {};
		SET_COMPONENT_TYPENAME("Orientation");
		SET_GETTER(orientation_type);
	};
}

#endif // ORIENTATION_H_INCLUDED
