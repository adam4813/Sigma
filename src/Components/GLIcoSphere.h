#pragma once

#include "../IComponent.h"
#include "../Systems/GLSLShader.h"
#include <vector>
#include "../GLTransform.h"

struct face {
	face(unsigned short v1, unsigned short v2, unsigned short v3) : v1(v1), v2(v2), v3(v3) { }
	unsigned short v1, v2, v3;
};

struct vertex {
	vertex(float x, float y, float z) : x(x), y(y), z(z) { }
	float x,y,z;
};

class GLIcoSphere : public IComponent {
private:
	// We have a private ctor so the factory method must be used.
	GLIcoSphere(const int entityID = 0);

public:
	/**
	 * \brief Creates a new GLSprite
	 *
	 * This is the factory method to create a new GLIcoSphere. This method creates all the required buffers and fills them.
	 * \param[in] int entityID The entity this component belongs to
	 * \returns   GLIcoSphere* The newly creates GLIcoSphere
	 */
	static GLIcoSphere* Factory(int entityID);

	void Refine(std::vector<vertex> &verts, std::vector<face> &faces, int level);

	// The shader used for rendering GLIcoSphere.
	static void LoadShader();
	static GLSLShader shader;

	unsigned int VertBuf() const;
	void VertBuf(unsigned int val);
	unsigned int ColBuf() const;
	void ColBuf(unsigned int val);
	unsigned int Vao() const;
	void Vao(unsigned int val);
	unsigned int ElemBuf() const;
	void ElemBuf(unsigned int val);
	unsigned int NumberElements() const { return this->faces.size() * 3; }

	GLTransform& Transform() { return transform; }
	void Transform(GLTransform val) { transform = val; }
private:
	unsigned int vertBuf;    // will point to where GL put the buffer
	unsigned int colBuf;    // will point to where GL put the buffer
	unsigned int elemBuf;
	unsigned int vao;
	GLTransform transform;

	std::vector<face> faces;
	std::vector<vertex> verts;
};
