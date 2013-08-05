#pragma once

//#include <matrix>
#include <map>

struct TRANSFORM {
	float scale[3];
	float rotation[3];
	float translation[3];
};

class SceneManager {
public:
	void SetEntity(int id, TRANSFORM position); // Adds an entity with ID to the scene at the given TRANSFORM. 
	TRANSFORM* GetEntity(int id); // Returns the TRANSFORM stored for the given entity.
private:
	std::map<int, TRANSFORM> entitySRT;
};