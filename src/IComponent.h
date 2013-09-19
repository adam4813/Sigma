#pragma once
#include <string>

#define SET_COMPONENT_ID(ID)                                \
static const ComponentID getStaticComponentID() {return ID;}\
virtual const ComponentID getComponentID() override{return getStaticComponentID();}

class IComponent {
public:
    typedef std::string ComponentID;
	IComponent(const int id = 0) : entityID(id) {}
	virtual ~IComponent() {}
	int GetEntityID() { return this->entityID; }
	virtual const ComponentID getComponentID()=0;
private:
	const int entityID; // The entity that owns this component.
};
