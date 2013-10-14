#pragma once

#include "IGLComponent.h"

#include <vector>

namespace Sigma {
	class GLMesh;

	struct AABBTreeNode {
		AABBTreeNode(float halfsize) : halfsize(halfsize), expanded(false), inCollision(false) {
			for (int i = 0; i < 8; ++i) {
				this->children[i] = nullptr;
			}
		}
		~AABBTreeNode() {
			for (AABBTreeNode* child : this->children) {
				if (child != nullptr) {
					delete child;
				}
			}
		}
		float center[3];
		float halfsize;
		AABBTreeNode* children[8];
		std::vector<unsigned int> faceIndexCache;
		bool expanded;
		bool inCollision;
	};

	class AABBTree : public IComponent {
	public:
		AABBTree(const int entityID);
		SET_COMPONENT_ID("AABBTree");
		~AABBTree() { }

		/**
		 * \brief Subdivides the AABB Tree to depth.
		 * 
		 * \param[in/out] AABBTreeNode * node The node to subdivide (nullptr for root down)
		 * \param[in/out] int depth The current depth of the subdivision.
		 * \returns   void
		 */
		void Subdivivde(AABBTreeNode* node = nullptr, int depth = 0);

		/**
		 * \brief Populate the internal lists from a GLMesh.
		 * 
		 * \param[in/out] Sigma::GLMesh * mesh The mesh to gather faces and verts from to populate the internal lists
		 * \returns   void 
		 */
		void Populate(std::string fname);

		/**
		 * \brief Performs a SAT test on a node and a face.
		 * 
		 * \param[in] AABBTreeNode * n2b The node to check.
		 * \param[in] const Sigma::Face * face The face to check
		 * \return   bool True if the overlap or intersect.
		 */
		bool SAT( AABBTreeNode* n2b, const Sigma::Face& face );

		Sigma::GLMesh* GenerateMesh(unsigned int entityID);

		/**
		 * \brief Check for a collision within the tree.
		 *
		 * \param[in] float center[3] The center of the object's sphere to check against the tree.
		 * \param[in] float radius The radius of the object's sphere to check against the tree
		 * \return    bool true if the object collides with the tree.
		 */
		bool CollisionCheck(glm::vec3 SphereCenter, float sphereradius);

		void Offset(glm::vec3 val) { offset = val; }
		glm::vec3 Offset() const { return offset; }
	private:
		AABBTreeNode root;
		std::vector<Sigma::Face> faces;
		std::vector<Sigma::Vertex> verts;
		int currentDepth;
		glm::vec3 offset;
	};
}

bool AABBSphereTest(glm::vec3 AABBCenter, float AABBHalfsize, glm::vec3 SphereCenter, float SphereRadius);