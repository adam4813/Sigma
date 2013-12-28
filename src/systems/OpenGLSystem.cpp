#include "systems/OpenGLSystem.h"
#include "systems/GLSLShader.h"
#include "systems/GLSixDOFView.h"
#include "controllers/FPSCamera.h"
#include "components/GLSprite.h"
#include "components/GLIcoSphere.h"
#include "components/GLCubeSphere.h"
#include "components/GLMesh.h"
#include "components/GLScreenQuad.h"
#include "components/PointLight.h"
#include "components/SpotLight.h"

#ifndef __APPLE__
#include "GL/glew.h"
#endif

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Sigma{
	// RenderTarget methods
	RenderTarget::~RenderTarget() {
		glDeleteTextures(this->texture_ids.size(), &this->texture_ids[0]); // Perhaps should check if texture was created for this RT or is used elsewhere
		glDeleteRenderbuffers(1, &this->depth_id);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &this->fbo_id);
	}

	void RenderTarget::BindWrite() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo_id);

		std::vector<GLenum> buffers;

		for(unsigned int i=0; i < this->texture_ids.size(); i++) {
			buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}

		glDrawBuffers(this->texture_ids.size(), &buffers[0]);
	}

	void RenderTarget::BindRead() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo_id);
	}

	void RenderTarget::UnbindWrite() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		std::vector<GLenum> buffers;
		buffers.push_back(GL_COLOR_ATTACHMENT0);
		glDrawBuffers(1, &buffers[0]);
	}

	void RenderTarget::UnbindRead() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	std::map<std::string, Sigma::resource::GLTexture> OpenGLSystem::textures;

	OpenGLSystem::OpenGLSystem() : windowWidth(1024), windowHeight(768), deltaAccumulator(0.0),
		framerate(60.0f), pointQuad(1000), ambientQuad(1001), spotQuad(1002), viewMode("") {}

	//std::map<std::string, resource::GLTexture> OpenGLSystem::textures = std::map<std::string, resource::GLTexture>();

	std::map<std::string, Sigma::IFactory::FactoryFunction>
        OpenGLSystem::getFactoryFunctions() {
		using namespace std::placeholders;

		std::map<std::string, Sigma::IFactory::FactoryFunction> retval;
		retval["GLSprite"] = std::bind(&OpenGLSystem::createGLSprite,this,_1,_2);
		retval["GLIcoSphere"] = std::bind(&OpenGLSystem::createGLIcoSphere,this,_1,_2);
		retval["GLCubeSphere"] = std::bind(&OpenGLSystem::createGLCubeSphere,this,_1,_2);
		retval["GLMesh"] = std::bind(&OpenGLSystem::createGLMesh,this,_1,_2);
		retval["FPSCamera"] = std::bind(&OpenGLSystem::createGLView,this,_1,_2, "FPSCamera");
		retval["GLSixDOFView"] = std::bind(&OpenGLSystem::createGLView,this,_1,_2, "GLSixDOFView");
		retval["PointLight"] = std::bind(&OpenGLSystem::createPointLight,this,_1,_2);
		retval["SpotLight"] = std::bind(&OpenGLSystem::createSpotLight,this,_1,_2);
		retval["GLScreenQuad"] = std::bind(&OpenGLSystem::createScreenQuad,this,_1,_2);

        return retval;
    }

	IComponent* OpenGLSystem::createGLView(const id_t entityID, const std::vector<Property> &properties, std::string mode) {
		viewMode = mode;

		if(mode=="FPSCamera") {
			this->views.push_back(new Sigma::event::handler::FPSCamera(entityID));
		}
		else if(mode=="GLSixDOFView") {
			this->views.push_back(new GLSixDOFView(entityID));
		}
		else {
			std::cerr << "Invalid view type!" << std::endl;
			return nullptr;
		}

		float x=0.0f, y=0.0f, z=0.0f, rx=0.0f, ry=0.0f, rz=0.0f;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);

			if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "rx") {
				rx = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "ry") {
				ry = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "rz") {
				rz = p->Get<float>();
				continue;
			}
		}

		this->views[this->views.size() - 1]->Transform()->TranslateTo(x,y,z);
		this->views[this->views.size() - 1]->Transform()->Rotate(rx,ry,rz);

		this->addComponent(entityID, this->views[this->views.size() - 1]);

		return this->views[this->views.size() - 1];
	}

	IComponent* OpenGLSystem::createGLSprite(const id_t entityID, const std::vector<Property> &properties) {
		GLSprite* spr = new GLSprite(entityID);
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		int componentID = 0;
		std::string textureFilename;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
			}
			else if (p->GetName() == "x") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
			}
			else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			}
			else if (p->GetName() == "textureFilename"){
				textureFilename = p->Get<std::string>();
			}
		}

		// Check if the texture is loaded and load it if not.
		if (textures.find(textureFilename) == textures.end()) {
			Sigma::resource::GLTexture texture;
			texture.LoadDataFromFile(textureFilename);
			if (texture.GetID() != 0) {
				Sigma::OpenGLSystem::textures[textureFilename] = texture;
			}
		}

		// It should be loaded, but in case an error occurred double check for it.
		if (textures.find(textureFilename) != textures.end()) {
			spr->SetTexture(&Sigma::OpenGLSystem::textures[textureFilename]);
		}
		spr->LoadShader();
		spr->Transform()->Scale(glm::vec3(scale));
		spr->Transform()->Translate(x,y,z);
		spr->InitializeBuffers();
		this->addComponent(entityID,spr);
		return spr;
	}

	IComponent* OpenGLSystem::createGLIcoSphere(const id_t entityID, const std::vector<Property> &properties) {
		Sigma::GLIcoSphere* sphere = new Sigma::GLIcoSphere(entityID);
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		int componentID = 0;
		std::string shader_name = "shaders/icosphere";

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			}
			else if (p->GetName() == "shader"){
				shader_name = p->Get<std::string>();
			}
			else if (p->GetName() == "lightEnabled") {
				sphere->SetLightingEnabled(p->Get<bool>());
			}
		}
		sphere->Transform()->Scale(scale,scale,scale);
		sphere->Transform()->Translate(x,y,z);
		sphere->LoadShader(shader_name);
		sphere->InitializeBuffers();
		sphere->SetCullFace("back");
		this->addComponent(entityID,sphere);
		return sphere;
	}

	IComponent* OpenGLSystem::createGLCubeSphere(const id_t entityID, const std::vector<Property> &properties) {
		Sigma::GLCubeSphere* sphere = new Sigma::GLCubeSphere(entityID);

		std::string texture_name = "";
		std::string shader_name = "shaders/cubesphere";
		std::string cull_face = "back";
		int subdivision_levels = 1;
		float rotation_speed = 0.0f;
		bool fix_to_camera = false;

		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		float rz = 0.0f;
		int componentID = 0;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
			}
			else if (p->GetName() == "x") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
			}
			else if (p->GetName() == "rx") {
				rx = p->Get<float>();
			}
			else if (p->GetName() == "ry") {
				ry = p->Get<float>();
			}
			else if (p->GetName() == "rz") {
				rz = p->Get<float>();
			}
			else if (p->GetName() == "subdivision_levels") {
				subdivision_levels = p->Get<int>();
			}
			else if (p->GetName() == "texture") {
				texture_name = p->Get<std::string>();
			}
			else if (p->GetName() == "shader") {
				shader_name = p->Get<std::string>();
			}
			else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			}
			else if (p->GetName() == "cullface") {
				cull_face = p->Get<std::string>();
			}
			else if (p->GetName() == "fix_to_camera") {
				fix_to_camera = p->Get<bool>();
			}
			else if (p->GetName() == "lightEnabled") {
				sphere->SetLightingEnabled(p->Get<bool>());
			}
		}

		sphere->SetSubdivisions(subdivision_levels);
		sphere->SetFixToCamera(fix_to_camera);
		sphere->SetCullFace(cull_face);
		sphere->Transform()->Scale(scale,scale,scale);
		sphere->Transform()->Rotate(rx,ry,rz);
		sphere->Transform()->Translate(x,y,z);
		sphere->LoadShader(shader_name);
		sphere->LoadTexture(texture_name);
		sphere->InitializeBuffers();

		this->addComponent(entityID,sphere);
		return sphere;
	}

	IComponent* OpenGLSystem::createGLMesh(const id_t entityID, const std::vector<Property> &properties) {
		Sigma::GLMesh* mesh = new Sigma::GLMesh(entityID);

		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		float rz = 0.0f;
		int componentID = 0;
		std::string cull_face = "back";
		std::string shaderfile = "";

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &*propitr;
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "rx") {
				rx = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "ry") {
				ry = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "rz") {
				rz = p->Get<float>();
				continue;
			}
			else if (p->GetName() == "meshFile") {
				std::cerr << "Loading mesh: " << p->Get<std::string>() << std::endl;
				mesh->LoadMesh(p->Get<std::string>());
			}
			else if (p->GetName() == "shader") {
				shaderfile = p->Get<std::string>();
			}
			else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			}
			else if (p->GetName() == "cullface") {
				cull_face = p->Get<std::string>();
			}
			else if (p->GetName() == "lightEnabled") {
				mesh->SetLightingEnabled(p->Get<bool>());
			}
			else if (p->GetName() == "parent") {
				/* Right now hacky, only GLMesh and FPSCamera are supported as parents */

				int parentID = p->Get<int>();
				SpatialComponent *comp = dynamic_cast<SpatialComponent *>(this->getComponent(parentID, Sigma::GLMesh::getStaticComponentTypeName()));

				if(comp) {
					mesh->Transform()->SetParentTransform(comp->Transform());
				} else {
					comp = dynamic_cast<SpatialComponent *>(this->getComponent(parentID, Sigma::event::handler::FPSCamera::getStaticComponentTypeName()));

					if(comp) {
						mesh->Transform()->SetParentTransform(comp->Transform());
					}
				}
			}
		}

		mesh->SetCullFace(cull_face);
		mesh->Transform()->Scale(scale,scale,scale);
		mesh->Transform()->Translate(x,y,z);
		mesh->Transform()->Rotate(rx,ry,rz);
		if(shaderfile != "") {
			mesh->LoadShader(shaderfile);
		}
		else {
			mesh->LoadShader(); // load default
		}
        mesh->InitializeBuffers();
        this->addComponent(entityID,mesh);
        return mesh;
    }

	IComponent* OpenGLSystem::createScreenQuad(const id_t entityID, const std::vector<Property> &properties) {
		Sigma::GLScreenQuad* quad = new Sigma::GLScreenQuad(entityID);

		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;

		int componentID = 0;
		std::string textureName;
		bool textureInMemory = false;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);
			if (p->GetName() == "left") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "top") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "width") {
				w = p->Get<float>();
			}
			else if (p->GetName() == "height") {
				h = p->Get<float>();
			}
			else if (p->GetName() == "textureName") {
				textureName = p->Get<std::string>();
				textureInMemory = true;
			}
			else if (p->GetName() == "textureFileName") {
				textureName = p->Get<std::string>();
			}
		}

		// Check if the texture is loaded and load it if not.
		if (textures.find(textureName) == textures.end()) {
			Sigma::resource::GLTexture texture;
			if (textureInMemory) { // We are using an in memory texture. It will be populated somewhere else
				Sigma::OpenGLSystem::textures[textureName] = texture;
			}
			else { // The texture in on disk so load it.
				texture.LoadDataFromFile(textureName);
				if (texture.GetID() != 0) {
					Sigma::OpenGLSystem::textures[textureName] = texture;
				}
			}
		}

		// It should be loaded, but in case an error occurred double check for it.
		if (textures.find(textureName) != textures.end()) {
			quad->SetTexture(&Sigma::OpenGLSystem::textures[textureName]);
		}

		quad->SetPosition(x, y);
		quad->SetSize(w, h);
		quad->LoadShader("shaders/quad");
		quad->InitializeBuffers();
		this->screensSpaceComp.push_back(std::unique_ptr<IGLComponent>(quad));

		return quad;
	}

	IComponent* OpenGLSystem::createPointLight(const id_t entityID, const std::vector<Property> &properties) {
		Sigma::PointLight *light = new Sigma::PointLight(entityID);

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &*propitr;
			if (p->GetName() == "x") {
				light->position.x = p->Get<float>();
			}
			else if (p->GetName() == "y") {
				light->position.y = p->Get<float>();
			}
			else if (p->GetName() == "z") {
				light->position.z = p->Get<float>();
			}
			else if (p->GetName() == "intensity") {
				light->intensity = p->Get<float>();
			}
			else if (p->GetName() == "cr") {
				light->color.r = p->Get<float>();
			}
			else if (p->GetName() == "cg") {
				light->color.g = p->Get<float>();
			}
			else if (p->GetName() == "cb") {
				light->color.b = p->Get<float>();
			}
			else if (p->GetName() == "ca") {
				light->color.a = p->Get<float>();
			}
			else if (p->GetName() == "radius") {
				light->radius = p->Get<float>();
			}
			else if (p->GetName() == "falloff") {
				light->falloff = p->Get<float>();
			}
		}

		this->addComponent(entityID, light);
		return light;
	}

	IComponent* OpenGLSystem::createSpotLight(const id_t entityID, const std::vector<Property> &properties) {
		Sigma::SpotLight *light = new Sigma::SpotLight(entityID);

		float x=0.0f, y=0.0f, z=0.0f;
		float rx=0.0f, ry=0.0f, rz=0.0f;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &*propitr;
			if (p->GetName() == "x") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
			}
			else if (p->GetName() == "rx") {
				rx = p->Get<float>();
			}
			else if (p->GetName() == "ry") {
				ry = p->Get<float>();
			}
			else if (p->GetName() == "rz") {
				rz = p->Get<float>();
			}
			else if (p->GetName() == "intensity") {
				light->intensity = p->Get<float>();
			}
			else if (p->GetName() == "cr") {
				light->color.r = p->Get<float>();
			}
			else if (p->GetName() == "cg") {
				light->color.g = p->Get<float>();
			}
			else if (p->GetName() == "cb") {
				light->color.b = p->Get<float>();
			}
			else if (p->GetName() == "ca") {
				light->color.a = p->Get<float>();
			}
			else if (p->GetName() == "angle") {
				light->angle = p->Get<float>();
				light->cosCutoff = glm::cos(light->angle);
			}
			else if (p->GetName() == "exponent") {
				light->exponent = p->Get<float>();
			}
			else if (p->GetName() == "parent") {
				/* Right now hacky, only GLMesh and FPSCamera are supported as parents */
				int parentID = p->Get<int>();
				SpatialComponent *comp = dynamic_cast<SpatialComponent *>(this->getComponent(parentID, Sigma::GLMesh::getStaticComponentTypeName()));

				if(comp) {
					light->transform.SetParentTransform(comp->Transform());
				} else {
					comp = dynamic_cast<SpatialComponent *>(this->getComponent(parentID, Sigma::event::handler::FPSCamera::getStaticComponentTypeName()));

					if(comp) {
						light->transform.SetParentTransform(comp->Transform());
					}
				}
			}
		}

		light->transform.TranslateTo(x, y, z);
		light->transform.Rotate(rx, ry, rz);

		this->addComponent(entityID, light);

		return light;
	}

	int OpenGLSystem::createRenderTarget(const unsigned int w, const unsigned int h) {
		std::unique_ptr<RenderTarget> newRT(new RenderTarget());

		// Create the frame buffer object
		glGenFramebuffers(1, &newRT->fbo_id);
		glBindFramebuffer(GL_FRAMEBUFFER, newRT->fbo_id);

		printOpenGLError();

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		newRT->width = w;
		newRT->height = h;

		glGenRenderbuffers(1, &newRT->depth_id);
		glBindRenderbuffer(GL_RENDERBUFFER, newRT->depth_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);

		printOpenGLError();

		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newRT->depth_id);

		printOpenGLError();

		//Does the GPU support current FBO configuration?
		GLenum status;
		status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

		switch(status) {
			case GL_FRAMEBUFFER_COMPLETE:
				std::cout << "Successfully created render target.";
				break;
			default:
				assert(0 && "Error: Framebuffer format is not compatible.");
		}

		// Unbind objects
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		this->renderTargets.push_back(std::move(newRT));
		return (this->renderTargets.size() - 1);
	}

	void OpenGLSystem::createRTBuffer(unsigned int rtID, GLint format, GLenum internalFormat, GLenum type) {
		RenderTarget *rt = this->renderTargets[rtID].get();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rt->fbo_id);

		// Create a texture for each requested target
		GLuint texture_id;

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		// Texture params for full screen quad
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//NULL means reserve texture memory, but texels are undefined
		glTexImage2D(GL_TEXTURE_2D, 0, format,
					 (GLsizei)rt->width,
					 (GLsizei)rt->height,
					 0, internalFormat, type, NULL);

		//Attach 2D texture to this FBO
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+rt->texture_ids.size(), GL_TEXTURE_2D, texture_id, 0);

		this->renderTargets[rtID]->texture_ids.push_back(texture_id);

		//Does the GPU support current FBO configuration?
		GLenum status;
		status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

		switch(status) {
			case GL_FRAMEBUFFER_COMPLETE:
				std::cout << "Successfully created render target." << std::endl;
				break;
			default:
				assert(0 && "Error: Framebuffer format is not compatible.");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

    bool OpenGLSystem::Update(const double delta) {
        this->deltaAccumulator += delta;

        // Check if the deltaAccumulator is greater than 1/<framerate>th of a second.
        //  ..if so, it's time to render a new frame
        if (this->deltaAccumulator > (1.0 / this->framerate)) {

			/////////////////////
			// Rendering Setup //
			/////////////////////

			glm::vec3 viewPosition;
			glm::mat4 viewProjInv;

			// Setup the view matrix and position variables
			glm::mat4 viewMatrix;
			if (this->views.size() > 0) {
				viewMatrix = this->views[this->views.size() - 1]->GetViewMatrix();
				viewPosition = this->views[this->views.size() - 1]->Transform()->GetPosition();
			}

			// Setup the projection matrix
			glm::mat4 viewProj = glm::mul(this->ProjectionMatrix, viewMatrix);

			viewProjInv = glm::inverse(viewProj);

			// Calculate frustum for culling
			this->GetView(0)->CalculateFrustum(viewProj);

			// Clear the backbuffer and primary depth/stencil buffer
			glClearColor(0.0f,0.0f,0.0f,1.0f);
            glViewport(0, 0, this->windowWidth, this->windowHeight); // Set the viewport size to fill the window
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

			//////////////////
			// GBuffer Pass //
			//////////////////

			// Bind the first buffer, which is the Geometry Buffer
			if(this->renderTargets.size() > 0) {
				this->renderTargets[0]->BindWrite();
			}

			// Disable blending
			glDisable(GL_BLEND);

			// Clear the GBuffer
            glClearColor(0.0f,0.0f,0.0f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

			// Loop through and draw each GL Component component.
			for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
				for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
					IGLComponent *glComp = dynamic_cast<IGLComponent *>(citr->second.get());

					if(glComp && glComp->IsLightingEnabled()) {
						glComp->GetShader()->Use();

						// Set view position
						//glUniform3f(glGetUniformBlockIndex(glComp->GetShader()->GetProgram(), "viewPosW"), viewPosition.x, viewPosition.y, viewPosition.z);

						// For now, turn on ambient intensity and turn off lighting
						glUniform1f(glGetUniformLocation(glComp->GetShader()->GetProgram(), "ambLightIntensity"), 0.05f);
						glUniform1f(glGetUniformLocation(glComp->GetShader()->GetProgram(), "diffuseLightIntensity"), 0.0f);
						glUniform1f(glGetUniformLocation(glComp->GetShader()->GetProgram(), "specularLightIntensity"), 0.0f);

						glComp->Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);
					}
				}
			}

			// Unbind the first buffer, which is the Geometry Buffer
			if(this->renderTargets.size() > 0) {
				this->renderTargets[0]->UnbindWrite();
			}

			// Copy gbuffer's depth buffer to the screen depth buffer
			// needed for non deferred rendering at the end of this method
			// NOTE: I'm sure there's a faster way to do this
			if(this->renderTargets.size() > 0) {
				this->renderTargets[0]->BindRead();
			}

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, this->windowWidth, this->windowHeight, 0, 0, this->windowWidth, this->windowHeight,
                  GL_DEPTH_BUFFER_BIT, GL_NEAREST);

			if(this->renderTargets.size() > 0) {
				this->renderTargets[0]->UnbindRead();
			}

			///////////////////
			// Lighting Pass //
			///////////////////

			// Turn on additive blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			// Disable depth testing
			glDepthFunc(GL_NONE);
			glDepthMask(GL_FALSE);

			// Bind the second buffer, which is the Light Accumulation Buffer
			//if(this->renderTargets.size() > 1) {
			//	this->renderTargets[1]->BindWrite();
			//}

			// Clear it
            //glClear(GL_COLOR_BUFFER_BIT);

			// Bind the Geometry buffer for reading
			if(this->renderTargets.size() > 0) {
				this->renderTargets[0]->BindRead();
			}

			// Ambient light pass

			// Currently simple constant ambient light, could use SSAO here
			glm::vec4 ambientLight(0.1f, 0.1f, 0.1f, 1.0f);

			GLSLShader &shader = (*this->ambientQuad.GetShader().get());
			shader.Use();

			// Load variables
			glUniform4f(shader("ambientColor"), ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);

			glUniform1i(shader("colorBuffer"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->renderTargets[0]->texture_ids[0]);

			this->ambientQuad.Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);

			shader.UnUse();

			// Dynamic light passes

			// Loop through each light, render a fullscreen quad if it is visible

			for(auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
				for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
					// Check if this component is a point light
					PointLight *light = dynamic_cast<PointLight*>(citr->second.get());

					// If it is a point light, and it intersects the frustum, then render
					if(light && this->GetView(0)->CameraFrustum.isectSphere(light->position, light->radius) ) {

						GLSLShader &shader = (*this->pointQuad.GetShader().get());
						shader.Use();

						// Load variables
						glUniform3fv(shader("viewPosW"), 3, &viewPosition[0]);
						glUniformMatrix4fv(shader("viewProjInverse"), 1, false, &viewProjInv[0][0]);
						glUniform3fv(shader("lightPosW"), 1, &light->position[0]);
						glUniform1f(shader("lightRadius"), light->radius);
						glUniform4fv(shader("lightColor"), 1, &light->color[0]);

						glUniform1i(shader("diffuseBuffer"), 0);
						glUniform1i(shader("normalBuffer"), 1);
						glUniform1i(shader("depthBuffer"), 2);

						// Bind GBuffer textures
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, this->renderTargets[0]->texture_ids[0]);
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, this->renderTargets[0]->texture_ids[1]);
						glActiveTexture(GL_TEXTURE2);
						glBindTexture(GL_TEXTURE_2D, this->renderTargets[0]->texture_ids[2]);

						this->pointQuad.Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);

						shader.UnUse();

						continue;
					}

					SpotLight *spotLight = dynamic_cast<SpotLight *>(citr->second.get());

					if(spotLight && spotLight->IsEnabled()) {
						GLSLShader &shader = (*this->spotQuad.GetShader().get());
						shader.Use();

						glm::vec3 position = spotLight->transform.ExtractPosition();
						glm::vec3 direction = spotLight->transform.GetForward();

						// Load variables
						glUniformMatrix4fv(shader("viewProjInverse"), 1, false, &viewProjInv[0][0]);
						glUniform3fv(shader("lightPosW"), 1, &position[0]);
						glUniform3fv(shader("lightDirW"), 1, &direction[0]);
						glUniform4fv(shader("lightColor"), 1, &spotLight->color[0]);
						glUniform1f(shader("lightAngle"), spotLight->angle);
						glUniform1f(shader("lightCosCutoff"), spotLight->cosCutoff);
						glUniform1f(shader("lightExponent"), spotLight->exponent);

						glUniform1i(shader("diffuseBuffer"), 0);
						glUniform1i(shader("normalBuffer"), 1);
						glUniform1i(shader("depthBuffer"), 2);

						// Bind GBuffer textures
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, this->renderTargets[0]->texture_ids[0]);
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, this->renderTargets[0]->texture_ids[1]);
						glActiveTexture(GL_TEXTURE2);
						glBindTexture(GL_TEXTURE_2D, this->renderTargets[0]->texture_ids[2]);

						this->spotQuad.Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);

						shader.UnUse();

						continue;
					}
				}
			}

			// Unbind the Geometry buffer for reading
			if(this->renderTargets.size() > 0) {
				this->renderTargets[0]->UnbindRead();
			}

			// Unbind the second buffer, which is the Light Accumulation Buffer
			//if(this->renderTargets.size() > 1) {
			//	this->renderTargets[1]->UnbindWrite();
			//}

			// Remove blending
			glDisable(GL_BLEND);

			// Re-enabled depth test
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);

			////////////////////
			// Composite Pass //
			////////////////////

			// Not needed yet

			///////////////////////
			// Draw Unlit Objects
			///////////////////////

			// Loop through and draw each GL Component component.
			for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
				for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
					IGLComponent *glComp = dynamic_cast<IGLComponent *>(citr->second.get());

					if(glComp && !glComp->IsLightingEnabled()) {
						glComp->GetShader()->Use();

						// Set view position
						glUniform3f(glGetUniformBlockIndex(glComp->GetShader()->GetProgram(), "viewPosW"), viewPosition.x, viewPosition.y, viewPosition.z);

						glComp->Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);
					}
				}
			}

			//////////////////
			// Overlay Pass //
			//////////////////

			// Enable transparent rendering
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for (auto citr = this->screensSpaceComp.begin(); citr != this->screensSpaceComp.end(); ++citr) {
					citr->get()->GetShader()->Use();

					// Set view position
					//glUniform3f(glGetUniformBlockIndex(citr->get()->GetShader()->GetProgram(), "viewPosW"), viewPosition.x, viewPosition.y, viewPosition.z);

					// For now, turn on ambient intensity and turn off lighting
					glUniform1f(glGetUniformLocation(citr->get()->GetShader()->GetProgram(), "ambLightIntensity"), 0.05f);
					glUniform1f(glGetUniformLocation(citr->get()->GetShader()->GetProgram(), "diffuseLightIntensity"), 0.0f);
					glUniform1f(glGetUniformLocation(citr->get()->GetShader()->GetProgram(), "specularLightIntensity"), 0.0f);
					citr->get()->Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);
			}

			// Remove blending
			glDisable(GL_BLEND);

			// Unbind frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

            this->deltaAccumulator = 0.0;
            return true;
        }
        return false;
    }

	GLTransform *OpenGLSystem::GetTransformFor(const unsigned int entityID) {
		auto entity = &(_Components[entityID]);

		// for now, just returns the first component's transform
		// bigger question: should entities be able to have multiple GLComponents?
		for(auto compItr = entity->begin(); compItr != entity->end(); compItr++) {
			IGLComponent *glComp = dynamic_cast<IGLComponent *>((*compItr).second.get());
			if(glComp) {
				GLTransform *transform = glComp->Transform();
				return transform;
			}
		}

		// no GL components
		return 0;
	}

    const int* OpenGLSystem::Start() {
        // Use the GL3 way to get the version number
        glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
        glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

		// Sanity check to make sure we are at least in a good major version number.
		assert((OpenGLVersion[0] > 1) && (OpenGLVersion[0] < 5));

		// Determine the aspect ratio and sanity check it to a safe ratio
		float aspectRatio = static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight);
		if (aspectRatio < 1.0f) {
			aspectRatio = 4.0f / 3.0f;
		}

		// Generate a projection matrix (the "view") based on basic window dimensions
        this->ProjectionMatrix = glm::perspective(
            45.0f, // field-of-view (height)
            aspectRatio, // aspect ratio
            0.1f, // near culling plane
            10000.0f // far culling plane
            );

        // App specific global gl settings
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
#if __APPLE__
        // GL_TEXTURE_CUBE_MAP_SEAMLESS and GL_MULTISAMPLE are Core.
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // allows for cube-mapping without seams
        glEnable(GL_MULTISAMPLE);
