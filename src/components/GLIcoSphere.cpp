#include "GLIcoSphere.h"
#include "GL/glew.h"
#include <map>
#include <stdint.h>

GLIcoSphere::GLIcoSphere( const int entityID /*= 0*/ ) : GLMesh(entityID) {
}

void GLIcoSphere::InitializeBuffers() {
	srand(this->GetEntityID());
	// Create the verts to begin refining at.
	double t = (1.0 + glm::sqrt(5.0)) / 2.0;
	glm::vec2 coordPair = glm::normalize(glm::vec2(1,t));

	AddVertex(Sigma::Vertex(-coordPair.r, coordPair.g, 0));
	AddVertex(Sigma::Vertex(coordPair.r, coordPair.g, 0));
	AddVertex(Sigma::Vertex(-coordPair.r, -coordPair.g, 0));
	AddVertex(Sigma::Vertex(coordPair.r, -coordPair.g, 0));

	this->verts.push_back(Sigma::Vertex(-coordPair.r, coordPair.g, 0));
	this->verts.push_back(Sigma::Vertex(coordPair.r, coordPair.g, 0));
	this->verts.push_back(Sigma::Vertex(-coordPair.r, -coordPair.g, 0));
	this->verts.push_back(Sigma::Vertex(coordPair.r, -coordPair.g, 0));

	AddVertex(Sigma::Vertex(0, -coordPair.r, coordPair.g));
	AddVertex(Sigma::Vertex(0, coordPair.r, coordPair.g));
	AddVertex(Sigma::Vertex(0, -coordPair.r, -coordPair.g));
	AddVertex(Sigma::Vertex(0, coordPair.r, -coordPair.g));

	this->verts.push_back(Sigma::Vertex(0, -coordPair.r, coordPair.g));
	this->verts.push_back(Sigma::Vertex(0, coordPair.r, coordPair.g));
	this->verts.push_back(Sigma::Vertex(0, -coordPair.r, -coordPair.g));
	this->verts.push_back(Sigma::Vertex(0, coordPair.r, -coordPair.g));

	AddVertex(Sigma::Vertex(coordPair.g, 0, -coordPair.r));
	AddVertex(Sigma::Vertex(coordPair.g, 0, coordPair.r));
	AddVertex(Sigma::Vertex(-coordPair.g, 0, -coordPair.r));
	AddVertex(Sigma::Vertex(-coordPair.g, 0, coordPair.r));

	this->verts.push_back(Sigma::Vertex(coordPair.g, 0, -coordPair.r));
	this->verts.push_back(Sigma::Vertex(coordPair.g, 0, coordPair.r));
	this->verts.push_back(Sigma::Vertex(-coordPair.g, 0, -coordPair.r));
	this->verts.push_back(Sigma::Vertex(-coordPair.g, 0, coordPair.r));


	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

	// Create the faces to begin refining at.
	std::vector<Sigma::Face> faceLevelZero;

	faceLevelZero.push_back(Sigma::Face(0,11,5));
	faceLevelZero.push_back(Sigma::Face(0,5,1));
	faceLevelZero.push_back(Sigma::Face(0,1,7));
	faceLevelZero.push_back(Sigma::Face(0,7,10));
	faceLevelZero.push_back(Sigma::Face(0,10,11));

	faceLevelZero.push_back(Sigma::Face(1,5,9));
	faceLevelZero.push_back(Sigma::Face(5,11,4));
	faceLevelZero.push_back(Sigma::Face(11,10,2));
	faceLevelZero.push_back(Sigma::Face(10,7,6));
	faceLevelZero.push_back(Sigma::Face(7,1,8));

	faceLevelZero.push_back(Sigma::Face(3,9,4));
	faceLevelZero.push_back(Sigma::Face(3,4,2));
	faceLevelZero.push_back(Sigma::Face(3,2,6));
	faceLevelZero.push_back(Sigma::Face(3,6,8));
	faceLevelZero.push_back(Sigma::Face(3,8,9));

	faceLevelZero.push_back(Sigma::Face(4,9,5));
	faceLevelZero.push_back(Sigma::Face(2,4,11));
	faceLevelZero.push_back(Sigma::Face(6,2,10));
	faceLevelZero.push_back(Sigma::Face(8,6,7));
	faceLevelZero.push_back(Sigma::Face(9,8,1));

	// Refine the IcoSphere by 2 levels.
	// We are using a temp face buffer as it will be replaced by a new set.
	// The vertex buffer is the actual buffer as we will add to it.
	std::cout << "Subdividing icosphere..." << std::endl;
	Refine(this->verts, faceLevelZero, 4);

	// Store the new faces.
	for (auto faceitr = faceLevelZero.begin(); faceitr != faceLevelZero.end(); ++faceitr) {
		AddFace((*faceitr));
	}

	AddMeshGroupIndex(0);

	std::vector<Sigma::Vertex> surfaceNorms;

	// compute surface normals
	for(size_t i = 0; i < GetFaceCount(); i++) {
		glm::vec3 vector1, vector2, cross, normal;
		const Sigma::Face* f = GetFace(i);
		Sigma::Vertex vert1(verts[f->v1]), vert2(verts[f->v2]), vert3(verts[f->v3]);

		vector1 = glm::normalize(glm::vec3(vert2.x-vert1.x, vert2.y-vert1.y, vert2.z-vert1.z));
		vector2 = glm::normalize(glm::vec3(vert3.x-vert1.x, vert3.y-vert1.y, vert3.z-vert1.z));
		cross = glm::cross(vector1, vector2);
		normal = glm::normalize(cross);

		surfaceNorms.push_back(Sigma::Vertex(normal.x, normal.y, normal.z));
	}

	// compute vertex normals
	// should probably compute adjacency first, this could be slow
	for(size_t i = 0; i < verts.size(); i++) {
		Sigma::Vertex total_normals(0.0f, 0.0f, 0.0f);

		for(size_t j = 0; j < GetFaceCount(); j++) {
			const Sigma::Face* f = GetFace(j);
			if (f->v1 == i || f->v2 == i || f->v3 == i) {
				total_normals.x += surfaceNorms[j].x;
				total_normals.y += surfaceNorms[j].y;
				total_normals.z += surfaceNorms[j].z;
			}
		}

		glm::vec3 final_normal(total_normals.x, total_normals.y, total_normals.z);
		final_normal = glm::normalize(final_normal);
		AddVertexNormal(Sigma::Vertex(final_normal.x, final_normal.y, final_normal.z));
	}

	surfaceNorms.clear();
	GLMesh::InitializeBuffers();
}

