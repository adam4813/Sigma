#ifndef SIGMAMOTIONSTATE_INCLUDED
#define SIGMAMOTIONSTATE_INCLUDED

//#define BT_USE_DOUBLE_PRECISION
#include "bullet/btBulletDynamicsCommon.h"
#include "components/WorldPosition.h"
#include "VectorMap.hpp"
#include <cmath>

namespace Sigma {
	struct orientation_type;

    class SigmaMotionState : public btMotionState {
    public:
        SigmaMotionState(const id_t id, WorldPosition& wp, VectorMap<id_t, orientation_type>& wo)
        : id(id), wp(&wp), wo(&wo) {};

        virtual ~SigmaMotionState() {};

        virtual void getWorldTransform(btTransform &worldTrans) const;

        virtual void setWorldTransform(const btTransform &worldTrans);

    private:
        const id_t id;
        WorldPosition* wp;
        VectorMap<id_t, orientation_type>* wo;
    };
}


#endif // SIGMAMOTIONSTATE_INCLUDED
