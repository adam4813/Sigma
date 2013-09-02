#include "GLIcoSphere.h"
#include "GL/glew.h"
#include <map>
#include <stdint.h>

GLIcoSphere::GLIcoSphere( const int entityID /*= 0*/ ) : IGLComponent(entityID), mesh(entityID) {
	this->drawMode = GL_TRIANGLES;
	this->ElemBufIndex = 2;
	this->ColorBufIndex = 1;
	this->VertBufIndex = 0;
	this->NormalBufIndex = 3;
}

void GLIcoSphere::Initialize() {
	srand(this->GetEntityID());
	// Create the verts to begin refining at.
	double t = (1.0 + glm::sqrt(5.0)) / 2.0;
	glm::vec2 coordPair = glm::normalize(glm::vec2(1,t));
	this->mesh.AddVertex(Sigma::Vertex(-coordPair.r, coordPair.g, 0));
	this->mesh.AddVertex(Sigma::Vertex(coordPair.r, coordPair.g, 0));
	this->mesh.AddVertex(Sigma::Vertex(-coordPair.r, -coordPair.g, 0));
	this->mesh.AddVertex(Sigma::Vertex(coordPair.r, -coordPair.g, 0));

	this->mesh.AddVertex(Sigma::Vertex(0, -coordPair.r, coordPair.g));
	this->mesh.AddVertex(Sigma::Vertex(0, coordPair.r, coordPair.g));
	this->mesh.AddVertex(Sigma::Vertex(0, -coordPair.r, -coordPair.g));
	this->mesh.AddVertex(Sigma::Vertex(0, coordPair.r, -coordPair.g));

	this->mesh.AddVertex(Sigma::Vertex(coordPair.r, 0, -coordPair.g));
	this->mesh.AddVertex(Sigma::Vertex(coordPair.r, 0, coordPair.g));
	this->mesh.AddVertex(Sigma::Vertex(-coordPair.r, 0, -coordPair.g));
	this->mesh.AddVertex(Sigma::Vertex(-coordPair.r, 0, coordPair.g));


	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->mesh.AddVertexColor((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

	this->verts.push_back(Sigma::Vertex(-coordPair.r, coordPair.g, 0));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(coordPair.r, coordPair.g, 0));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(-coordPair.r, -coordPair.g, 0));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(coordPair.r, -coordPair.g, 0));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

	this->verts.push_back(Sigma::Vertex(0, -coordPair.r, coordPair.g));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(0, coordPair.r, coordPair.g));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(0, -coordPair.r, -coordPair.g));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(0, coordPair.r, -coordPair.g));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

	this->verts.push_back(Sigma::Vertex(coordPair.g, 0, -coordPair.r));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(coordPair.g, 0, coordPair.r));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(-coordPair.g, 0, -coordPair.r));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));
	this->verts.push_back(Sigma::Vertex(-coordPair.g, 0, coordPair.r));
	this->colors.push_back((rand() % 6) > 0 ? Sigma::Color(0,0,1) : Sigma::Color(0,1,0));

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
	Refine(this->verts, faceLevelZero, this->GetEntityID());

	// Store the new faces.
	this->faces = faceLevelZero;
	for (auto faceitr = faceLevelZero.begin(); faceitr != faceLevelZero.end(); ++faceitr) {
		this->mesh.AddFace((*faceitr));
	}

	this->mesh.AddMeshGroupIndex(0);

	std::vector<Sigma::Vertex> surfaceNorms;

	// compute surface normals
	for(size_t i = 0; i < faces.size(); i++) {
		glm::vec3 vector1, vector2, cross, normal;
		Sigma::Vertex vert1(verts[faces[i].v1]), vert2(verts[faces[i].v2]), vert3(verts[faces[i].v3]);

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

		for(size_t j = 0; j < faces.size(); j++) {
			if (faces[j].v1 == i || faces[j].v2 == i || faces[j].v3 == i) {
				total_normals.x += surfaceNorms[j].x;
				total_normals.y += surfaceNorms[j].y;
				total_normals.z += surfaceNorms[j].z;
			}
		}

		glm::vec3 final_normal(total_normals.x, total_normals.y, total_normals.z);
		final_normal = glm::normalize(final_normal);
		vertNorms.push_back(Sigma::Vertex(final_normal.x, final_normal.y, final_normal.z));
		this->mesh.AddVertexNormal(Sigma::Vertex(final_normal.x, final_normal.y, final_normal.z));
	}

	surfaceNorms.clear();
	this->mesh.Initialize();

	// We must create a vao and then store it in our GLIcoSphere.
	glGenVertexArrays(1, &this->vao); // Generate the VAO
	glBindVertexArray(this->vao); // Bind the VAO

	glGenBuffers(1, &this->buffers[this->VertBufIndex]); 	// Generate the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]); // Bind the vertex buffer.
	size_t size = sizeof(Sigma::Vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sigma::Vertex) * this->verts.size(), &this->verts.front(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
	GLint posLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Position"); // Find the location in the shader where the vertex buffer data will be placed.
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Sigma::Vertex), 0); // Tell the VAO the vertex data will be stored at the location we just found.
	glEnableVertexAttribArray(posLocation); // Enable the VAO line for vertex data.


	glGenBuffers(1, &this->buffers[this->ColorBufIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->ColorBufIndex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sigma::Color) * this->colors.size(), &this->colors.front(), GL_STATIC_DRAW);
	GLint colLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Color");
	glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colLocation);

	glGenBuffers(1, &this->buffers[this->ElemBufIndex]); // Generate the element buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]); // Bind the element buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Sigma::Face) * this->faces.size(), &this->faces.front(), GL_STATIC_DRAW); // Store the faces in the element buffer.

	if (this->vertNorms.size() > 0) {
		glGenBuffers(1, &this->buffers[this->NormalBufIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->NormalBufIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Sigma::Vertex)*this->vertNorms.size(), &this->vertNorms[0], GL_STATIC_DRAW);
		GLint normalLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Normal");
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(normalLocation);
	}

	glBindVertexArray(0); // Reset the buffer binding because we are good programmers.
}

