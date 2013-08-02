#pragma once

class IComponent {
public:
	IComponent(const int id = 0) : entityID(id) {}
	virtual ~IComponent() {}
private:
	const int entityID; // The entity that owns this component.
};