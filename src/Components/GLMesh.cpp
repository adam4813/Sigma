#include "GLMesh.h"
#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "GLIcoSphere.h"

GLMesh::GLMesh(const int entityID) : IGLComponent(entityID) {
	this->drawMode = GL_TRIANGLES;
	this->ElemBufIndex = 2;
	this->ColorBufIndex = 1;
	this->VertBufIndex = 0;
}

void GLMesh::Initialize() {
	// We must create a vao and then store it in our GLIcoSphere.
	glGenVertexArrays(1, &this->vao); // Generate the VAO
	glBindVertexArray(this->vao); // Bind the VAO

	glGenBuffers(1, &this->buffers[this->VertBufIndex]); 	// Generate the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]); // Bind the vertex buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * this->verts.size(), &this->verts.front(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
	GLint posLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Position"); // Find the location in the shader where the vertex buffer data will be placed.
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // Tell the VAO the vertex data will be stored at the location we just found.
	glEnableVertexAttribArray(posLocation); // Enable the VAO line for vertex data.

	glGenBuffers(1, &this->buffers[this->ColorBufIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->ColorBufIndex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color) * this->colors.size(), &this->colors.front(), GL_STATIC_DRAW);
	GLint colLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Color");
	glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colLocation);

	glGenBuffers(1, &this->buffers[this->ElemBufIndex]); // Generate the element buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]); // Bind the element buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face) * this->faces.size(), &this->faces.front(), GL_STATIC_DRAW); // Store the faces in the element buffer.

	glBindVertexArray(0); // Reset the buffer binding because we are good programmers.
}

void GLMesh::LoadMesh(std::string fname) {
	std::ifstream in(fname, std::ios::in);
	if (!in) {
		std::cerr << "Cannot open " << fname << std::endl;
		return;
	}

	std::string line;
	while (getline(in, line)) {
		if (line.substr(0,2) == "v ") {
			std::istringstream s(line.substr(2));
			float x,y,z;
			s >> x; s >> y; s >> z;
			this->verts.push_back(vertex(x, y, z));
			this->colors.push_back(color(0.5 - (1.0f / x), 0.5 - (1.0f / y), 0.5 - (1.0f / z)));
		}  else if (line.substr(0,2) == "f ") {
			GLushort a,b,c;
			std::string cur = line.substr(2, line.find(' ', 2) - 2);
			std::string left = line.substr(line.find(' ', 2) + 1);
			a = atoi(cur.substr(0, cur.find('/')).c_str());
			cur = left.substr(0, left.find(' '));
			left = left.substr(left.find(' ') + 1);
			b = atoi(cur.substr(0, cur.find('/')).c_str());
			cur = left.substr(0, left.find(' '));
			left = left.substr(left.find(' '));
			c = atoi(cur.substr(0, cur.find('/')).c_str());
			a--; b--; c--;
			this->faces.push_back(face(a,b,c));
		} else if (line[0] == 'g') {
			this->groupIndex.push_back(this->faces.size());
		} else if (line[0] == '#') {
			/* ignoring this line */
		} else {
			/* ignoring this line */
			std::cerr << "Unrecognized line " << line << std::endl;
		}
	}

	/*normals.resize(mesh->vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (int i = 0; i < elements.size(); i+=3) {
		GLushort ia = elements[i];
		GLushort ib = elements[i+1];
		GLushort ic = elements[i+2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
			glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
		normals[ia] = normals[ib] = normals[ic] = normal;
	}*/
}
