#include "AABBTree.h"

#include "components/GLMesh.h"
#include <queue>
#include <stack>

namespace Sigma {
	AABBTree::AABBTree(const int entityID) : IComponent(entityID), root(2.5f) {
		this->currentDepth = 0;

		float center[3] = {0,0,0};
		this->root.center[0] = center[0];
		this->root.center[1] = center[1];
		this->root.center[2] = center[2];
	}

	void AABBTree::Subdivivde(AABBTreeNode* node /*= nullptr*/, int depth /*= 0*/) {
		if (node == nullptr) {
			node = &this->root;
		}
		if (depth > (currentDepth + 1)) {
			depth = currentDepth + 1;
		}
		// Check if the depth is greater than the previous current depth
		if (depth >= 0) {
			for (int i = 0; i < 8; ++i) {
				if (node->expanded == false) { // This level of children have never been expanded before so create new children.
						AABBTreeNode* n2b = new AABBTreeNode(node->halfsize / 2); // Quad 2 back
						n2b->center[0] = node->center[0] - node->halfsize / 2;
						n2b->center[1] = node->center[1] + node->halfsize / 2;
						n2b->center[2] = node->center[2] - node->halfsize / 2;

						for (unsigned int i = 0; i < node->faceIndexCache.size(); ++i) {
							if (SAT(n2b, this->faces[node->faceIndexCache[i]])) {
								n2b->faceIndexCache.push_back(node->faceIndexCache[i]);
							}
						}
						if (n2b->faceIndexCache.size() > 0) {
							node->children[0] = n2b;
						} else {
							node->children[0] = nullptr;
							delete n2b;
						}

						AABBTreeNode* n1b = new AABBTreeNode(node->halfsize / 2); // Quad 1 back
						n1b->center[0] = node->center[0] + node->halfsize / 2;
						n1b->center[1] = node->center[1] + node->halfsize / 2;
						n1b->center[2] = node->center[2] - node->halfsize / 2;

						for (unsigned int i = 0; i < node->faceIndexCache.size(); ++i) {
							if (SAT(n1b, this->faces[node->faceIndexCache[i]])) {
								n1b->faceIndexCache.push_back(node->faceIndexCache[i]);
							}
						}
						if (n1b->faceIndexCache.size() > 0) {
							node->children[1] = n1b;
						} else {
							node->children[1] = nullptr;
							delete n1b;
						}

						AABBTreeNode* n3b = new AABBTreeNode(node->halfsize / 2); // Quad 3 back
						n3b->center[0] = node->center[0] - node->halfsize / 2;
						n3b->center[1] = node->center[1] - node->halfsize / 2;
						n3b->center[2] = node->center[2] - node->halfsize / 2;

						for (unsigned int i = 0; i < node->faceIndexCache.size(); ++i) {
							if (SAT(n3b, this->faces[node->faceIndexCache[i]])) {
								n3b->faceIndexCache.push_back(node->faceIndexCache[i]);
							}
						}
						if (n3b->faceIndexCache.size() > 0) {
							node->children[2] = n3b;
						} else {
							node->children[2] = nullptr;
							delete n3b;
						}

						AABBTreeNode* n4b = new AABBTreeNode(node->halfsize / 2); // Quad 4 back
						n4b->center[0] = node->center[0] + node->halfsize / 2;
						n4b->center[1] = node->center[1] - node->halfsize / 2;
						n4b->center[2] = node->center[2] - node->halfsize / 2;

						for (unsigned int i = 0; i < node->faceIndexCache.size(); ++i) {
							if (SAT(n4b, this->faces[node->faceIndexCache[i]])) {
								n4b->faceIndexCache.push_back(node->faceIndexCache[i]);
							}
						}
						if (n4b->faceIndexCache.size() > 0) {
							node->children[3] = n4b;
						} else {
							node->children[3] = nullptr;
							delete n4b;
						}

						AABBTreeNode* n2f = new AABBTreeNode(node->halfsize / 2); // Quad 2 back
						n2f->center[0] = node->center[0] - node->halfsize / 2;
						n2f->center[1] = node->center[1] + node->halfsize / 2;
						n2f->center[2] = node->center[2] + node->halfsize / 2;

						for (unsigned int i = 0; i < node->faceIndexCache.size(); ++i) {
							if (SAT(n2f, this->faces[node->faceIndexCache[i]])) {
								n2f->faceIndexCache.push_back(node->faceIndexCache[i]);
							}
						}
						if (n2f->faceIndexCache.size() > 0) {
							node->children[4] = n2f;
						} else {
							node->children[4] = nullptr;
							delete n2f;
						}

						AABBTreeNode* n1f = new AABBTreeNode(node->halfsize / 2); // Quad 1 back
						n1f->center[0] = node->center[0] + node->halfsize / 2;
						n1f->center[1] = node->center[1] + node->halfsize / 2;
						n1f->center[2] = node->center[2] + node->halfsize / 2;

						for (unsigned int i = 0; i < node->faceIndexCache.size(); ++i) {
							if (SAT(n1f, this->faces[node->faceIndexCache[i]])) {
								n1f->faceIndexCache.push_back(node->faceIndexCache[i]);
							}
						}
						if (n1f->faceIndexCache.size() > 0) {
							node->children[5] = n1f;
						} else {
							node->children[5] = nullptr;
							delete n1f;
						}

						AABBTreeNode* n3f = new AABBTreeNode(node->halfsize / 2); // Quad 3 back
						n3f->center[0] = node->center[0] - node->halfsize / 2;
						n3f->center[1] = node->center[1] - node->halfsize / 2;
						n3f->center[2] = node->center[2] + node->halfsize / 2;

						for (unsigned int i = 0; i < node->faceIndexCache.size(); ++i) {
							if (SAT(n3f, this->faces[node->faceIndexCache[i]])) {
								n3f->faceIndexCache.push_back(node->faceIndexCache[i]);
							}
						}
						if (n3f->faceIndexCache.size() > 0) {
							node->children[6] = n3f;
						} else {
							node->children[6] = nullptr;
							delete n3f;
						}

						AABBTreeNode* n4f = new AABBTreeNode(node->halfsize / 2); // Quad 4 back
						n4f->center[0] = node->center[0] + node->halfsize / 2;
						n4f->center[1] = node->center[1] - node->halfsize / 2;
						n4f->center[2] = node->center[2] + node->halfsize / 2;

						for (unsigned int i = 0; i < node->faceIndexCache.size(); ++i) {
							if (SAT(n4f, this->faces[node->faceIndexCache[i]])) {
								n4f->faceIndexCache.push_back(node->faceIndexCache[i]);
							}
						}
						if (n4f->faceIndexCache.size() > 0) {
							node->children[7] = n4f;
						} else {
							node->children[7] = nullptr;
							delete n4f;
						}

						node->expanded = true;
				}
				else { // We have already expanded this level
					// If the child is null it means there are no faces that would intersect it.
					if (node->children[i] != nullptr) {
						Subdivivde(node->children[i], depth - 1);
					}
				}
			}
		}
		this->currentDepth = depth;
	}

