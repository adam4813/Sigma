#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include "IComponent.h"

template<typename T>
class ISystem
{
    typedef std::unordered_map< IComponent::ComponentID,
                                std::unique_ptr<T>> ComponentMap;
    typedef std::unordered_map<std::size_t,ComponentMap> EntityComponentMap;
    public:
        ISystem() {};
        virtual ~ISystem() {};
        T* getComponent(std::size_t EntityID, IComponent::ComponentID ID)
        {
            if(this->_Components.find(EntityID)!=this->_Components.end())
            {
                return _Components[EntityID][ID].get();
            }else
            {
                return NULL;
            }
        }
        void addComponent(std::size_t EntityID,T* Component)
        {
            auto found=_Components.find(EntityID);
            if(found==_Components.end())
            {
                _Components.insert(std::pair<std::size_t,ComponentMap>(EntityID,ComponentMap()));
            }
            _Components[EntityID][Component->getComponentID()]=std::unique_ptr<T>(Component);
        }
    protected:
        EntityComponentMap _Components;
    private:
};

#endif // ISYSTEM_H
