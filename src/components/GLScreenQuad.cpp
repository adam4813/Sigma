#include "components/GLScreenQuad.h"
#include "resources/GLTexture.h"

namespace Sigma {
	GLScreenQuad::GLScreenQuad(int entityID) : GLMesh(entityID), texture(nullptr), x(0), y(0), w(0), h(0) {}
	GLScreenQuad::~GLScreenQuad() {}

	void GLScreenQuad::InitializeBuffers() {
		this->AddVertex(Vertex(this->x, this->y, 0.0f));
		this->AddVertex(Vertex(this->w, this->y, 0.0f));
		this->AddVertex(Vertex(this->x, this->h, 0.0f));
		this->AddVertex(Vertex(this->w, this->h, 0.0f));

		this->AddFace(Face(0, 1, 2));
		this->AddFace(Face(2, 1, 3));

		float pixelWidth, pixelHeight;

		// NOTE: This is currently dependend on a fixed screen size
		pixelWidth = abs(this->x - this->w) * 512.0f;
		pixelHeight = abs(this->y - this->h) * 384.0f;

		// Calculate nearest power of two texture size, for speed
		this->texture_size = this->NearestPowerOf2(pixelWidth, pixelHeight);
		float offset;

		// Figure out corrected texture coordinates
		if(pixelWidth > pixelHeight) {
			offset = (pixelHeight / pixelWidth);
			this->texCoords.push_back(TexCoord(0.0f, 0.0f));
			this->texCoords.push_back(TexCoord(1.0f, 0.0f));
			this->texCoords.push_back(TexCoord(0.0f, offset));
			this->texCoords.push_back(TexCoord(1.0f, offset));
		} else {
			offset = (pixelWidth / pixelHeight);
			this->texCoords.push_back(TexCoord(0.0f, 0.0f));
			this->texCoords.push_back(TexCoord(offset, 0.0f));
			this->texCoords.push_back(TexCoord(0.0f, 1.0f));
			this->texCoords.push_back(TexCoord(offset, 1.0f));
		}

		// Add the mesh group
		this->AddMeshGroupIndex(0);

		GLMesh::InitializeBuffers();
	}

	void GLScreenQuad::Render(glm::mediump_float *view, glm::mediump_float *proj) {
		this->shader->Use();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		glBindVertexArray(this->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->GetBuffer(this->ElemBufIndex));

		glUniform1i(glGetUniformLocation((*this->shader).GetProgram(), "in_Texture"), 0);
		glBindTexture(GL_TEXTURE_2D, this->texture->GetID());
		glActiveTexture(GL_TEXTURE0);

		for (int i = 0, cur = this->MeshGroup_ElementCount(0), prev = 0; cur != 0; prev = cur, cur = this->MeshGroup_ElementCount(++i)) {
			glDrawElements(this->DrawMode(), cur, GL_UNSIGNED_INT, (void*)prev);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindVertexArray(0);

		// Clear the texture for next frame

        glBindTexture(GL_TEXTURE_2D, 0);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);

		this->shader->UnUse();
	}

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