	void AABBTree::Populate(std::string fname) {
		Sigma::GLMesh mesh(0);
		mesh.LoadMesh(fname);
		for (unsigned int i = 0; i < mesh.GetFaceCount(); ++i) {
			this->faces.push_back(*mesh.GetFace(i));
		}

		glm::quat qX = glm::angleAxis(this->rotation.x, 1.0f,0.0f,0.0f);
		glm::quat qY = glm::angleAxis(this->rotation.y, 0.0f,1.0f,0.0f);
		glm::quat qZ = glm::angleAxis(this->rotation.z, 0.0f,0.0f,1.0f);
		glm::quat orientation = qX * qY * qZ;
		glm::mat4 rotateMatrix = glm::mat4_cast(orientation);

		for (unsigned int i = 0; i < mesh.GetVertexCount(); ++i) {
			glm::vec4 vert(mesh.GetVertex(i)->x, mesh.GetVertex(i)->y, mesh.GetVertex(i)->z, 1.0f);
			glm::vec4 transformedVert = rotateMatrix * vert;
			this->verts.push_back(Sigma::Vertex(transformedVert.x, transformedVert.y, transformedVert.z));
		}

		for (unsigned int i = 0; i < this->faces.size(); ++i) {
			if (SAT(&this->root, this->faces[i])) {
				this->root.faceIndexCache.push_back(i);
			}
		}

		root.halfsize = this->halfsize;
		Subdivivde(nullptr, 1);
	}

