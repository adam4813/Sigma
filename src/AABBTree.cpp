#include "AABBTree.h"

#include "components/GLMesh.h"

void AABBTree::Subdivivde(Node* node, int maxDepth) {
	if (node == nullptr) {
		node = &this->root;
	}
	// Check if the current recursion depth is greater than the previous max depth
	if (maxDepth >= 0) {
		for (int i = 0; i < 8; ++i) {
			if (node->expanded == false) { // This level of children have never been expanded before so create new children.
					Node* n2b = new Node(node->halfsize / 2); // Quad 2 back
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

					Node* n1b = new Node(node->halfsize / 2); // Quad 1 back
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

					Node* n3b = new Node(node->halfsize / 2); // Quad 3 back
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

					Node* n4b = new Node(node->halfsize / 2); // Quad 4 back
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

					Node* n2f = new Node(node->halfsize / 2); // Quad 2 back
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

					Node* n1f = new Node(node->halfsize / 2); // Quad 1 back
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

					Node* n3f = new Node(node->halfsize / 2); // Quad 3 back
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

					Node* n4f = new Node(node->halfsize / 2); // Quad 4 back
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
					Subdivivde(node->children[i], maxDepth - 1);
				}
			}
		}
	}


}

void AABBTree::Populate(Sigma::GLMesh* mesh) {
	for (unsigned int i = 0; i < mesh->GetFaceCount(); ++i) {
		this->faces.push_back(mesh->GetFace(i));
	}

	for (unsigned int i = 0; i < mesh->GetVertexCount(); ++i) {
		this->verts.push_back(mesh->GetVertex(i));
	}

	for (unsigned int i = 0; i < this->faces.size(); ++i) {
		if (SAT(&this->root, this->faces[i])) {
			this->root.faceIndexCache.push_back(i);
		}
	}

	Subdivivde(nullptr, 1);
}

AABBTree::AABBTree() : root(5.0f) {
	this->depth = 0;

	float center[3] = {0,0,0};
	float halfsize = 2.5f;
	this->root.center[0] = center[0];
	this->root.center[1] = center[1];
	this->root.center[2] = center[2];

	/*Node* n2b = new Node(halfsize / 2); // Quad 2 back
	n2b->center[0] = center[0] - halfsize / 2;
	n2b->center[1] = center[1] + halfsize / 2;
	n2b->center[2] = center[2] - halfsize / 2;

	Node* n1b = new Node(halfsize / 2); // Quad 1 back
	n1b->center[0] = center[0] + halfsize / 2;
	n1b->center[1] = center[1] + halfsize / 2;
	n1b->center[2] = center[2] - halfsize / 2;

	Node* n3b = new Node(halfsize / 2); // Quad 3 back
	n3b->center[0] = center[0] - halfsize / 2;
	n3b->center[1] = center[1] - halfsize / 2;
	n3b->center[2] = center[2] - halfsize / 2;

	Node* n4b = new Node(halfsize / 2); // Quad 4 back
	n4b->center[0] = center[0] + halfsize / 2;
	n4b->center[1] = center[1] - halfsize / 2;
	n4b->center[2] = center[2] - halfsize / 2;

	Node* n2f = new Node(halfsize / 2); // Quad 2 back
	n2f->center[0] = center[0] - halfsize / 2;
	n2f->center[1] = center[1] + halfsize / 2;
	n2f->center[2] = center[2] + halfsize / 2;

	Node* n1f = new Node(halfsize / 2); // Quad 1 back
	n1f->center[0] = center[0] + halfsize / 2;
	n1f->center[1] = center[1] + halfsize / 2;
	n1f->center[2] = center[2] + halfsize / 2;

	Node* n3f = new Node(halfsize / 2); // Quad 3 back
	n3f->center[0] = center[0] - halfsize / 2;
	n3f->center[1] = center[1] - halfsize / 2;
	n3f->center[2] = center[2] + halfsize / 2;

	Node* n4f = new Node(halfsize / 2); // Quad 4 back
	n4f->center[0] = center[0] + halfsize / 2;
	n4f->center[1] = center[1] - halfsize / 2;
	n4f->center[2] = center[2] + halfsize / 2;

	this->root.children[0] = n2b;
	this->root.children[1] = n1b;
	this->root.children[2] = n3b;
	this->root.children[3] = n4b;
	this->root.children[4] = n2f;
	this->root.children[5] = n1f;
	this->root.children[6] = n3f;
	this->root.children[7] = n4f;
	this->root.expanded = true;*/
}

#include "tribox.h"

bool AABBTree::SAT(Node* n2b, const Sigma::Face* face) {
	float verts[3][3];
	verts[0][0] = this->verts[face->v1]->x;
	verts[0][1] = this->verts[face->v1]->y;
	verts[0][2] = this->verts[face->v1]->z;

	verts[1][0] = this->verts[face->v2]->x;
	verts[1][1] = this->verts[face->v2]->y;
	verts[1][2] = this->verts[face->v2]->z;

	verts[2][0] = this->verts[face->v3]->x;
	verts[2][1] = this->verts[face->v3]->y;
	verts[2][2] = this->verts[face->v3]->z;

	float halfsize[3] = {n2b->halfsize,n2b->halfsize,n2b->halfsize};

	return triBoxOverlap(n2b->center, halfsize, verts);
}

