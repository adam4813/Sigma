#pragma once
#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include <string>
#include "Sigma.h"

#define SET_COMPONENT_TYPENAME(ID)                                \
static const Sigma::IComponent::ComponentID getStaticComponentTypeName() {return ID;}\
virtual const Sigma::IComponent::ComponentID getComponentTypeName() override{return getStaticComponentTypeName();}

#define SET_STATIC_COMPONENT_TYPENAME(ID)                                \
static const Sigma::IComponent::ComponentID getComponentTypeName() {return ID;};

namespace Sigma{
    class IComponent {
    public:
        typedef std::string ComponentID;
        IComponent(const id_t id = 0) : entityID(id) {}
        virtual ~IComponent() {}
        int GetEntityID() { return this->entityID; }
        virtual const ComponentID getComponentTypeName()=0;
    private:
        const id_t entityID; // The entity that owns this component.

    }; // class IComponent
} // namespace Sigma

#endif // ICOMPONENT_H
