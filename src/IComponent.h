#pragma once

class IComponent {
public:
	IComponent(const int id = 0) : entityID(id) {}
	virtual ~IComponent() {}
	int GetEntityID() { return this->entityID; }
private:
	const int entityID; // The entity that owns this component.
};