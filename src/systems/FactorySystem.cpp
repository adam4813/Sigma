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
            registeredFactoryFunctions[type.c_str()](type.c_str(), entityID, properties);
        } else{
            std::cerr << "Error: Couldn't create Component:" << type.c_str()  << std::endl;
        }
    }

    void FactorySystem::register_Factory(IFactory& Factory){
        for(auto& FactoryFunc : Factory.getFactoryFunctions()){
            registeredFactoryFunctions[FactoryFunc.first]=FactoryFunc.second;
        }
    }

} // namespace Sigma
