#include "FactorySystem.h"

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

    void FactorySystem::create(const std::string type,
                               const unsigned int entityID,
                               std::vector<Property> &properties){
        if(registeredFactoryFunctions.find(type.c_str()) !=
			registeredFactoryFunctions.end()){
				std::cerr << "Creating component of type: " << type.c_str() << std::endl;
            registeredFactoryFunctions[type.c_str()](entityID, properties);
        } else{
            std::cerr << "Error: Couldn't create Component:" << type.c_str()  << std::endl;
        }
    }

    void FactorySystem::register_Factory(IFactory& Factory){
		auto& factoryfunctions = Factory.getFactoryFunctions();
		for(auto& FactoryFunc = factoryfunctions.begin(); FactoryFunc != factoryfunctions.end(); ++FactoryFunc){
			std::cerr << "Registering component factory of type: " << FactoryFunc->first << std::endl;
            registeredFactoryFunctions[FactoryFunc->first]=FactoryFunc->second;
        }
    }

} // namespace Sigma
