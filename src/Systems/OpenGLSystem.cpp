#include "OpenGLSystem.h"
#include "GLSLShader.h"
#include "GLSixDOFView.h"
#include "../Components/GLSprite.h"
#include "../Components/GLIcoSphere.h"
#include "../Components/GLMesh.h"

OpenGLSystem::OpenGLSystem() : windowWidth(800), windowHeight(600), deltaAccumulator(0.0) {
	this->view = new GLSixDOFView(); 
}

OpenGLSystem::~OpenGLSystem() {
	delete this->view;
	for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
		for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
			delete *vecitr;
		}
	}
}

IGLComponent* OpenGLSystem::Factory(const std::string type, const unsigned int entityID, std::vector<Property> &properties) {
	if (type == "GLSprite") {
		GLSprite* spr = new GLSprite(entityID);
		spr->Initialize();
		if (entityID == 2) {
			spr->Transform().Translate(2,2,0);
		}
		this->components[entityID].push_back(spr);
		spr->Transform().Translate(0,0,0);
		return spr;
	} else if (type == "GLIcoSphere") {
		GLIcoSphere* sphere = new GLIcoSphere(entityID);
		sphere->Initialize();
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &*propitr;
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			}
		}
		sphere->Transform().Scale(scale,scale,scale);
		sphere->Transform().Translate(x,y,z);
		this->components[entityID].push_back(sphere);
		return sphere;
	} else if (type=="GLMesh") {
		GLMesh* mesh = new GLMesh(entityID);
		mesh->LoadMesh("lowpolyship.obj");
		mesh->Initialize();
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &*propitr;
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			}
		}
		mesh->Transform().Scale(scale,scale,scale);
		mesh->Transform().Translate(x,y,z);
		this->components[entityID].push_back(mesh);
	}
	return nullptr;
}

bool OpenGLSystem::Update(const double delta) {
	this->deltaAccumulator += delta;
	this->view->UpdateViewMatrix();
	// Check if the deltaAccumulator is greater than 1/60 of a second.
	if (deltaAccumulator > 16.7) {
		// Set up the scene to a "clean" state.
		glClearColor(1.0f,1.0f,1.0f,0.0f);
		glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

		// Set the ViewProjection matrix to be used in the shader.
		// Loop through and draw each component.
		for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
			for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
				try {
					GLSprite* sprite = dynamic_cast<GLSprite*>(*vecitr);
					if (sprite == nullptr) {
						throw std::bad_cast();
					}
					glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
					GLSprite::shader.Use();
					glUniform1i(glGetUniformLocation(GLSprite::shader.GetProgram(),"tex"), 0); // 0 for GL_TEXTURE0
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, 1);
					glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_Model"), 1, GL_FALSE, &sprite->Transform().ModelMatrix()[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_View"), 1, GL_FALSE, &this->view->ViewMatrix[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_Proj"), 1, GL_FALSE, &this->ProjectionMatrix[0][0]);
					glBindVertexArray(sprite->Vao());
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->GetBuffer(sprite->ElemBufIndex));
					glDrawElements(sprite->DrawMode(), sprite->NumberElements(), GL_UNSIGNED_SHORT, (void*)0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
					glBindVertexArray(0);
					GLSprite::shader.UnUse();
					break;
				} catch (std::bad_cast b) {
					//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					GLIcoSphere::shader.Use();
					(*vecitr)->Transform().Rotate(0.0f,0.1f,0.0f);
					glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Model"), 1, GL_FALSE, &(*vecitr)->Transform().ModelMatrix()[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_View"), 1, GL_FALSE, &this->view->ViewMatrix[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Proj"), 1, GL_FALSE, &this->ProjectionMatrix[0][0]);
					glBindVertexArray((*vecitr)->Vao());
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*vecitr)->GetBuffer((*vecitr)->ElemBufIndex));
					for (int i = 0, cur = (*vecitr)->NumberElements(0), prev = 0; cur != 0; prev = cur, cur = (*vecitr)->NumberElements(++i)) {
						glDrawElements((*vecitr)->DrawMode(), cur, GL_UNSIGNED_SHORT, (void*)prev);
					}
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
					glBindVertexArray(0);
					GLIcoSphere::shader.UnUse();
				}
				/*try {
					GLIcoSphere* sphere = dynamic_cast<GLIcoSphere*>(*vecitr);
					if (sphere == nullptr) {
						throw std::bad_cast();
					}
					//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					GLIcoSphere::shader.Use();
					sphere->Transform().Rotate(0.0f,0.1f,0.0f);
					glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Model"), 1, GL_FALSE, &sphere->Transform().ModelMatrix()[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_View"), 1, GL_FALSE, &this->view->ViewMatrix[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Proj"), 1, GL_FALSE, &this->ProjectionMatrix[0][0]);
					glBindVertexArray(sphere->Vao());
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere->GetBuffer(sphere->ElemBufIndex));
					glDrawElements(sphere->DrawMode(), sphere->NumberElements(), GL_UNSIGNED_SHORT, (void*)0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
					glBindVertexArray(0);
					GLIcoSphere::shader.UnUse();
					break;
				}
				catch (std::bad_cast b) {

				}*/
			}
		}

		this->deltaAccumulator = 0.0;
		return true;
	}
	return false;
}

IGLComponent* OpenGLSystem::GetComponent(int entityID) {
	if (this->components.find(entityID) != this->components.end()) {
		return this->components[entityID][0];
	}
	return nullptr;
}

SceneManager* OpenGLSystem::GetScene() {
	return &this->scene;
}

const int* OpenGLSystem::Start() {
	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	//Or better yet, use the GL3 way to get the version number
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	// Now that GL is up and running load the shaders
	GLSprite::LoadShader();
	GLIcoSphere::LoadShader();

	// Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
	this->ProjectionMatrix = glm::perspective(
		45.0f,
		4.0f / 3.0f,
		0.1f,
		10000.0f
		);

	this->view->Move(4.0f,3.0f,-10.f);

	return OpenGLVersion;
}

void OpenGLSystem::Move(float x, float y, float z) {
	this->view->Move(x,y,z);
}

void OpenGLSystem::Rotate(float x, float y, float z) {
	this->view->Rotate(x,y,z);
}