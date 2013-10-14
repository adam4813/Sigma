#pragma once

#include "IGLComponent.h"

#include <vector>

namespace Sigma {
	class GLMesh;
}

struct Node {
	Node(float halfsize) : halfsize(halfsize), expanded(false), inCollision(false) {
		for (int i = 0; i < 8; ++i) {
			this->children[i] = nullptr;
		}
	}
	~Node() {
		for (Node* child : this->children) {
			if (child != nullptr) {
				delete child;
			}
		}
	}
	float center[3];
	float halfsize;
	Node* children[8];
	std::vector<unsigned int> faceIndexCache;
	bool expanded;
	bool inCollision;
};

class AABBTree {
public:
	AABBTree();
	~AABBTree() { }

	/**
	 * \brief Subdivides the AABB Tree to depth.
	 * 
	 * \param[in/out] Node * node The node to subdivide (nullptr for root down)
	 * \param[in/out] int depth The current depth of the subdivision.
	 * \returns   void
	 */
	void Subdivivde(Node* node = nullptr, int depth = 0);

	/**
	 * \brief Populate the internal lists from a GLMesh.
	 * 
	 * \param[in/out] Sigma::GLMesh * mesh The mesh to gather faces and verts from to populate the internal lists
	 * \returns   void 
	 */
	void Populate(Sigma::GLMesh* mesh);

	/**
	 * \brief Performs a SAT test on a node and a face.
	 * 
	 * \param[in] Node * n2b The node to check.
	 * \param[in] const Sigma::Face * face The face to check
	 * \return   bool True if the overlap or intersect.
	 */
	bool SAT( Node* n2b, const Sigma::Face* face );

	Sigma::GLMesh* GenerateMesh(unsigned int entityID);

	/**
	 * \brief Check for a collision within the tree.
	 *
	 * \param[in] float center[3] The center of the object's sphere to check against the tree.
	 * \param[in] float radius The radius of the object's sphere to check against the tree
	 * \return    bool true if the object collides with the tree.
	 */
	bool CollisionCheck(glm::vec3 SphereCenter, float sphereradius);
private:
	Node root;
	std::vector<const Sigma::Face*> faces;
	std::vector<const Sigma::Vertex*> verts;
	int currentDepth;
};