#pragma once

#include "../GLTransform.h"
#include "../IGLComponent.h"
#include <vector>
#include <map>

struct texCoord {
	texCoord(float u, float v = 0.0f) : u(u), v(v) { }
	float u, v;
};

struct material {
	material() {
		ka[0] = 0.2f; ka[1] = 0.2f; ka[2] = 0.2f;
		kd[0] = 0.8f; kd[1] = 0.8f; kd[2] = 0.8f;
		ks[0] = 1.0f; ks[1] = 1.0f; ks[2] = 1.0f;
		tr = 1.0f;
		ns = 0.0f;
		illum = 1;
	}
	float ka[3];
	float kd[3];
	float ks[3];
	float tr; // Aka d
	float ns;
	int illum;
	// TODO: Add maps
};

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

	void ParseMTL(std::string fname);
private:
	std::vector<unsigned int> groupIndex; // Stores which index in faces a group starts at.
	std::vector<face> faces; // Stores vectors of face groupings.
	std::vector<vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
	std::vector<vertex> vertNorms; // The vertex normals for each vert.
	std::vector<face> faceNorms; // The index for each vert normal.
	std::vector<texCoord> texCoords; // The texture coords for each vertex.
	std::vector<face> texFaces; // The texture coords for each face.
	std::vector<color> colors;
	std::map<std::string, material> mats;
};
