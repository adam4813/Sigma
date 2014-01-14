#ifndef BODY_H_INCLUDED
#define BODY_H_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>
#include "IECSComponentData.h"
#include "IECSComponent.h"

namespace Sigma {
    /** \brief A component for Bullet body
     */
	class Body : public Sigma::IECSComponentData<btRigidBody>, public Sigma::IECSComponent {
	public:
		Body(const WeakPtrWrapper<btRigidBody>& ptr) : IECSComponentData<btRigidBody>(ptr) {};
		virtual ~Body() {};
		SET_COMPONENT_TYPENAME("Body");
		SET_GETTER(btRigidBody);
	};
}


#endif // BODY_H_INCLUDED
