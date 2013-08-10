#pragma once

#include "../IComponent.h"
#include "../Systems/GLSLShader.h"

class GLSprite : public IComponent {
private:
	// We have a private ctor so the factory method must be used.
	GLSprite(const int entityID = 0);

public:
	/**
	 * \brief Creates a new GLSprite
	 *
	 * This is the factory method to create a new GLSprite. This method creates all the required buffers and fills them.
	 * \param[in] int entityID The entity this component belongs to
	 * \returns   GLSprite* The newly creates GLSprite
	 */
	static GLSprite* Factory(int entityID);

	// The shader used for rendering GLSprites.
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
	void OffsetX(float x);
	float OffsetX() const;
	void OffsetY(float y);
	float OffsetY() const;
private:
	unsigned int vertBuf;    // will point to where GL put the buffer
	unsigned int colBuf;    // will point to where GL put the buffer
	unsigned int elemBuf;
	unsigned int vao;
	float offsetX, offsetY;
};
