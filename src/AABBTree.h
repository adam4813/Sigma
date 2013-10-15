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

	struct CollisionPoint {
		glm::vec3 position;
		glm::vec3 normal;
	};

	class AABBTree : public IComponent {
	public:
		AABBTree(const int entityID);
		SET_COMPONENT_ID("AABBTree");
		~AABBTree() { }

		/**
		 * \brief Subdivides the AABB Tree to depth.
		 * 
		 * \param[in] AABBTreeNode * node The node to subdivide (nullptr for root down)
		 * \param[in] int depth The current depth of the subdivision.
		 * \returns   void
		 */
		void Subdivivde(AABBTreeNode* node = nullptr, int depth = 0);

		/**
		 * \brief Populate the internal lists from a GLMesh.
		 * 
		 * \param[in/] std::string fname The name of the mesh file to load.
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
		 * \return    unsigned int The number of nodes in collision.
		 */
		unsigned int CollisionCheck(glm::vec3 SphereCenter, float sphereradius);

		/**
		 * \brief Returns a collision point at the given index.
		 *
		 * \param[in] const unsigned int index
		 * \return    const CollisionPoint* The point of collision for the given index.
		 */
		const CollisionPoint* GetCollisionPoint(const unsigned int index);

		void Offset(glm::vec3 val) { this->offset = val; }
		glm::vec3 Offset() const { return offset; }
		void Rotation(glm::vec3 val) { this->rotation = val; }
		glm::vec3 Rotation() const { return this->rotation; }
		void Halfsize( float val ) { this->halfsize = val; }
	private:
		std::vector<CollisionPoint> collisions; // A vector to hold nodes that are in collision.
		AABBTreeNode root;
		std::vector<Sigma::Face> faces;
		std::vector<Sigma::Vertex> verts;
		int currentDepth;
		glm::vec3 offset;
		glm::vec3 rotation;
		float halfsize;
	};
}

bool AABBSphereTest(glm::vec3 AABBCenter, float AABBHalfsize, glm::vec3 SphereCenter, float SphereRadius, glm::vec3& collisionPoint);