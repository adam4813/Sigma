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
	this->NormalBufIndex = 3;
}

void GLMesh::Initialize() {
	// We must create a vao and then store it in our GLIcoSphere.
	glGenVertexArrays(1, &this->vao); // Generate the VAO
	glBindVertexArray(this->vao); // Bind the VAO

	if (this->verts.size() > 0) {
		glGenBuffers(1, &this->buffers[this->VertBufIndex]); 	// Generate the vertex buffer.
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]); // Bind the vertex buffer.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * this->verts.size(), &this->verts.front(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
		GLint posLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Position"); // Find the location in the shader where the vertex buffer data will be placed.
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // Tell the VAO the vertex data will be stored at the location we just found.
		glEnableVertexAttribArray(posLocation); // Enable the VAO line for vertex data.
	}
	if (this->colors.size() > 0) {
		glGenBuffers(1, &this->buffers[this->ColorBufIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->ColorBufIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color) * this->colors.size(), &this->colors.front(), GL_STATIC_DRAW);
		GLint colLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Color");
		glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(colLocation);
	}
	if (this->faces.size() > 0) {
		glGenBuffers(1, &this->buffers[this->ElemBufIndex]); // Generate the element buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]); // Bind the element buffer.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face) * this->faces.size(), &this->faces.front(), GL_STATIC_DRAW); // Store the faces in the element buffer.
	}
	if (this->vertNorms.size() > 0) {
		glGenBuffers(1, &this->buffers[this->NormalBufIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->NormalBufIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*this->vertNorms.size(), &this->vertNorms[0], GL_STATIC_DRAW);
		GLint normalLocation = glGetAttribLocation(GLIcoSphere::shader.GetProgram(), "in_Normal");
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(normalLocation);
	}

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
		if (line.substr(0,2) == "v ") { // Vertex position
			float x,y,z;
			std::istringstream s(line.substr(2));
			s >> x; s >> y; s >> z;
			this->verts.push_back(vertex(x, y, z));
		}  else if (line.substr(0,2) == "f ") { // Face
			short indicies[3][3];
			std::string cur = line.substr(2, line.find(' ', 2) - 2);
			std::string left = line.substr(line.find(' ', 2) + 1);

			for (int i = 0; i < 3; ++i) { // Each face contains 3 sets of indicies. Each set is 3 indicies v/t/n.
				std::string first = cur.substr(0, cur.find('/')); // Substring for the v portion
				indicies[i][0] = atoi(first.c_str());
				if ((cur.find('/') + 1) != cur.find('/', cur.find('/') + 1)) { // Check if we have a t portion
					std::string second = cur.substr(cur.find('/') + 1, cur.find('/', cur.find('/') + 1)); // Substring for the t portion
					indicies[i][1] = atoi(second.c_str());
				} else {
					indicies[i][1] = 0;
				}
				if (cur.find('/', cur.find('/')) != cur.find_last_of('/')) { // Check if we have an n portion
					std::string third = cur.substr(cur.find_last_of('/') + 1); // Substring for the n portion
					indicies[i][2] = atoi(third.c_str());
				} else {
					indicies[i][2] = 0;
				}
				cur = left.substr(0, left.find(' '));
				left = left.substr(left.find(' ') + 1);
			}
			GLushort a,b,c;
			a = indicies[0][0]; b = indicies[1][0]; c = indicies[2][0];
			a--; b--; c--;
			GLushort ta,tb,tc;
			ta = indicies[0][1]; tb = indicies[1][1]; tc = indicies[2][1];
			ta--; tb--; tc--;
			GLushort na,nb,nc;
			na = indicies[0][2]; nb = indicies[1][2]; nc = indicies[2][2];
			na--; nb--; nc--;
			this->faces.push_back(face(a,b,c));
			this->texFaces.push_back(face(ta,tb,tc));
			this->faceNorms.push_back(face(na,nb,nc));
		}  else if (line.substr(0,2) == "vt") { //  Vertex tex coord
			float u, v = 0.0f;
			std::istringstream s(line.substr(3));
			s >> u; s >> v;
			this->texCoords.push_back(texCoord(u,v));
		}  else if (line.substr(0,2) == "vn") { // Vertex normal
			float i, j, k;
			std::istringstream s(line.substr(3));
			s >> i; s >> j; s >> k;
			this->vertNorms.push_back(vertex(i,j,k));
		} else if (line[0] == 'g') { // Face group
			this->groupIndex.push_back(this->faces.size());
		} else if ((line[0] == '#') || (line.size() == 0)) { // Comment or blank line
			/* ignoring this line comment or blank*/
		} else { // Unknown
			/* ignoring this line */
			std::cerr << "Unrecognized line " << line << std::endl;
		}
	}

	// Check if vertex normals exist
	if(vertNorms.size() == 0) {
		std::vector<vertex> surfaceNorms;

		// compute surface normals
		for(size_t i = 0; i < faces.size(); i++) {
			glm::vec3 vector1, vector2, cross, normal;
			vertex vert1(verts[faces[i].v1]), vert2(verts[faces[i].v2]), vert3(verts[faces[i].v3]);

			vector1 = glm::normalize(glm::vec3(vert2.x-vert1.x, vert2.y-vert1.y, vert2.z-vert1.z));
			vector2 = glm::normalize(glm::vec3(vert3.x-vert1.x, vert3.y-vert1.y, vert3.z-vert1.z));
			cross = glm::cross(vector1, vector2);
			normal = glm::normalize(cross);

			surfaceNorms.push_back(vertex(normal.x, normal.y, normal.z));
		}

		// compute vertex normals
		// should probably compute adjacency first, this could be slow
		for(size_t i = 0; i < verts.size(); i++) {
			vertex total_normals(0.0f, 0.0f, 0.0f);

			for(size_t j = 0; j < faces.size(); j++) {
				if (faces[j].v1 == i || faces[j].v2 == i || faces[j].v3 == i) {
					total_normals.x += surfaceNorms[j].x;
					total_normals.y += surfaceNorms[j].y;
					total_normals.z += surfaceNorms[j].z;
				}
			}

			glm::vec3 final_normal(total_normals.x, total_normals.y, total_normals.z);
			final_normal = glm::normalize(final_normal);
			vertNorms.push_back(vertex(final_normal.x, final_normal.y, final_normal.z));
			std::cout << vertNorms[i].x << " " << vertNorms[i].y << " " << vertNorms[i].z << std::endl;
		}

		surfaceNorms.clear();
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