void GLIcoSphere::LoadShader() {
	GLIcoSphere::shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/icosphere.vert");
	GLIcoSphere::shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/icosphere.frag");
	GLIcoSphere::shader.CreateAndLinkProgram();
}


void GLIcoSphere::Render(glm::mediump_float *view, glm::mediump_float *proj) {
	GLMesh::Render(view, proj);
}

glm::vec3 GetMidPoint(Sigma::Vertex v1, Sigma::Vertex v2) {
	return glm::normalize(glm::vec3((v1.x + v2.x) / 2.0, (v1.y + v2.y) / 2.0, (v1.z + v2.z) / 2.0));
}

void GLIcoSphere::Refine(std::vector<Sigma::Vertex> &verts, std::vector<Sigma::Face> &faces, int level) {
	std::map<int64_t, int> cache;

	// refine faces
	std::vector<Sigma::Face> newFaces = faces;
	for (int i = 0; i < level; ++i) {
		std::vector<Sigma::Face> tempFaces;
		for (auto faceitr = newFaces.begin(); faceitr != newFaces.end(); ++faceitr) {
			int a,b,c;

			{
				int v1 = (*faceitr).v1;
				int v2 = (*faceitr).v2;
				// first check if we have it already
				bool firstIsSmaller = v1 < v2;
				int64_t smallerIndex = firstIsSmaller ? v1 : v2;
				int64_t greaterIndex = firstIsSmaller ? v2 : v1;
				int64_t key = (smallerIndex << 32) + greaterIndex;

				if (cache.find(key) != cache.end()) {
					a = cache[key];
				} else {
					glm::vec3 middle = GetMidPoint(this->verts[v1], this->verts[v2]);

					AddVertex(Sigma::Vertex(middle.x, middle.y, middle.z));
					this->verts.push_back(Sigma::Vertex(middle.x, middle.y, middle.z));
					int blue = 0;
					if (GetVertexColor(v1)->b > 0.0f) {
						blue++;
					}
					if (GetVertexColor(v2)->b > 0.0f) {
						blue++;
					}
					int green = 0;
					if (GetVertexColor(v1)->g > 0.0f) {
						green++;
					}
					if (GetVertexColor(v2)->g > 0.0f) {
						green++;
					}
					
					if (blue == green) {
						int val = rand() % 2;
						if (val > 0) {
							green = 0;
						} else {
							blue = 0;
						}
					} else {
						int val = rand() % 6;
						if (val > 1) {
							blue = 1;
							green = 0;
						} else {
							green = 1;
							blue = 0;
						}
					}

					AddVertexColor(Sigma::Color(0,static_cast<float>(green),static_cast<float>(blue)));

					a = cache[key] = this->verts.size() - 1;
				}
			}
			{
				int v1 = (*faceitr).v2;
				int v2 = (*faceitr).v3;
				// first check if we have it already
				bool firstIsSmaller = v1 < v2;
				int64_t smallerIndex = firstIsSmaller ? v1 : v2;
				int64_t greaterIndex = firstIsSmaller ? v2 : v1;
				int64_t key = (smallerIndex << 32) + greaterIndex;

				if (cache.find(key) != cache.end()) {
					b = cache[key];
				} else {
					glm::vec3 middle = GetMidPoint(this->verts[v1], this->verts[v2]);

					AddVertex(Sigma::Vertex(middle.x, middle.y, middle.z));
					this->verts.push_back(Sigma::Vertex(middle.x, middle.y, middle.z));
					int blue = 0;
					if (GetVertexColor(v1)->b > 0.0f) {
						blue++;
					}
					if (GetVertexColor(v2)->b > 0.0f) {
						blue++;
					}
					int green = 0;
					if (GetVertexColor(v1)->g > 0.0f) {
						green++;
					}
					if (GetVertexColor(v2)->g > 0.0f) {
						green++;
					}

					if (blue == green) {
						int val = rand() % 2;
						if (val > 0) {
							green = 0;
						} else {
							blue = 0;
						}
					} else {
						int val = rand() % 6;
						if (val > 1) {
							blue = 1;
							green = 0;
						} else {
							green = 1;
							blue = 0;
						}
					}
					AddVertexColor(Sigma::Color(0,static_cast<float>(green),static_cast<float>(blue)));

					b = cache[key] = this->verts.size() - 1;
				}
			}
			{
				int v1 = (*faceitr).v3;
				int v2 = (*faceitr).v1;
				// first check if we have it already
				bool firstIsSmaller = v1 < v2;
				int64_t smallerIndex = firstIsSmaller ? v1 : v2;
				int64_t greaterIndex = firstIsSmaller ? v2 : v1;
				int64_t key = (smallerIndex << 32) + greaterIndex;

				if (cache.find(key) != cache.end()) {
					c = cache[key];
				} else {
					glm::vec3 middle = GetMidPoint(this->verts[v1], this->verts[v2]);

					AddVertex(Sigma::Vertex(middle.x, middle.y, middle.z));
					this->verts.push_back(Sigma::Vertex(middle.x, middle.y, middle.z));
					int blue = 0;
					if (GetVertexColor(v1)->b > 0.0f) {
						blue++;
					}
					if (GetVertexColor(v2)->b > 0.0f) {
						blue++;
					}
					int green = 0;
					if (GetVertexColor(v1)->g > 0.0f) {
						green++;
					}
					if (GetVertexColor(v2)->g > 0.0f) {
						green++;
					}

					if (blue == green) {
						int val = rand() % 2;
						if (val > 0) {
							green = 0;
						} else {
							blue = 0;
						}
					} else {
						int val = rand() % 6;
						if (val > 1) {
							blue = 1;
							green = 0;
						} else {
							green = 1;
							blue = 0;
						}
					}
					AddVertexColor(Sigma::Color(0,static_cast<float>(green),static_cast<float>(blue)));

					c = cache[key] = this->verts.size() - 1;
				}
			}

			tempFaces.push_back(Sigma::Face((*faceitr).v1, a, c));
			tempFaces.push_back(Sigma::Face((*faceitr).v2, b, a));
			tempFaces.push_back(Sigma::Face((*faceitr).v3, c, b));
			tempFaces.push_back(Sigma::Face(a, b, c));
		}
		newFaces = tempFaces;
	}
	faces = newFaces;
}

