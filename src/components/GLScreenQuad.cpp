#include "components/GLScreenQuad.h"
#include "resources/GLTexture.h"

namespace Sigma {
	GLScreenQuad::GLScreenQuad() : texture(nullptr), x(0), y(0), w(0), h(0), inverted(false) {}
	GLScreenQuad::~GLScreenQuad() {}

	void GLScreenQuad::InitializeBuffers() {

		float left = this->x * 2.0f - 1.0f;
		float top = this->y * -2.0f + 1.0f;
		float right = (this->x + this->w) * 2.0f - 1.0f;
		float bottom = (this->y + this->h) * -2.0f + 1.0f;

		this->AddVertex(Vertex(left, top, 0.0f)); // -1, 1
		this->AddVertex(Vertex(right, top, 0.0f)); // 1, 1
		this->AddVertex(Vertex(left, bottom, 0.0f)); // -1, -1
		this->AddVertex(Vertex(right, bottom, 0.0f)); // 1, -1

		this->AddFace(Face(0, 1, 2));
		this->AddFace(Face(2, 1, 3));

		if (!this->inverted) {
			this->texCoords.push_back(TexCoord(0.0f, 0.0f));
			this->texCoords.push_back(TexCoord(1.0f, 0.0f));
			this->texCoords.push_back(TexCoord(0.0f, 1.0f));
			this->texCoords.push_back(TexCoord(1.0f, 1.0f));
		}
		else {
			this->texCoords.push_back(TexCoord(0.0f, 1.0f));
			this->texCoords.push_back(TexCoord(1.0f, 1.0f));
			this->texCoords.push_back(TexCoord(0.0f, 0.0f));
			this->texCoords.push_back(TexCoord(1.0f, 0.0f));
		}

		// Add the mesh group
		this->AddMaterialGroupIndex(0);
	}

	//void GLScreenQuad::Render(glm::mediump_float *view, glm::mediump_float *proj) {
	//	//this->shader->Use();

	//	glDisable(GL_CULL_FACE);
	//	glDisable(GL_DEPTH_TEST);
	//	glDepthMask(GL_FALSE);

	//	glBindVertexArray(this->vao);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->GetBuffer(this->ElemBufIndex));

	//	if(this->texture) {
	//		glUniform1i(glGetUniformLocation((*this->shader).GetProgram(), "in_Texture"), 0);
	//		glBindTexture(GL_TEXTURE_2D, this->texture->GetID());
	//		glActiveTexture(GL_TEXTURE0);
	//	}

	//	size_t prev = 0;
	//	for (int i = 0, cur = this->MeshGroup_ElementCount(0); cur != 0; prev = cur, cur = this->MeshGroup_ElementCount(++i)) {
	//		glDrawElements(this->DrawMode(), cur, GL_UNSIGNED_INT, (void*)prev);
	//	}

	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	//	glBindVertexArray(0);

	//	// Clear the texture for next frame

	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	glEnable(GL_DEPTH_TEST);
	//	glDepthMask(GL_TRUE);
	//	glEnable(GL_CULL_FACE);

	//	//this->shader->UnUse();
	//}

	unsigned int GLScreenQuad::GetTexture() {
		if (this->texture) {
			return this->texture->GetID();
		}
		return 0;
	}

	void GLScreenQuad::SetTexture(resource::GLTexture* texture) {
		this->texture = texture;
	}
};
