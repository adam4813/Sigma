#include "components/Renderable.h"

#include "resources/Mesh.h"
#include "resources/Shader.h"
#include "Sigma.h"

namespace Sigma{
	Renderable::Renderable() : lightingEnabled(true), SpatialComponent(0) {
		memset(&this->buffers, 0, sizeof(this->buffers));
		this->vao = 0;
		this->drawMode = GL_TRIANGLES;
		this->VertBufIndex = 0;
		this->ColorBufIndex = 1;
		this->ElemBufIndex = 2;
		this->NormalBufIndex = 3;
		this->UVBufIndex = 4;
		this->cull_face = 0;
		this->depthFunc = GL_LESS;
	}

	Renderable::Renderable(const id_t entityID) : lightingEnabled(true), SpatialComponent(entityID) {
		memset(&this->buffers, 0, sizeof(this->buffers));
		this->vao = 0;
		this->drawMode = GL_TRIANGLES;
		this->VertBufIndex = 0;
		this->ColorBufIndex = 1;
		this->ElemBufIndex = 2;
		this->NormalBufIndex = 3;
		this->UVBufIndex = 4;
		this->cull_face = 0;
		this->depthFunc = GL_LESS;
	}

	void Renderable::SetMesh(std::shared_ptr<resource::Mesh> m) {
		this->mesh = m;
	}

	unsigned int Renderable::GetBuffer(const int index) {
		return this->buffers[index];
	}

	unsigned int Renderable::DrawMode() const {
		return this->drawMode;
	}

