#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include "IComponent.h"

namespace Sigma{

    template<typename T>
    class ISystem
    {
        typedef std::unordered_map< IComponent::ComponentID,
                                    std::unique_ptr<T>> ComponentMap;
        typedef std::unordered_map<std::size_t,ComponentMap> EntityComponentMap;
        public:
            ISystem() {};
            virtual ~ISystem() {};
            /**
             * \brief Retrieves a component
             *
             * Retrieves the Component specified by ID belonging to the Entity specified by EntityID
             * In case t
             * \param[in] std::size_t EntityId the Id of the Entity the wanted Component belongs to
             * \param[in] IComponent::ComponentID ID The Id of the wanted Component
             * \return   T* returns the Component or NULL, if either the Entity doesn't exist or the Entity doesn't have that Component
             */
            T* getComponent(std::size_t EntityID, IComponent::ComponentID ID)
            {
                if(this->_Components.find(EntityID)!=this->_Components.end()&&
                   this->_Components[EntityID].find(ID)!=this->_Components[EntityID].end())
                {
                    return _Components[EntityID][ID].get();
                }else
                {
                    return NULL;
                }
            }

            /**
             * \brief Adds a component
             *
             * Adds the given Component to the Entity specified by EntityID
             * \param[in] std::size_t EntityId the Id of the Entity the Component belongs to
             * \param[in] T* Component The Component that should be added to the given EntityID
             */
            void addComponent(std::size_t EntityID,T* Component)
            {
                auto found=_Components.find(EntityID);
                if(found==_Components.end())
                {
                    _Components.insert(std::pair<std::size_t,ComponentMap>(EntityID,ComponentMap()));
                }
                _Components[EntityID][Component->getComponentTypeName()]=std::unique_ptr<T>(Component);
            }
        protected:
            EntityComponentMap _Components;
        private:
    };

}

#endif // ISYSTEM_H
