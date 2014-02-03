#include "systems/FactorySystem.h"
#include "Sigma.h"

namespace Sigma{
	std::shared_ptr<FactorySystem> FactorySystem::_instance;

	FactorySystem::FactorySystem(){
		// nothing to construct
	}

	FactorySystem::~FactorySystem(){
		// destruction of private members is handled by stl
	}

	/**
	 * \brief return or create the singleton instance
	 */
	FactorySystem& FactorySystem::getInstance(){
		if(!_instance){
			FactorySystem::_instance.reset( new FactorySystem() );
		}
		return *FactorySystem::_instance;
	}

	IComponent* FactorySystem::create(const std::string& type,
							   const id_t entityID,
							   const std::vector<Property> &properties){
		if(registeredFactoryFunctions.find(type) != registeredFactoryFunctions.end()){
			LOG << "Creating component of type: " << type;
			return registeredFactoryFunctions[type](entityID, properties);
		}
		else {
			return nullptr;
		}
	}

	std::vector<std::unique_ptr<IECSComponent>> FactorySystem::createECS(const std::string& type,
							const id_t entityID,
							const std::vector<Property> &properties){
		if(registeredECSFactoryFunctions.find(type) != registeredECSFactoryFunctions.end()){
			LOG << "Creating ECS component of type: " << type;
			return registeredECSFactoryFunctions[type](entityID, properties);
		}
		else {
			LOG_DEBUG << "Error: Couldn't find component: " << type;
			return std::vector<std::unique_ptr<IECSComponent>>();
		}
	}

	void FactorySystem::register_Factory(IFactory& Factory){
		const auto& factoryfunctions = Factory.getFactoryFunctions();
		for(auto FactoryFunc = factoryfunctions.begin(); FactoryFunc != factoryfunctions.end(); ++FactoryFunc){
			LOG << "Registering component factory of type: " << FactoryFunc->first;
			registeredFactoryFunctions[FactoryFunc->first]=FactoryFunc->second;
		}
	}

	void FactorySystem::register_ECSFactory(IECSFactory& Factory){
		const auto& factoryfunctions = Factory.getECSFactoryFunctions();
		for(auto FactoryFunc = factoryfunctions.begin(); FactoryFunc != factoryfunctions.end(); ++FactoryFunc){
			std::cerr << "Registering ECS component factory of type: " << FactoryFunc->first << std::endl;
			registeredECSFactoryFunctions[FactoryFunc->first]=FactoryFunc->second;
		}
	}
} // namespace Sigma