//void GLIcoSphere::RefineFace(const unsigned int index) {
//	std::vector<Sigma::Face> face;
//	face.push_back(*GetFace(index));
//
//	Refine(this->verts, face, 1);
//	RemoveFace(index);
//	// Store the new faces.
//	for (auto faceitr = face.begin(); faceitr != face.end(); ++faceitr) {
//		AddFace((*faceitr));
//	}
//
//	std::vector<Sigma::Vertex> surfaceNorms;
//
//	// compute surface normals
//	for(size_t i = 0; i < 4; i++) {
//		glm::vec3 vector1, vector2, cross, normal;
//		Sigma::Face f = face[i];
//		Sigma::Vertex vert1(verts[f.v1]), vert2(verts[f.v2]), vert3(verts[f.v3]);
//
//		vector1 = glm::normalize(glm::vec3(vert2.x-vert1.x, vert2.y-vert1.y, vert2.z-vert1.z));
//		vector2 = glm::normalize(glm::vec3(vert3.x-vert1.x, vert3.y-vert1.y, vert3.z-vert1.z));
//		cross = glm::cross(vector1, vector2);
//		normal = glm::normalize(cross);
//
//		surfaceNorms.push_back(Sigma::Vertex(normal.x, normal.y, normal.z));
//	}
//
//	// compute vertex normals
//	// should probably compute adjacency first, this could be slow
//	for(size_t i = 0; i < verts.size(); i++) {
//		Sigma::Vertex total_normals(0.0f, 0.0f, 0.0f);
//
//		for(size_t j = 0; j < 4; j++) {
//			Sigma::Face f = face[j];
//			if (f.v1 == i || f.v2 == i || f.v3 == i) {
//				total_normals.x += surfaceNorms[j].x;
//				total_normals.y += surfaceNorms[j].y;
//				total_normals.z += surfaceNorms[j].z;
//			}
//		}
//
//		glm::vec3 final_normal(total_normals.x, total_normals.y, total_normals.z);
//		final_normal = glm::normalize(final_normal);
//		AddVertexNormal(Sigma::Vertex(final_normal.x, final_normal.y, final_normal.z));
//	}
//	GLMesh::InitializeBuffers();
//}

GLSLShader GLIcoSphere::shader;