	void Renderable::InitializeBuffers() {
		if(!this->shader) {
			assert(0 && "Shader must be loaded before buffers can be initialized.");
		}

		// We must create a vao and then store it in our Mesh.
		if (this->vao == 0) {
			glGenVertexArrays(1, &this->vao); // Generate the VAO
		}
		glBindVertexArray(this->vao); // Bind the VAO

		// Check if the mesh is valid and assign an empty one if it isn't.
		if (!this->mesh) {
			this->mesh.reset(new resource::Mesh());
		}

		if (this->mesh->VertexCount() > 0) {
			if (this->buffers[this->VertBufIndex] == 0) {
				glGenBuffers(1, &this->buffers[this->VertBufIndex]); 	// Generate the vertex buffer.
			}
			glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]); // Bind the vertex buffer.
			glBufferData(GL_ARRAY_BUFFER, sizeof(resource::Vertex) * this->mesh->VertexCount(), &this->mesh->GetVertexBuffer(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
			GLint posLocation = glGetAttribLocation((*shader).GetProgram(), "in_Position"); // Find the location in the shader where the vertex buffer data will be placed.
			glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // Tell the VAO the vertex data will be stored at the location we just found.
			glEnableVertexAttribArray(posLocation); // Enable the VAO line for vertex data.
		}
		if (this->mesh->GetTexCoordCount() > 0) {
			if (this->buffers[this->UVBufIndex] == 0) {
				glGenBuffers(1, &this->buffers[this->UVBufIndex]); 	// Generate the vertex buffer.
			}
			glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->UVBufIndex]); // Bind the vertex buffer.
			glBufferData(GL_ARRAY_BUFFER, sizeof(resource::TexCoord) * this->mesh->GetTexCoordCount(), &this->mesh->GetTexCoordBuffer(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
			GLint uvLocation = glGetAttribLocation((*shader).GetProgram(), "in_UV"); // Find the location in the shader where the vertex buffer data will be placed.
			glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); // Tell the VAO the vertex data will be stored at the location we just found.
			glEnableVertexAttribArray(uvLocation); // Enable the VAO line for vertex data.
		}
		if (this->mesh->VertexColorCount() > 0) {
			if (this->buffers[this->ColorBufIndex] == 0) {
				glGenBuffers(1, &this->buffers[this->ColorBufIndex]);
			}
			glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->ColorBufIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(resource::Color) * this->mesh->VertexColorCount(), &this->mesh->GetVertexColorBuffer(), GL_STATIC_DRAW);
			GLint colLocation = glGetAttribLocation((*shader).GetProgram(), "in_Color");
			glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(colLocation);
		}
		if (this->mesh->FaceCount() > 0) {
			if (this->buffers[this->ElemBufIndex] == 0) {
				glGenBuffers(1, &this->buffers[this->ElemBufIndex]); // Generate the element buffer.
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]); // Bind the element buffer.
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(resource::Face) * this->mesh->FaceCount(), &this->mesh->GetFaceBuffer(), GL_STATIC_DRAW); // Store the faces in the element buffer.
		}
		if (this->mesh->VertexNormalCount() > 0) {
			if (this->buffers[this->NormalBufIndex] == 0) {
				glGenBuffers(1, &this->buffers[this->NormalBufIndex]);
			}
			glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->NormalBufIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(resource::Vertex)*this->mesh->VertexNormalCount(), &this->mesh->GetVertexNormalBuffer(), GL_STATIC_DRAW);
			GLint normalLocation = glGetAttribLocation((*shader).GetProgram(), "in_Normal");
			glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(normalLocation);
		}

		glBindVertexArray(0); // Reset the buffer binding because we are good programmers.

		this->shader->Use();
		this->shader->AddUniform("in_Model");
		this->shader->AddUniform("in_View");
		this->shader->AddUniform("in_Proj");
		this->shader->AddUniform("texEnabled");
		this->shader->AddUniform("ambientTexEnabled");
		this->shader->AddUniform("diffuseTexEnabled");
		this->shader->AddUniform("texAmb");
		this->shader->AddUniform("texDiff");
		this->shader->AddUniform("cubeMap");
		this->shader->AddUniform("cubeMapNormal");
		this->shader->AddUniform("specularHardness");
		this->shader->UnUse();
	}

	void Renderable::Render(glm::mediump_float *view, glm::mediump_float *proj) {
		glm::mat4 modelMatrix = this->Transform()->GetMatrix();

		//if(this->parentTransform != 0) {
		//	modelMatrix = this->parentTransform->GetMatrix() * modelMatrix;
		//}

		this->shader->Use();
		glUniformMatrix4fv((*this->shader)("in_Model"), 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv((*this->shader)("in_View"), 1, GL_FALSE, view);
		glUniformMatrix4fv((*this->shader)("in_Proj"), 1, GL_FALSE, proj);

		glBindVertexArray(this->Vao());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->GetBuffer(this->ElemBufIndex));

		if(this->cull_face == 0) {
			glDisable(GL_CULL_FACE);
		}
		else {
			glCullFace(this->cull_face);
		}

		glDepthFunc(this->depthFunc);

		size_t prev = 0;
		for (int i = 0, cur = this->mesh->MeshGroup_ElementCount(0); cur != 0; prev = cur, cur = this->mesh->MeshGroup_ElementCount(++i)) {
			if (this->mesh->MaterialGroupsCount() > 0) {
				const resource::Material* mat = this->mesh->GetMaterialGroup(*this->mesh->GetMaterialGroupName(prev));

				if (mat->ambientMap) {
					glUniform1i((*this->shader)("texEnabled"), 1);
					glUniform1i((*this->shader)("ambientTexEnabled"), 1);
					glUniform1i((*this->shader)("texAmb"), 1);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, mat->ambientMap);
				} else {
					glUniform1i((*this->shader)("ambientTexEnabled"), 0);
				}

				if (mat->diffuseMap) {
					glUniform1i((*this->shader)("texEnabled"), 1);
					glUniform1i((*this->shader)("diffuseTexEnabled"), 1);
					glUniform1i((*this->shader)("texDiff"), 0);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mat->diffuseMap);
				} else {
					glUniform1i((*this->shader)("diffuseTexEnabled"), 0);
				}

				if (mat->cubeMap) {
					glActiveTexture(GL_TEXTURE0);
					glUniform1i((*this->shader)("cubeMap"), 0);
					glBindTexture(GL_TEXTURE_CUBE_MAP, mat->cubeMap);
				}

				if (mat->cubeNormalMap) {
					glActiveTexture(GL_TEXTURE1);
					glUniform1i((*this->shader)("cubeNormalMap"), 0);
					glBindTexture(GL_TEXTURE_CUBE_MAP, mat->cubeNormalMap);
				}

				glUniform1f((*this->shader)("specularHardness"), mat->hardness);
			}
			else {
				glUniform1i((*this->shader)("texEnabled"), 0);
				glUniform1i((*this->shader)("diffuseTexEnabled"), 0);
				glUniform1i((*this->shader)("ambientTexEnabled"), 0);
			}
			glDrawElements(this->DrawMode(), cur, GL_UNSIGNED_INT, (void*)prev);
		}

		glDepthFunc(GL_LESS);

		// reset defaults
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		this->shader->UnUse();
	}

	unsigned int Renderable::Vao() const {
		return this->vao;
	}

	void Renderable::SetCullFace(std::string cull_face) {
		if(cull_face == "back") {
			this->cull_face = GL_BACK;
		}
		else if (cull_face == "front") {
			this->cull_face = GL_FRONT;
		}
		else if (cull_face == "none") {
			this->cull_face = 0;
		}
		else {
			assert(0 && "Invalid cull_face parameter");
		}
	}

	void Renderable::SetDepthFunc(std::string depthFunc) {
		if(depthFunc == "less") {
			this->depthFunc = GL_LESS;
		}
		else if (depthFunc == "lequal") {
			this->depthFunc = GL_LEQUAL;
		}
	}

	std::shared_ptr<resource::Shader> Renderable::GetShader() {
		return this->shader;
	}

	void Renderable::SetShader(std::shared_ptr<resource::Shader> s) {
		this->shader = s;
	}

	void Renderable::SetLightingEnabled(bool enabled) {
		this->lightingEnabled = enabled;
	}

	bool Renderable::IsLightingEnabled() {
		return this->lightingEnabled;
	}
} // namespace Sigma