#include "GLIcoSphere.h"
#include "GL/glew.h"
#include <map>
#include <stdint.h>

GLIcoSphere::GLIcoSphere( const int entityID /*= 0*/ ) : IComponent(entityID) { }

unsigned int GLIcoSphere::VertBuf() const {
	return this->vertBuf;
}

void GLIcoSphere::VertBuf(unsigned int vb) {
	this->vertBuf = vb;
}

unsigned int GLIcoSphere::ColBuf() const {
	return this->colBuf;
}

void GLIcoSphere::ColBuf(unsigned int cb) {
	this->colBuf = cb;
}

unsigned int GLIcoSphere::Vao() const {
	return this->vao;
}

void GLIcoSphere::Vao(unsigned int v) {
	this->vao = v;
}

unsigned int GLIcoSphere::ElemBuf() const {
	return this->elemBuf;
}

void GLIcoSphere::ElemBuf(unsigned int eb) {
	this->elemBuf = eb;
}

GLIcoSphere* GLIcoSphere::Factory(int entityID) {
	GLIcoSphere* sphere = new GLIcoSphere(entityID);

	// Create the verts to begin refining at.
	double t = (1.0 + glm::sqrt(5.0)) / 2.0;
	glm::vec2 coordPair = glm::normalize(glm::vec2(1,t));

	sphere->verts.push_back(vertex(-coordPair.r, coordPair.g, 0));
	sphere->verts.push_back(vertex(coordPair.r, coordPair.g, 0));
	sphere->verts.push_back(vertex(-coordPair.r, -coordPair.g, 0));
	sphere->verts.push_back(vertex(coordPair.r, -coordPair.g, 0));

	sphere->verts.push_back(vertex(0, -coordPair.r, coordPair.g));
	sphere->verts.push_back(vertex(0, coordPair.r, coordPair.g));
	sphere->verts.push_back(vertex(0, -coordPair.r, -coordPair.g));
	sphere->verts.push_back(vertex(0, coordPair.r, -coordPair.g));

	sphere->verts.push_back(vertex(coordPair.g, 0, -coordPair.r));
	sphere->verts.push_back(vertex(coordPair.g, 0, coordPair.r));
	sphere->verts.push_back(vertex(-coordPair.g, 0, -coordPair.r));
	sphere->verts.push_back(vertex(-coordPair.g, 0, coordPair.r));

	// Create the faces to begin refining at.
	std::vector<face> faceLevelZero;

	faceLevelZero.push_back(face(0,11,5));
	faceLevelZero.push_back(face(0,5,1));
	faceLevelZero.push_back(face(0,1,7));
	faceLevelZero.push_back(face(0,7,10));
	faceLevelZero.push_back(face(0,10,11));

	faceLevelZero.push_back(face(1,5,9));
	faceLevelZero.push_back(face(5,11,4));
	faceLevelZero.push_back(face(11,10,2));
	faceLevelZero.push_back(face(10,7,6));
	faceLevelZero.push_back(face(7,1,8));

	faceLevelZero.push_back(face(3,9,4));
	faceLevelZero.push_back(face(3,4,2));
	faceLevelZero.push_back(face(3,2,6));
	faceLevelZero.push_back(face(3,6,8));
	faceLevelZero.push_back(face(3,8,9));

	faceLevelZero.push_back(face(4,9,5));
	faceLevelZero.push_back(face(2,4,11));
	faceLevelZero.push_back(face(6,2,10));
	faceLevelZero.push_back(face(8,6,7));
	faceLevelZero.push_back(face(9,8,1));

	// Refine the IcoSphere by 2 levels.
	// We are using a temp face buffer as it will be replaced by a new set.
	// The vertex buffer is the actual buffer as we will add to it.
	sphere->Refine(sphere->verts, faceLevelZero, 2);

	// Store the new faces.
	sphere->faces = faceLevelZero;

	// We must create a vao and then store it in our GLIcoSphere.
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID); // Generate the VAO
	glBindVertexArray(vaoID); // Bind the VAO

	GLuint vertbuf;
	glGenBuffers(1, &vertbuf); 	// Generate the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertbuf); // Bind the vertex buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * sphere->verts.size(), &sphere->verts.front(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
	GLint posLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Position"); // Find the location in the shader where the vertex buffer data will be placed.
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // Tell the VAO the vertex data will be stored at the location we just found.
	glEnableVertexAttribArray(posLocation); // Enable the VAO line for vertex data.

	GLuint elembuf;
	glGenBuffers(1, &elembuf); // Generate the element buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elembuf); // Bind the element buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face) * sphere->faces.size(), &sphere->faces.front(), GL_STATIC_DRAW); // Store the faces in the element buffer.

	glBindVertexArray(0); // Reset the buffer binding because we are good programmers.

	// Store the buffers in our component.
	sphere->Vao(vaoID);
	sphere->VertBuf(vertbuf);
	sphere->ElemBuf(elembuf);

	return sphere;
}

void GLIcoSphere::LoadShader() {
	GLIcoSphere::shader.LoadFromFile(GL_VERTEX_SHADER, "..\\..\\shaders\\vert.shade");
	GLIcoSphere::shader.LoadFromFile(GL_FRAGMENT_SHADER, "..\\..\\shaders\\frag.shade");
	GLIcoSphere::shader.CreateAndLinkProgram();
}

glm::vec3 GetMidPoint(vertex v1, vertex v2) {
	return glm::normalize(glm::vec3((v1.x + v2.x) / 2.0, (v1.y + v2.y) / 2.0, (v1.z + v2.z) / 2.0));
}

void GLIcoSphere::Refine(std::vector<vertex> &verts, std::vector<face> &faces, int level) {
	std::map<int64_t, int> cache;

	// refine faces
	std::vector<face> newFaces = faces;
	for (int i = 0; i < level; ++i) {
		std::vector<face> tempFaces;
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

					verts.push_back(vertex(middle.x, middle.y, middle.z));

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

					verts.push_back(vertex(middle.x, middle.y, middle.z));

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

					verts.push_back(vertex(middle.x, middle.y, middle.z));

					c = cache[key] = this->verts.size() - 1;
				}
			}

			tempFaces.push_back(face((*faceitr).v1, a, c));
			tempFaces.push_back(face((*faceitr).v2, b, a));
			tempFaces.push_back(face((*faceitr).v3, c, b));
			tempFaces.push_back(face(a, b, c));
		}
		newFaces = tempFaces;
	}
	faces = newFaces;
}

GLSLShader GLIcoSphere::shader;
