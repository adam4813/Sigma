#pragma once

#include "../GLTransform.h"
#include "../IGLComponent.h"
#include <vector>

class GLMesh : public IGLComponent {
public:
	GLMesh(const int entityID); // Ctor that sets the entity ID.
	/**
	 * \brief Creates a new IGLComponent.
	 *
	 * \param[in] int entityID The entity this component belongs to.
	 * \returns IGLCompoent* the newly created component.
	 */
	void Initialize() ;

	/**
	 * \brief Returns the number of elements to draw for this component.
	 *
	 * \returns unsigned int The number of elements to draw.
	 */
	unsigned int NumberElements(const int group = 0) const {
		if (group < (groupIndex.size() - 1)) {
			return (groupIndex[group+1] - groupIndex[group]) * 3;
		} else if (group > (groupIndex.size() - 1)) {
			return 0;
		} else {
			return (this->faces.size() - groupIndex[group]) * 3;
		}
	}

	void LoadMesh(std::string fname);
private:
	std::vector<unsigned int> groupIndex; // Stores which index in faces a group starts at.
	std::vector<face> faces; // Stores vectors of face groupings.
	std::vector<vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
	std::vector<color> colors;
};