void GLIcoSphere::LoadShader() {
	GLIcoSphere::shader.LoadFromFile(GL_VERTEX_SHADER, "..\\..\\shaders\\icosphere.vert");
	GLIcoSphere::shader.LoadFromFile(GL_FRAGMENT_SHADER, "..\\..\\shaders\\icosphere.frag");
	GLIcoSphere::shader.CreateAndLinkProgram();
}


void GLIcoSphere::Update(glm::mediump_float *view, glm::mediump_float *proj) {
	this->mesh.Update(view, proj);
	GLIcoSphere::shader.Use();
	this->Transform().Rotate(0.0f,0.1f,0.0f);
	glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Model"), 1, GL_FALSE, &this->Transform().ModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_View"), 1, GL_FALSE, view);
	glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Proj"), 1, GL_FALSE, proj);
	glBindVertexArray(this->Vao());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->GetBuffer(this->ElemBufIndex));
	for (int i = 0, cur = this->MeshGroup_ElementCount(0), prev = 0; cur != 0; prev = cur, cur = this->MeshGroup_ElementCount(++i)) {
		glDrawElements(this->DrawMode(), cur, GL_UNSIGNED_SHORT, (void*)prev);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	GLIcoSphere::shader.UnUse();
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

					this->mesh.AddVertex(Sigma::Vertex(middle.x, middle.y, middle.z));
					verts.push_back(Sigma::Vertex(middle.x, middle.y, middle.z));
					int blue = this->colors[v1].b + this->colors[v2].b;
					int green = this->colors[v1].g + this->colors[v2].g;
					
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
					this->colors.push_back(Sigma::Color(0,green,blue));
					this->mesh.AddVertexColor(Sigma::Color(0,green,blue));

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

					this->mesh.AddVertex(Sigma::Vertex(middle.x, middle.y, middle.z));
					verts.push_back(Sigma::Vertex(middle.x, middle.y, middle.z));
					int blue = this->colors[v1].b + this->colors[v2].b;
					int green = this->colors[v1].g + this->colors[v2].g;

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
					this->colors.push_back(Sigma::Color(0,green,blue));
					this->mesh.AddVertexColor(Sigma::Color(0,green,blue));

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

					this->mesh.AddVertex(Sigma::Vertex(middle.x, middle.y, middle.z));
					verts.push_back(Sigma::Vertex(middle.x, middle.y, middle.z));
					int blue = this->colors[v1].b + this->colors[v2].b;
					int green = this->colors[v1].g + this->colors[v2].g;

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
					this->colors.push_back(Sigma::Color(0,green,blue));
					this->mesh.AddVertexColor(Sigma::Color(0,green,blue));

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

GLSLShader GLIcoSphere::shader;
