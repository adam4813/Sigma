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
    class PhysicalWorldLocation : IComponent {
    public:
		SET_COMPONENT_TYPENAME("PhysicalWorldLocation");

        PhysicalWorldLocation(BulletPhysics& bphys) : mover(1), bphys(&bphys) {};

        virtual ~PhysicalWorldLocation() {};

        static void AddEntity(const id_t id, const coordinate_type x, const coordinate_type y,
                       const coordinate_type z, const coordinate_type rx, const coordinate_type ry, const coordinate_type rz);

        static void RemoveEntity(const id_t id) {
            pphysical.RemoveEntityPosition(id);
            ophysical.RemoveEntityOrientation(id);
        };

        static std::unique_ptr<position_type> getPosition(const id_t id) {
            return std::unique_ptr<position_type>(new position_type(pphysical.x(id), pphysical.y(id), pphysical.z(id)));
        };

        static std::unique_ptr<orientation_type> getOrientation(const id_t id) {
            return std::unique_ptr<orientation_type>(new orientation_type(ophysical.euler(id)));
        };

        // TODO : move to UserViewSystem
        void AddViewer(const id_t id, coordinate_type view_limit) {
            viewDistanceMap.insert({{id, view_limit}});
            viewBitsetMap.insert({{id, BitArray<unsigned short>::Create()}});
        };

        static inline SigmaMotionState* GetMotionState(const id_t id) {
            return new SigmaMotionState(id, pphysical, ophysical);
        };

        // TODO : move to UserViewSystem
        inline std::unique_ptr<position_array> TransformedPositions(const id_t id) {
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
        std::map<id_t, std::shared_ptr<BitArray<unsigned short>>> viewBitsetMap;
        std::map<id_t, coordinate_type> viewDistanceMap;
    };
}
#endif // PHYSICALWORLDCOMPONENT_H_INCLUDED
