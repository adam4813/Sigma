#ifndef WORLDOBJECTSSYSTEM_H_INCLUDED
#define WORLDOBJECTSSYSTEM_H_INCLUDED

#include "components/WorldPosition.h"
#include "components/WorldOrientation.h"
#include "components/SigmaMotionState.h"
#include "components/SharedPointerMap.hpp"
#include "components/BitArray.h"
#include <map>

namespace Sigma {
    class WorldObjectSystem {
    public:
        WorldObjectSystem() {};

        virtual ~WorldObjectSystem() {};

        void AddObject(type_id id, const coordinate_type x, const coordinate_type y,
                       const coordinate_type z, const orientation_type& orientation) {
            pphysical.PositionWrite_x(id) = x;
            pphysical.PositionWrite_y(id) = y;
            pphysical.PositionWrite_z(id) = z;
        };

        void AddViewer(type_id id, position_type view_limit) {
            viewDistanceMap.insert({{id, view_limit}});
            viewBitsetMap.SetElement(id, new BitArray());
        };

        SigmaMotionState MotionState(type_id id) {
            return SigmaMotionState(id, pphysical, ophysical);
        };

    private:
        std::unique_ptr<position_array> TransformedPositions(type_id id) {
            return pphysical.RelativeTo(pphysical.x(id), pphysical.y(id), pphysical.z(id));
        };

        WorldPosition pphysical;
        WorldOrientation ophysical;
        SharedPointerMap<type_id,BitArray> viewBitsetMap;
        std::map<type_id, position_type> viewDistanceMap;
    };
}

#endif // WORLDOBJECTSSYSTEM_H_INCLUDED
