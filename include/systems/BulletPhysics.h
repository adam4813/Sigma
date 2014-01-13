#pragma  once

#include "IFactory.h"
#include "IECSFactory.h"
#include "ISystem.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "components/InterpolatedMovement.h"
#include "components/RigidBody.h"
#include "IBulletShape.h"
#include "Sigma.h"

class Property;
class IMoverComponent;
struct GLFPSView;

namespace Sigma {
	class BulletPhysics
		: public Sigma::IFactory, public Sigma::IECSFactory, public Sigma::ISystem<IBulletShape> {
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
		bool Update(const double delta);

		IComponent* createBulletShapeMesh(const id_t entityID, const std::vector<Property> &properties);
		IComponent* createBulletShapeSphere(const id_t entityID, const std::vector<Property> &properties);

		std::unique_ptr<IECSComponent> addControllableMove(const id_t entityID, const std::vector<Property> &properties);
		std::unique_ptr<IECSComponent> addInterpolatedMove(const id_t entityID, const std::vector<Property> &properties);
		std::unique_ptr<IECSComponent> addPhysicalWorldLocation(const id_t entityID, const std::vector<Property> &properties);
		std::unique_ptr<IECSComponent> addRigidBody(const id_t entityID, const std::vector<Property> &properties);

		std::map<std::string,IFactory::FactoryFunction> getFactoryFunctions();
		std::map<std::string,IECSFactory::FactoryFunction> getECSFactoryFunctions();

	private:
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
	};
}
