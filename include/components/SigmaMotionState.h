#ifndef SIGMAMOTIONSTATE_INCLUDED
#define SIGMAMOTIONSTATE_INCLUDED

//#define BT_USE_DOUBLE_PRECISION
#include "bullet/btBulletDynamicsCommon.h"
#include "components/WorldPosition.h"
#include "components/WorldOrientation.h"
#include <cmath>

namespace Sigma {
    class SigmaMotionState : public btMotionState {
    public:
        SigmaMotionState(const id_t id, WorldPosition& wp, WorldOrientation& wo)
        : id(id), wp(&wp), wo(&wo) {};

        virtual ~SigmaMotionState() {};

        virtual void getWorldTransform(btTransform &worldTrans) const {
            worldTrans.setIdentity();
            worldTrans.setOrigin(btVector3(wp->x(id), wp->y(id), wp->z(id)));
            orientation_type o = wo->euler(id);
            worldTrans.getBasis().setEulerZYX(o.alpha, o.beta, o.gamma);
        };

        virtual void setWorldTransform(const btTransform &worldTrans);

    private:
        const id_t id;
        WorldPosition* wp;
        WorldOrientation* wo;
    };
}


#endif // SIGMAMOTIONSTATE_INCLUDED