#else
        if (GLEW_AMD_seamless_cubemap_per_texture) {
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // allows for cube-mapping without seams
		}
		if (GLEW_ARB_multisample) {
			glEnable(GL_MULTISAMPLE_ARB);
		}
#endif
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);

		// Setup a screen quad for deferred rendering
		this->pointQuad.SetSize(1.0f, 1.0f);
		this->pointQuad.SetPosition(0.0f, 0.0f);
		this->pointQuad.LoadShader("shaders/pointlight");
		this->pointQuad.InitializeBuffers();
		this->pointQuad.SetCullFace("none");

		this->pointQuad.GetShader()->Use();
		this->pointQuad.GetShader()->AddUniform("viewPosW");
		this->pointQuad.GetShader()->AddUniform("viewProjInverse");
		this->pointQuad.GetShader()->AddUniform("lightPosW");
		this->pointQuad.GetShader()->AddUniform("lightRadius");
		this->pointQuad.GetShader()->AddUniform("lightColor");
		this->pointQuad.GetShader()->AddUniform("diffuseBuffer");
		this->pointQuad.GetShader()->AddUniform("normalBuffer");
		this->pointQuad.GetShader()->AddUniform("depthBuffer");
		this->pointQuad.GetShader()->UnUse();

		this->spotQuad.SetSize(1.0f, 1.0f);
		this->spotQuad.SetPosition(0.0f, 0.0f);
		this->spotQuad.LoadShader("shaders/spotlight");
		this->spotQuad.InitializeBuffers();
		this->spotQuad.SetCullFace("none");

		this->spotQuad.GetShader()->Use();
		this->spotQuad.GetShader()->AddUniform("viewProjInverse");
		this->spotQuad.GetShader()->AddUniform("lightPosW");
		this->spotQuad.GetShader()->AddUniform("lightDirW");
		this->spotQuad.GetShader()->AddUniform("lightColor");
		this->spotQuad.GetShader()->AddUniform("lightAngle");
		this->spotQuad.GetShader()->AddUniform("lightCosCutoff");
		this->spotQuad.GetShader()->AddUniform("lightExponent");
		this->spotQuad.GetShader()->AddUniform("diffuseBuffer");
		this->spotQuad.GetShader()->AddUniform("normalBuffer");
		this->spotQuad.GetShader()->AddUniform("depthBuffer");
		this->spotQuad.GetShader()->UnUse();

		this->ambientQuad.SetSize(1.0f, 1.0f);
		this->ambientQuad.SetPosition(0.0f, 0.0f);
		this->ambientQuad.LoadShader("shaders/ambient");
		this->ambientQuad.InitializeBuffers();
		this->ambientQuad.SetCullFace("none");

		this->ambientQuad.GetShader()->Use();
		this->ambientQuad.GetShader()->AddUniform("ambientColor");
		this->ambientQuad.GetShader()->AddUniform("colorBuffer");
		this->ambientQuad.GetShader()->UnUse();

        return OpenGLVersion;
    }

    void OpenGLSystem::SetViewportSize(const unsigned int width, const unsigned int height) {
        this->windowHeight = height;
		this->windowWidth = width;

		// Determine the aspect ratio and sanity check it to a safe ratio
		float aspectRatio = static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight);
		if (aspectRatio < 1.0f) {
			aspectRatio = 4.0f / 3.0f;
		}

        // update projection matrix based on new aspect ratio
        this->ProjectionMatrix = glm::perspective(
            45.0f,
            aspectRatio,
            0.1f,
            10000.0f
            );
    }

} // namespace Sigma

//-----------------------------------------------------------------
// Print for OpenGL errors
//
// Returns 1 if an OpenGL error occurred, 0 otherwise.
//

int printOglError(char *file, int line)
{

	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		std::cerr << "glError in file " << file << " @ line " << line << ": " << gluErrorString(glErr) << std::endl;
		retCode = 1;
	}
	return retCode;
}