	#include "tribox.h"

	bool AABBTree::SAT(AABBTreeNode* n2b, const Sigma::Face& face) {
		float verts[3][3];
		verts[0][0] = this->verts[face.v1].x;
		verts[0][1] = this->verts[face.v1].y;
		verts[0][2] = this->verts[face.v1].z;

		verts[1][0] = this->verts[face.v2].x;
		verts[1][1] = this->verts[face.v2].y;
		verts[1][2] = this->verts[face.v2].z;

		verts[2][0] = this->verts[face.v3].x;
		verts[2][1] = this->verts[face.v3].y;
		verts[2][2] = this->verts[face.v3].z;

		float halfsize[3] = {n2b->halfsize,n2b->halfsize,n2b->halfsize};

		return triBoxOverlap(n2b->center, halfsize, verts);
	}

	Sigma::GLMesh* AABBTree::GenerateMesh(unsigned int entityID) {
		Sigma::GLMesh* mesh = new Sigma::GLMesh(entityID);
		mesh->AddMeshGroupIndex(0);
		std::stack<AABBTreeNode*> endStack;
		std::stack<AABBTreeNode*> openStack;
		openStack.push(&root);

		while (openStack.size() > 0) {
			AABBTreeNode* top = openStack.top();
			openStack.pop();
			for (unsigned int i = 0; i < 8; ++i) {
				if (top->children[i] != nullptr) {
					if (top->children[i]->expanded == true) {
						openStack.push(top->children[i]);
					}
					else {
						endStack.push(top->children[i]);
					}
				}
			}
		}

		while (endStack.size() > 0) {
			float center[3] = {endStack.top()->center[0], endStack.top()->center[1], endStack.top()->center[2]};
			float halfsize = endStack.top()->halfsize;

			unsigned int offset = mesh->GetVertexCount();

			mesh->AddVertex(Sigma::Vertex(center[0] - halfsize, center[1] - halfsize, center[2] + halfsize)); // q3f
			mesh->AddVertexNormal(Sigma::Vertex(1, 1, -1)); // q3f
			if (endStack.top()->inCollision) {
				mesh->AddVertexColor(Sigma::Color(1.0f,0.0f,0.0f));
			}
			else {
				mesh->AddVertexColor(Sigma::Color(1.0f,1.0f,1.0f));
			}
			mesh->AddVertex(Sigma::Vertex(center[0] + halfsize, center[1] - halfsize, center[2] + halfsize)); // q4f
			mesh->AddVertexNormal(Sigma::Vertex(-1, 1, -1)); // q3f
			if (endStack.top()->inCollision) {
				mesh->AddVertexColor(Sigma::Color(1.0f,0.0f,0.0f));
			}
			else {
				mesh->AddVertexColor(Sigma::Color(1.0f,1.0f,1.0f));
			}
			mesh->AddVertex(Sigma::Vertex(center[0] + halfsize, center[1] + halfsize, center[2] + halfsize)); // q1f
			mesh->AddVertexNormal(Sigma::Vertex(-1, -1, -1)); // q3f
			if (endStack.top()->inCollision) {
				mesh->AddVertexColor(Sigma::Color(1.0f,0.0f,0.0f));
			}
			else {
				mesh->AddVertexColor(Sigma::Color(1.0f,1.0f,1.0f));
			}
			mesh->AddVertex(Sigma::Vertex(center[0] - halfsize, center[1] + halfsize, center[2] + halfsize)); // q2f
			mesh->AddVertexNormal(Sigma::Vertex(1, -1, -1)); // q3f
			if (endStack.top()->inCollision) {
				mesh->AddVertexColor(Sigma::Color(1.0f,0.0f,0.0f));
			}
			else {
				mesh->AddVertexColor(Sigma::Color(1.0f,1.0f,1.0f));
			}

			mesh->AddVertex(Sigma::Vertex(center[0] - halfsize, center[1] - halfsize, center[2] - halfsize)); // q3b
			mesh->AddVertexNormal(Sigma::Vertex(1, 1, 1)); // q3f
			if (endStack.top()->inCollision) {
				mesh->AddVertexColor(Sigma::Color(1.0f,0.0f,0.0f));
			}
			else {
				mesh->AddVertexColor(Sigma::Color(1.0f,1.0f,1.0f));
			}
			mesh->AddVertex(Sigma::Vertex(center[0] + halfsize, center[1] - halfsize, center[2] - halfsize)); // q4b
			mesh->AddVertexNormal(Sigma::Vertex(-1, 1, 1)); // q3f
			if (endStack.top()->inCollision) {
				mesh->AddVertexColor(Sigma::Color(1.0f,0.0f,0.0f));
			}
			else {
				mesh->AddVertexColor(Sigma::Color(1.0f,1.0f,1.0f));
			}
			mesh->AddVertex(Sigma::Vertex(center[0] + halfsize, center[1] + halfsize, center[2] - halfsize)); // q1b
			mesh->AddVertexNormal(Sigma::Vertex(-1, -1, 1)); // q3f
			if (endStack.top()->inCollision) {
				mesh->AddVertexColor(Sigma::Color(1.0f,0.0f,0.0f));
			}
			else {
				mesh->AddVertexColor(Sigma::Color(1.0f,1.0f,1.0f));
			}
			mesh->AddVertex(Sigma::Vertex(center[0] - halfsize, center[1] + halfsize, center[2] - halfsize)); // q2b
			mesh->AddVertexNormal(Sigma::Vertex(1, -1, 1)); // q3f
			if (endStack.top()->inCollision) {
				mesh->AddVertexColor(Sigma::Color(1.0f,0.0f,0.0f));
			}
			else {
				mesh->AddVertexColor(Sigma::Color(1.0f,1.0f,1.0f));
			}

			mesh->AddFace(Sigma::Face(0 + offset,1 + offset,2 + offset));
			mesh->AddFace(Sigma::Face(2 + offset,3 + offset,0 + offset));

			mesh->AddFace(Sigma::Face(3 + offset,2 + offset,6 + offset));
			mesh->AddFace(Sigma::Face(6 + offset,7 + offset,3 + offset));

			mesh->AddFace(Sigma::Face(7 + offset,6 + offset,5 + offset));
			mesh->AddFace(Sigma::Face(5 + offset,4 + offset,7 + offset));

			mesh->AddFace(Sigma::Face(4 + offset,0 + offset,3 + offset));
			mesh->AddFace(Sigma::Face(3 + offset,7 + offset,4 + offset));

			mesh->AddFace(Sigma::Face(0 + offset,1 + offset,5 + offset));
			mesh->AddFace(Sigma::Face(5 + offset,4 + offset,0 + offset));

			mesh->AddFace(Sigma::Face(1 + offset,5 + offset,6 + offset));
			mesh->AddFace(Sigma::Face(6 + offset,2 + offset,1 + offset));
			endStack.pop();
		}

		mesh->SetCullFace("none");
		mesh->Transform()->Scale(1.0f,1.0f,1.0f);
		mesh->Transform()->Translate(this->offset);
		mesh->LoadShader(); // load default
		mesh->InitializeBuffers();

		return mesh;
	}

