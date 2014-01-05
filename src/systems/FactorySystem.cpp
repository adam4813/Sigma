#include "systems/FactorySystem.h"

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
			std::cerr << "Creating component of type: " << type << std::endl;
            return registeredFactoryFunctions[type](entityID, properties);
        } else{
            std::cerr << "Error: Couldn't find component: " << type << std::endl;
            return nullptr;
        }
    }

    void FactorySystem::register_Factory(IFactory& Factory){
		const auto& factoryfunctions = Factory.getFactoryFunctions();
		for(auto FactoryFunc = factoryfunctions.begin(); FactoryFunc != factoryfunctions.end(); ++FactoryFunc){
			std::cerr << "Registering component factory of type: " << FactoryFunc->first << std::endl;
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
