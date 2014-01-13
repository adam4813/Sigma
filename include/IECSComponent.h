#ifndef IECSCOMPONENT_H_INCLUDED
#define IECSCOMPONENT_H_INCLUDED

#include <string>
#include "Sigma.h"

#define SET_COMPONENT_TYPENAME(ID)                                \
static const Sigma::IComponent::ComponentID getStaticComponentTypeName() {return ID;}\
virtual const Sigma::IComponent::ComponentID getComponentTypeName() const override{return getStaticComponentTypeName();}

namespace Sigma{
    /** \brief An interface for components
     */
    class IECSComponent {
    public:
        IECSComponent(){}
        virtual ~IECSComponent() {}
        virtual const ComponentID getComponentTypeName() const =0;
    }; // class IComponent
} // namespace Sigma


#endif // IECSCOMPONENT_H_INCLUDED
