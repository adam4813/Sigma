#pragma  once

#define BT_USE_DOUBLE_PRECISION
#include "IFactory.h"
#include "ISystem.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "IMoverComponent.h"
#include "IBulletShape.h"
#include "components/BulletMover.h"

class Property;
class IMoverComponent;
struct GLFPSView;

namespace Sigma {
	class BulletPhysics
		: public Sigma::IFactory, public Sigma::ISystem<IBulletShape> {
	public:
		BulletPhysics() {};
		~BulletPhysics();
		/**
		 * \brief Starts the Simple Physics system.
		 *
		 * \return bool Returns false on startup failure.
		 */
		bool Start();

		/**
		 * \brief Causes an update in the system based on the change in time.
		 *
		 * Updates the state of the system based off how much time has elapsed since the last update.
		 * \param[in] const float delta The change in time since the last update
		 * \return bool Returns true if we had an update interval passed.
		 */
		bool Update(BulletMover& mover, const double delta);

		IComponent* createBulletShapeMesh(const unsigned int entityID, const std::vector<Property> &properties);
		IComponent* createBulletShapeSphere(const unsigned int entityID, const std::vector<Property> &properties);

		void initBulletMover(BulletMover& mover, const coordinate_type x, const coordinate_type y, const coordinate_type z,
                         const coordinate_type rx, const coordinate_type ry, const coordinate_type rz);

		std::map<std::string,FactoryFunction> getFactoryFunctions();
	private:
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
	};
}
