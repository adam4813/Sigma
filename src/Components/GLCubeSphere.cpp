#include <vector>
#include <algorithm>

#include "sdl.h"
#include "sdl_image.h"

#include "GLCubeSphere.h"

GLSLShader GLCubeSphere::shader;

// For std::find
namespace Sigma {
	bool operator ==(const Vertex &lhs, const Vertex &rhs) { return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z); }
}

GLCubeSphere::GLCubeSphere( const int entityID /*= 0*/ ) : IGLComponent(entityID) {
	this->drawMode = GL_TRIANGLES;
	this->VertBufIndex = 0;
	this->ColorBufIndex = 1;
	this->ElemBufIndex = 2;
}

void GLCubeSphere::Initialize() {
	srand(this->GetEntityID());

	int subdivisions = 5;

	// Create the verts to begin refining at.
	float t = 1.0f;
	glm::vec3 coordPair(t, t, t);

	this->verts.push_back(Sigma::Vertex(-coordPair.x, -coordPair.y, coordPair.z));
	this->verts.push_back(Sigma::Vertex(coordPair.x, -coordPair.y, coordPair.z));
	this->verts.push_back(Sigma::Vertex(coordPair.x, coordPair.y, coordPair.z));
	this->verts.push_back(Sigma::Vertex(-coordPair.x, coordPair.y, coordPair.z));
	this->verts.push_back(Sigma::Vertex(-coordPair.x, -coordPair.y, -coordPair.z));
	this->verts.push_back(Sigma::Vertex(coordPair.x, -coordPair.y, -coordPair.z));
	this->verts.push_back(Sigma::Vertex(coordPair.x, coordPair.y, -coordPair.z));
	this->verts.push_back(Sigma::Vertex(-coordPair.x, coordPair.y, -coordPair.z));

	// front
	this->faces.push_back(Sigma::Face(0,1,2));
	this->faces.push_back(Sigma::Face(2,3,0));
	// top
    this->faces.push_back(Sigma::Face(3,2,6));
    this->faces.push_back(Sigma::Face(6, 7, 3));
    // back
    this->faces.push_back(Sigma::Face(7, 6, 5));
    this->faces.push_back(Sigma::Face(5, 4, 7));
    // bottom
    this->faces.push_back(Sigma::Face(4, 5, 1));
    this->faces.push_back(Sigma::Face(1, 0, 4));
    // left
    this->faces.push_back(Sigma::Face(4, 0, 3));
    this->faces.push_back(Sigma::Face(3, 7, 4));
    // right
    this->faces.push_back(Sigma::Face(1, 5, 6));
    this->faces.push_back(Sigma::Face(6, 2, 1));

	this->SubDivide(subdivisions);

	glGenVertexArrays(1, &this->vao); // Generate the VAO
	glBindVertexArray(this->vao); // Bind the VAO

	glGenBuffers(1, &this->buffers[this->VertBufIndex]); 	// Generate the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]); // Bind the vertex buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sigma::Vertex) * this->verts.size(), &this->verts.front(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
	GLint posLocation = glGetAttribLocation(GLCubeSphere::shader.GetProgram(), "in_Position"); // Find the location in the shader where the vertex buffer data will be placed.
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // Tell the VAO the vertex data will be stored at the location we just found.
	glEnableVertexAttribArray(posLocation); // Enable the VAO line for vertex data.

	glGenBuffers(1, &this->buffers[this->ElemBufIndex]); // Generate the element buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]); // Bind the element buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Sigma::Face) * this->faces.size(), &this->faces.front(), GL_STATIC_DRAW); // Store the faces in the element buffer.

	glBindVertexArray(0); // Reset the buffer binding because we are good programmers.

	// albedo map
	glGenTextures(1, &this->_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->_cubeMap);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

	// There is always six filenames
	for(int i=0; i < 6; i++) {
		char filename[100];
		sprintf_s(filename, "mars%d.jpg", i+1);
		SDL_Surface *img;
		img = IMG_Load(filename);

		if(img) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,img->w,img->h,0,GL_RGB,GL_UNSIGNED_BYTE,(img->pixels));
			SDL_FreeSurface(img);
		} else {
			assert(0 && "Texture file did not load correctly.");
		}
	}
	
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void GLCubeSphere::SubDivide(int levels) {
	std::vector<Sigma::Face> newFaces;

	// Iterate over each face and subdivide it
	for(std::vector<Sigma::Face>::iterator i = this->faces.begin(); i != this->faces.end(); ++i) {
		Sigma::Vertex v1(0, 0, 0), v2(0, 0, 0), newVert(0, 0, 0);
		Sigma::Face newFace(0, 0, 0);

		short i1, i2, i3;

		// Split each edge
		v1 = this->verts[(*i).v1];
		v2 = this->verts[(*i).v2];

		newVert.x = (v1.x + v2.x)/2.0f;
		newVert.y = (v1.y + v2.y)/2.0f;
		newVert.z = (v1.z + v2.z)/2.0f;

		// See if vertex already exists
		std::vector<Sigma::Vertex>::iterator existingVert = std::find(this->verts.begin(), this->verts.end(), newVert);
		if(existingVert==this->verts.end()) {
			i1 = verts.size();
			this->verts.push_back(newVert);
		}
		else {
			i1 = std::distance(this->verts.begin(), existingVert);
		}

		v1 = this->verts[(*i).v1];
		v2 = this->verts[(*i).v3];

		newVert.x = (v1.x + v2.x)/2.0f;
		newVert.y = (v1.y + v2.y)/2.0f;
		newVert.z = (v1.z + v2.z)/2.0f;

		existingVert = std::find(this->verts.begin(), this->verts.end(), newVert);
		if(existingVert==this->verts.end()) {
			i2 = this->verts.size();
			this->verts.push_back(newVert);
		}
		else {
			i2 = std::distance(this->verts.begin(), existingVert);
		}

		v1 = this->verts[(*i).v2];
		v2 = this->verts[(*i).v3];

		newVert.x = (v1.x + v2.x)/2.0f;
		newVert.y = (v1.y + v2.y)/2.0f;
		newVert.z = (v1.z + v2.z)/2.0f;

		existingVert = std::find(this->verts.begin(), this->verts.end(), newVert);
		if(existingVert==verts.end()) {
			i3 = this->verts.size();
			this->verts.push_back(newVert);
		}
		else {
			i3 = std::distance(this->verts.begin(), existingVert);
		}

		newFace.v1 = (*i).v1;
		newFace.v2 = i1;
		newFace.v3 = i2;
		newFaces.push_back(newFace);

		newFace.v1 = i1;
		newFace.v2 = (*i).v2;
		newFace.v3 = i3;
		newFaces.push_back(newFace);

		newFace.v1 = i3;
		newFace.v2 = i2;
		newFace.v3 = i1;
		newFaces.push_back(newFace);

		newFace.v1 = i2;
		newFace.v2 = i3;
		newFace.v3 = (*i).v3;
		newFaces.push_back(newFace);
	}

	this->faces = newFaces;

	if(levels > 1) {
		this->SubDivide(levels-1);
	}
}

