#pragma once

#include <stdexcept>
#include "../GLTransform.h"
#include "../IGLComponent.h"
#include <stdexcept>
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
	GLuint ambientMap;
	GLuint diffuseMap;
	GLuint specularMap;
};

// Helper structs for OBJ loading
// Stores unique combinations of indices
struct VertexIndices {
		unsigned int vertex;
		unsigned int normal;
		unsigned int uv;
		unsigned int color;
};

// Stores a face with all indices
struct FaceIndices {
	VertexIndices v[3];
};

class GLMesh : public Sigma::IGLComponent {
public:
    SET_COMPONENT_ID("GLMesh");
	GLMesh(const int entityID); // Ctor that sets the entity ID.
	/**
	 * \brief Creates a new IGLComponent.
	 *
	 * \param[in] int entityID The entity this component belongs to.
	 * \returns IGLCompoent* the newly created component.
	 */
	void InitializeBuffers() ;
	virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

	/**
	 * \brief Returns the number of elements to draw for this component.
	 *
	 * Call this multiple times to get the elements for each mesh group. A return of 0 indicates all
	 * mesh groups have been returned, and the call loop should end.
	 * \param[in] const unsigned int group The mesh group to render.
	 * \returns unsigned int The number of elements to draw for the given mesh group.
	 */
	unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const {
		if (this->groupIndex.size() == 0) {
			return 0;
		}
		if ((group + 1) < (this->groupIndex.size())) {
			return (this->groupIndex[group+1] - this->groupIndex[group]) * 3;
		} else if (group > (this->groupIndex.size() - 1)) {
			return 0;
		} else {
			return (this->faces.size() - this->groupIndex[group]) * 3;
		}
	}

	void LoadMesh(std::string fname);

	void ParseMTL(std::string fname);

	/**
	 * \brief Add a vertex to the list.
	 *
	 * \param[in] const Sigma::Vertex & v The vertex to add. It is copied.
	 */
	void AddVertex(const Sigma::Vertex& v) {
		this->verts.push_back(v);
	}

	/**
	 * \brief Gets a vertex.
	 *
	 * Returns the vertex at the specific index.
	 * \param[in] const unsigned int index The index of the vertex to get.
	 * \returns   const Sigma::Vertex* The vertex at the index or nullptr if the index was invalid.
	 */
	const Sigma::Vertex* GetVertex(const unsigned int index) {
		try {
			return &this->verts.at(index);
    } catch (std::out_of_range&) {
			return nullptr;
		}
	}

	/**
	 * \brief Add a face to the list.
	 *
	 * \param[in] const Sigma::Face & f The face to add. It is copied.
	 */
	void AddFace(const Sigma::Face& f) {
		this->faces.push_back(f);
	}

	/**
	 * \brief Gets a face.
	 *
	 * Returns the face at the specific index.
	 * \param[in] const unsigned int index The index of the face to get.
	 * \returns   const Sigma::Face* The face at the index or nullptr if the index was invalid.
	 */
	const Sigma::Face* GetFace(const unsigned int index) {
		try {
			return &this->faces.at(index);
    } catch (std::out_of_range&) {
			return nullptr;
		}
	}

	bool RemoveFace(const unsigned int index) {
		try {
			this->faces.erase(this->faces.begin() + index);
			return true;
    } catch (std::out_of_range&) {
			return false;
		}
	}


	unsigned int GetFaceCount() {
		return this->faces.size();
	}

	/**
	 * \brief Adds a mesh group index.
	 *
	 * Adds a mesh group starting index to groupIndex. This is the starting face index for the mesh group.
	 * \param[in] const unsigned int index
	 */
	void AddMeshGroupIndex(const unsigned int index) {
		this->groupIndex.push_back(index);
	}

	/**
	 * \brief Add a vertex normal to the list.
	 *
	 * \param[in] const Sigma::Vertex & v The vertex normal to add. It is copied.
	 */
	void AddVertexNormal(const Sigma::Vertex& vn) {
		this->vertNorms.push_back(vn);
	}

	/**
	 * \brief Add a vertex color to the list.
	 *
	 * \param[in] const Sigma::Vertex & v The vertex color to add. It is copied.
	 */
	void AddVertexColor(const Sigma::Color& c) {
		this->colors.push_back(c);
	}

	/**
	 * \brief Gets a vertex color.
	 *
	 * Returns the vertex color at the specific index.
	 * \param[in] const unsigned int index The index of the color to get.
	 * \returns   const Sigma::Color* The color at the index or nullptr if the index was invalid.
	 */
	const Sigma::Color* GetVertexColor(const unsigned int index) {
		try {
			return &this->colors.at(index);
    } catch (std::out_of_range&) {
			return nullptr;
		}
	}
private:
	std::vector<unsigned int> groupIndex; // Stores which index in faces a group starts at.
	std::vector<Sigma::Face> faces; // Stores vectors of face groupings.
	std::vector<std::map<std::string, std::vector<unsigned int>>> materialGroups; // This is probably an abuse of the STL
	std::vector<Sigma::Vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
	std::vector<Sigma::Vertex> vertNorms; // The vertex normals for each vert.
	std::vector<texCoord> texCoords; // The texture coords for each vertex.
	std::vector<Sigma::Color> colors;
	std::map<std::string, material> mats;
};
