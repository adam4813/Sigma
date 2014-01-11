#ifndef SIGMAMOTIONSTATE_INCLUDED
#define SIGMAMOTIONSTATE_INCLUDED

//#define BT_USE_DOUBLE_PRECISION
#include "bullet/btBulletDynamicsCommon.h"
#include "VectorMap.hpp"
#include <cmath>

namespace Sigma {
	struct orientation_type;
	typedef double coordinate_type;

    class SigmaMotionState : public btMotionState {
    public:
        SigmaMotionState(const id_t id, VectorMap<id_t, coordinate_type>& wp_x,\
							VectorMap<id_t, coordinate_type>& wp_y,\
							VectorMap<id_t, coordinate_type>& wp_z,\
							VectorMap<id_t, orientation_type>& wo)
        : id(id), wp_x(&wp_x), wp_y(&wp_y), wp_z(&wp_z), wo(&wo) {};

        virtual ~SigmaMotionState() {};

        virtual void getWorldTransform(btTransform &worldTrans) const;

        virtual void setWorldTransform(const btTransform &worldTrans);

    private:
        const id_t id;
        VectorMap<id_t, coordinate_type>* wp_x;
        VectorMap<id_t, coordinate_type>* wp_y;
        VectorMap<id_t, coordinate_type>* wp_z;
        VectorMap<id_t, orientation_type>* wo;
    };
}


#endif // SIGMAMOTIONSTATE_INCLUDED
