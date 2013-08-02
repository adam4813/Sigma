#pragma once

//#include <matrix>
#include <map>

struct TRANSFORMATION {
	float scale[3];
	float rotation[3];
	float translation[3];
};

class SceneManager {
public:
	void SetEntity(int id, TRANSFORMATION position); // Adds an entity with ID to the scene at the given TRANSFORMATION. 
	TRANSFORMATION* GetEntity(int id); // Returns the TRANSFORMATION stored for the given entity.
private:
	std::map<int, TRANSFORMATION> entitySRT;
};