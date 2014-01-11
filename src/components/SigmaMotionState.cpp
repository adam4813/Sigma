#include "components/SigmaMotionState.h"
#include "components/PhysicalWorldLocation.h"

namespace Sigma {
		void SigmaMotionState::setWorldTransform(const btTransform &worldTrans) {
            auto vec = worldTrans.getOrigin();
            wp_x->at(id) = vec.x();
            wp_y->at(id) = vec.y();
            wp_z->at(id) = vec.z();
            float a, b, g;
            worldTrans.getBasis().getEulerZYX(a, b, g);
            wo->at(id) = orientation_type(a, b, g);
            PhysicalWorldLocation::MarkUpdated(id);
        }

		void SigmaMotionState::getWorldTransform(btTransform &worldTrans) const {
            worldTrans.setIdentity();
            worldTrans.setOrigin(btVector3(wp_x->at(id), wp_y->at(id), wp_z->at(id)));
            orientation_type o = wo->at(id);
            worldTrans.getBasis().setEulerZYX(o.alpha, o.beta, o.gamma);
        };


}