	unsigned int AABBTree::CollisionCheck(glm::vec3 SphereCenter, float SphereRadius) {
		std::stack<AABBTreeNode*> trace;
		trace.push(&this->root);
		unsigned int numCollisions = 0;
		this->collisions.clear();

		while (trace.size() > 0) {
			AABBTreeNode* top = trace.top();
			trace.pop();
			for (unsigned int i = 0; i < 8; ++i) {
				if (top->children[i] != nullptr) {
					if (top->children[i]->expanded == true) {
						trace.push(top->children[i]);
					}
					else {
						CollisionPoint cp;
						if (AABBSphereTest(glm::vec3(top->children[i]->center[0], top->children[i]->center[1], top->children[i]->center[2]), top->children[i]->halfsize, SphereCenter, SphereRadius, cp.position)) {
							top->children[i]->inCollision = true;

							// Determine the axis the normal will be on
							glm::vec3 normalAxis;
							if ((cp.position.z == top->children[i]->halfsize) || (cp.position.z == -top->children[i]->halfsize)) {
								if (cp.position.z > 0) {
									normalAxis.z = -1.0f;
								}
								else {
									normalAxis.z = 1.0f;
								}
							}
							else if ((cp.position.x == top->children[i]->halfsize) || (cp.position.x == -top->children[i]->halfsize)) {
								if (cp.position.x > 0) {
									normalAxis.x = 1.0f;
								}
								else {
									normalAxis.x = -1.0f;
								}
							}

							// Compute the normal
							glm::vec3 p0,p1,p2;
							p0.x = -1.0f * normalAxis.x;
							p1.x = normalAxis.x;
							p2.x = -1.0f * normalAxis.x;

							p0.y = 1.0f;
							p1.y = 1.0f;
							p2.y = -1.0f;

							p0.z = -1.0f * normalAxis.z;
							p1.z = normalAxis.z;
							p2.z = -1.0f * normalAxis.z;

							cp.normal = glm::cross(p1 - p0, p2 - p0);

							cp.normal *= 0.75f; // Scale it (normalizing causes penetration

							cp.normal.y = 0.0f; // Remove any y as we have no vertical movement
							this->collisions.push_back(cp);
							++numCollisions;
						}
						else {
							top->children[i]->inCollision = false;
						}
					}
				}
			}
		}
		return numCollisions;
	}

