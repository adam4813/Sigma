#include "components/SigmaMotionState.h"
#include "composites/PhysicalWorldLocation.h"

namespace Sigma {
		void SigmaMotionState::setWorldTransform(const btTransform &worldTrans) {
            auto vec = worldTrans.getOrigin();
            wp->at(id) = position_type(vec.x(), vec.y(), vec.z());
            float a, b, g;
            worldTrans.getBasis().getEulerZYX(a, b, g);
            wo->at(id) = orientation_type(a, b, g);
            PhysicalWorldLocation::MarkUpdated(id);
        }

		void SigmaMotionState::getWorldTransform(btTransform &worldTrans) const {
            worldTrans.setIdentity();
            auto xyz = wp->at(id).lock();
            worldTrans.setOrigin(btVector3(xyz->x, xyz->y, xyz->z));
            orientation_type o = wo->at(id);
            worldTrans.getBasis().setEulerZYX(o.alpha, o.beta, o.gamma);
        };


}
