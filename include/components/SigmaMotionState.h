#ifndef SIGMAMOTIONSTATE_INCLUDED
#define SIGMAMOTIONSTATE_INCLUDED

#include "bullet/btBulletDynamicsCommon.h"
#include "components/WorldPosition.h"

namespace Sigma {
    class SigmaMotionState : public btMotionState {
    public:
        SigmaMotionState(const type_id id, WorldPosition& wp, WorldOrientation& wo)
        : id(id), wp(&wp), wo(&wo) {};

        virtual ~SigmaMotionState() {};

        virtual void getWorldTransform(btTransform &worldTrans) const {
            worldTrans.setOrigin(btVector3(*wp->x(id).lock(), *wp->y(id).lock(), *wp->z(id).lock()));
            //TODO rotation matrix
            worldTrans.setBasis(btMatrix3x3(0,0,0,0,0,0,0,0,0));
        };

        virtual void setWorldTransform(const btTransform &worldTrans) {
            auto vec = worldTrans.getOrigin();
            wp->PositionWrite_x(id) = vec.x();
            wp->PositionWrite_y(id) = vec.y();
            wp->PositionWrite_z(id) = vec.z();

            // TODO orientation
        };

    private:
        const type_id id;
        WorldPosition* wp;
        WorldOrientation* wo;
    };
}


#endif // SIGMAMOTIONSTATE_INCLUDED
