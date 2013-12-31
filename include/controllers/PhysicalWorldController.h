#ifndef PHYSICALWORLDCOMPONENT_H_INCLUDED
#define PHYSICALWORLDCOMPONENT_H_INCLUDED

#include "components/WorldPosition.h"
#include "components/WorldOrientation.h"
#include "components/SigmaMotionState.h"
#include "SharedPointerMap.hpp"
#include "BitArray.hpp"
#include "components/BulletMover.h"
#include "systems/BulletPhysics.h"
#include <map>

namespace Sigma {
    class PhysicalWorldController {
    public:
        PhysicalWorldController(BulletPhysics& bphys) : mover(1), bphys(&bphys) {};

        virtual ~PhysicalWorldController() {};

        static void AddObject(type_id id, const coordinate_type x, const coordinate_type y,
                       const coordinate_type z, const coordinate_type rx, const coordinate_type ry, const coordinate_type rz);

        // TODO : move to UserViewSystem
        void AddViewer(type_id id, coordinate_type view_limit) {
            viewDistanceMap.insert({{id, view_limit}});
            viewBitsetMap.insert({{id, BitArray<unsigned short>::Create()}});
            AddObject(id, 0 , 1.5, 0, 0, 0, 0);
//            this->bphys->initBulletMover(mover, 0, 1.5, 0, 0, 0, 0);
        };

        static inline SigmaMotionState* GetMotionState(type_id id) {
            return new SigmaMotionState(id, pphysical, ophysical);
        };

        // TODO : move to UserViewSystem
        inline std::unique_ptr<position_array> TransformedPositions(type_id id) {
            return pphysical.RelativeTo(pphysical.x(id), pphysical.y(id), pphysical.z(id));
        };

        BulletMover* GetViewMover() {
            return &mover;
        };



    private:
        static WorldPosition pphysical;
        static WorldOrientation ophysical;
        BulletPhysics* bphys;

  		// TODO: Currently only one mover
  		BulletMover mover;


        // TODO : move to UserViewSystem
        std::map<type_id, std::shared_ptr<BitArray<unsigned short>>> viewBitsetMap;
        std::map<type_id, coordinate_type> viewDistanceMap;
    };
}
#endif // PHYSICALWORLDCOMPONENT_H_INCLUDED
