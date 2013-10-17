#pragma  once

#include "../IFactory.h"
#include "../ISystem.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "../IMoverComponent.h"
#include "../IBulletShape.h"

class Property;
class IMoverComponent;

namespace Sigma {
	class BulletPhysics
		: public Sigma::IFactory, public ISystem<IBulletShape> {
	public:
		BulletPhysics() { }
		~BulletPhysics();;
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
		bool Update(const double delta);

		void createBulletShapeMesh(const unsigned int entityID, std::vector<Property> &properties) ;

		std::map<std::string,FactoryFunction> getFactoryFunctions();
	private:
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
	};
}