void GLCubeSphere::LoadShader() {
	GLCubeSphere::shader.LoadFromFile(GL_VERTEX_SHADER, "..\\..\\shaders\\cubesphere.vert");
	GLCubeSphere::shader.LoadFromFile(GL_FRAGMENT_SHADER, "..\\..\\shaders\\cubesphere.frag");
	GLCubeSphere::shader.CreateAndLinkProgram();
}

void GLCubeSphere::Update(glm::mediump_float *view, glm::mediump_float *proj) {
	GLCubeSphere::shader.Use();
	
	this->Transform().Rotate(0.0f,0.01f,0.0f);

	glUniform1i(glGetUniformLocation(GLCubeSphere::shader.GetProgram(), "cubeMap"), 0);

	glUniformMatrix4fv(glGetUniformLocation(GLCubeSphere::shader.GetProgram(), "in_Model"), 1, GL_FALSE, &this->Transform().ModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(GLCubeSphere::shader.GetProgram(), "in_View"), 1, GL_FALSE, view);
	glUniformMatrix4fv(glGetUniformLocation(GLCubeSphere::shader.GetProgram(), "in_Proj"), 1, GL_FALSE, proj);

	glBindVertexArray(this->Vao());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->GetBuffer(this->ElemBufIndex));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->_cubeMap);

	for (int i = 0, cur = this->MeshGroup_ElementCount(0), prev = 0; cur != 0; prev = cur, cur = this->MeshGroup_ElementCount(++i)) {
		glDrawElements(this->DrawMode(), cur, GL_UNSIGNED_SHORT, (void*)prev);
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	GLCubeSphere::shader.UnUse();
}