	const CollisionPoint* AABBTree::GetCollisionPoint(const unsigned int index) {
		if (index < this->collisions.size()) {
			return &this->collisions[index];
		}
		return nullptr;
	}
}

bool AABBSphereTest(glm::vec3 AABBCenter, float AABBHalfsize, glm::vec3 SphereCenter, float SphereRadius, glm::vec3& collisionPoint) {
	// Get the center of the sphere relative to the center of the box
	glm::vec3 sphereCenterRelBox = SphereCenter - glm::vec3(AABBCenter);

	// Check sphere center against box along the X axis alone. 
	// If the sphere is off past the left edge of the box, 
	// then the left edge is closest to the sphere. 
	// Similar if it's past the right edge. If it's between 
	// the left and right edges, then the sphere's own X 
	// is closest, because that makes the X distance 0, 
	// and you can't get much closer than that :)

	if (sphereCenterRelBox.x < -AABBHalfsize)
		collisionPoint.x = -AABBHalfsize;
	else if (sphereCenterRelBox.x > AABBHalfsize)
		collisionPoint.x = AABBHalfsize;
	else
		collisionPoint.x = sphereCenterRelBox.x;

	// ...same for Y axis
	if (sphereCenterRelBox.y < -AABBHalfsize)
		collisionPoint.y = -AABBHalfsize;
	else if (sphereCenterRelBox.y > AABBHalfsize)
		collisionPoint.y = AABBHalfsize;
	else
		collisionPoint.y = sphereCenterRelBox.y;

	// ... same for Z axis
	if (sphereCenterRelBox.z < -AABBHalfsize)
		collisionPoint.z = -AABBHalfsize;
	else if (sphereCenterRelBox.z > AABBHalfsize)
		collisionPoint.z = AABBHalfsize;
	else
		collisionPoint.z = sphereCenterRelBox.z;

	// Now we have the closest point on the box, so get the distance from 
	// that to the sphere center, and see if it's less than the radius

	glm::vec3 dist = sphereCenterRelBox - collisionPoint;

	if (dist.x*dist.x + dist.y*dist.y + dist.z*dist.z < SphereRadius*SphereRadius)
		return true;
	else
		return false;
}
