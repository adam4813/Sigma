#pragma once
#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include <string>

#define SET_COMPONENT_ID(ID)                                \
static const Sigma::IComponent::ComponentID getStaticComponentID() {return ID;}\
virtual const Sigma::IComponent::ComponentID getComponentID() override{return getStaticComponentID();}

namespace Sigma{

    class IComponent {
    public:
        typedef std::string ComponentID;
        IComponent(const int id = 0) : entityID(id) {}
        virtual ~IComponent() {}
        int GetEntityID() { return this->entityID; }
        virtual const ComponentID getComponentID()=0;
    private:
        const int entityID; // The entity that owns this component.

    }; // class IComponent
} // namespace Sigma

#endif // ICOMPONENT_H
