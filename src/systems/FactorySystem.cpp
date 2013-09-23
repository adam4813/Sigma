#include "FactorySystem.h"
#include <iostream>

std::shared_ptr<FactorySystem> FactorySystem::_instance;
std::once_flag FactorySystem::only_once;

FactorySystem::FactorySystem()
{
    //ctor
}

FactorySystem::~FactorySystem()
{
    //dtor
}

FactorySystem& FactorySystem::getInstance()
{
    std::call_once( FactorySystem::only_once,
        [] ()
		{
		   FactorySystem::_instance.reset( new FactorySystem() );
        });
    return *FactorySystem::_instance;
}

void FactorySystem::create(const std::string type,
                           const unsigned int entityID,
                           std::vector<Property> &properties)
{
    if(registeredFactoryFunctions.find(type.c_str())!=
       registeredFactoryFunctions.end())
    {
        registeredFactoryFunctions[type.c_str()](type.c_str(),entityID,properties);
    }else
    {
        std::cerr << "Error: Couldn't create Component:" << type.c_str()  << std::endl;
    }
}

void FactorySystem::register_Factory(IFactory* Factory)
{
    for(auto& FactoryFunc : Factory->getFactoryFunctions())
    {
        registeredFactoryFunctions[FactoryFunc.first]=FactoryFunc.second;
    }
}
