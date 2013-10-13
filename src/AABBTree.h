#pragma once

#include "IGLComponent.h"

#include <vector>

namespace Sigma {
	class GLMesh;
}

struct Node {
	Node(float halfsize) : halfsize(halfsize), expanded(false) {
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
private:
	Node root;
	std::vector<const Sigma::Face*> faces;
	std::vector<const Sigma::Vertex*> verts;
	int currentDepth;
};