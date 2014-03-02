#include "resources/GLSprite.h"
#ifndef __APPLE__
#include "GL/glew.h"
#endif
#include "resources/Texture.h"

namespace Sigma{
	GLSprite::GLSprite( const id_t entityID /*= 0*/ ) : Renderable(entityID), texture(nullptr)  {
		this->drawMode = GL_TRIANGLES;
		this->ElemBufIndex = 2;
		this->ColorBufIndex = 1;
		this->VertBufIndex = 0;
		this->UVBufIndex = 3;
	}

	void GLSprite::InitializeBuffers() {
		static const GLfloat vert[] = {
			1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f
		};

		static const GLfloat col[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f,
		};

		static const GLfloat uv[] = {
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
		};

		// We must create a vao and then store it in our GLSprite.
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);

		glGenBuffers(1, &this->buffers[this->VertBufIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
		GLint posLocation = glGetAttribLocation((*shader).GetProgram(), "in_Position");
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posLocation);

		static const GLushort elem[] = { 0, 1, 2, 3, 4, 5 };
		glGenBuffers(1, &this->buffers[this->ElemBufIndex]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elem), elem, GL_STATIC_DRAW);

		glGenBuffers(1, &this->buffers[this->ColorBufIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->ColorBufIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
		GLint colLocation = glGetAttribLocation((*shader).GetProgram(), "in_Color");
		glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(colLocation);

		glGenBuffers(1, &this->buffers[this->UVBufIndex]);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->UVBufIndex]);
		glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);
		GLint uvlocation = glGetAttribLocation((*shader).GetProgram(),"in_UV");
		glVertexAttribPointer(uvlocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(uvlocation);

		glBindVertexArray(0);
		this->shader->Use();
		this->shader->AddUniform("in_Model");
		this->shader->AddUniform("in_View");
		this->shader->AddUniform("in_Proj");
		this->shader->AddUniform("tex");
		this->shader->UnUse();
	}

	void GLSprite::Render(glm::mediump_float *view, glm::mediump_float *proj) {
		this->shader->Use();

		glm::mat4 modelMatrix = this->Transform()->GetMatrix();

		glUniformMatrix4fv((*this->shader)("in_Model"), 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv((*this->shader)("in_View"), 1, GL_FALSE, view);
		glUniformMatrix4fv((*this->shader)("in_Proj"), 1, GL_FALSE, proj);

		glBindVertexArray(this->Vao());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->GetBuffer(this->ElemBufIndex));

		// Check to make sure we have a valid texture
		if (this->texture) {
			glUniform1i((*this->shader)("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texture->GetID());
		}

		glDrawElements(this->DrawMode(), this->MeshGroup_ElementCount(), GL_UNSIGNED_SHORT, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		this->shader->UnUse();
	}

	void GLSprite::SetTexture(std::shared_ptr<resource::Texture> texture) {
		this->texture = texture;
	}
} // namespace Sigma
