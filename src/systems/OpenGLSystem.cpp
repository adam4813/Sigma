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

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Sigma{
	// RenderTarget methods
	RenderTarget::~RenderTarget() {
		glDeleteTextures(1, &this->texture_id); // Perhaps should check if texture was created for this RT or is used elsewhere
		glDeleteRenderbuffers(1, &this->depth_id);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &this->fbo_id);
	}

	void RenderTarget::Use(int slot) {
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo_id);
	}
	
	std::map<std::string, Sigma::resource::GLTexture> OpenGLSystem::textures;
    OpenGLSystem::OpenGLSystem() : windowWidth(1024), windowHeight(768), deltaAccumulator(0.0),
		framerate(60.0f), viewMode("") {}

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
		retval["GLScreenQuad"] = std::bind(&OpenGLSystem::createScreenQuad,this,_1,_2);

        return retval;
    }

	IComponent* OpenGLSystem::createGLView(const unsigned int entityID, const std::vector<Property> &properties, std::string mode) {
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

		this->views[this->views.size() - 1]->Transform.Move(x,y,z);
		this->views[this->views.size() - 1]->Transform.Rotate(rx,ry,rz);

		return this->views[this->views.size() - 1];
	}

	IComponent* OpenGLSystem::createGLSprite(const unsigned int entityID, const std::vector<Property> &properties) {
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

	IComponent* OpenGLSystem::createGLIcoSphere(const unsigned int entityID, const std::vector<Property> &properties) {
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

	IComponent* OpenGLSystem::createGLCubeSphere(const unsigned int entityID, const std::vector<Property> &properties) {
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

	IComponent* OpenGLSystem::createGLMesh(const unsigned int entityID, const std::vector<Property> &properties) {
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
			else if (p->GetName() == "shader"){
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

	IComponent* OpenGLSystem::createScreenQuad(const unsigned int entityID, const std::vector<Property> &properties) {
		Sigma::GLScreenQuad* quad = new Sigma::GLScreenQuad(entityID);

		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;
		int componentID = 0;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);
			if (p->GetName() == "left") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "top") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "right") {
				w = p->Get<float>();
			}
			else if (p->GetName() == "bottom") {
				h = p->Get<float>();
			}
		}

		quad->SetPosition(x, y);
		quad->SetSize(w, h);
		quad->LoadShader("shaders/quad");
		quad->InitializeBuffers();
		this->addComponent(entityID,quad);
		return quad;
	}

	IComponent* OpenGLSystem::createPointLight(const unsigned int entityID, const std::vector<Property> &properties) {
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

	int OpenGLSystem::createRenderTarget(const unsigned int w, const unsigned int h, const unsigned int format) {
		std::unique_ptr<RenderTarget> newRT(new RenderTarget());

		glGenTextures(1, &newRT->texture_id);
		glBindTexture(GL_TEXTURE_2D, newRT->texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//NULL means reserve texture memory, but texels are undefined
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, (GLsizei)w, (GLsizei)h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

		glGenFramebuffers(1, &newRT->fbo_id);
		glBindFramebuffer(GL_FRAMEBUFFER, newRT->fbo_id);
		
		//Attach 2D texture to this FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newRT->texture_id, 0);
		
		glGenRenderbuffers(1, &newRT->depth_id);
		glBindRenderbuffer(GL_RENDERBUFFER, newRT->depth_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
		
		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newRT->depth_id);
		
		//Does the GPU support current FBO configuration?
		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		switch(status) {
			case GL_FRAMEBUFFER_COMPLETE:
				std::cout << "Successfully created render target.";
				break;
			default:
				std::cerr << "Error: Framebuffer format is not compatible." << std::endl;
		}

		// Unbind objects
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		this->renderTargets.push_back(std::move(newRT));
		return (this->renderTargets.size() - 1);
	}

    bool OpenGLSystem::Update(const double delta) {
        this->deltaAccumulator += delta;

        // Check if the deltaAccumulator is greater than 1/<framerate>th of a second.
        //  ..if so, it's time to render a new frame
        if (this->deltaAccumulator > 1000.0 / this->framerate) {
            
			// Hacky for now, but if we created at least one render target
			// then the 0th one is the draw buffer, 1+ could be for post-processing
			if(this->renderTargets.size() > 0) {
				// Bind the primary render target
				glBindFramebuffer(GL_FRAMEBUFFER, this->renderTargets[0]->fbo_id);
			}

			glm::vec3 viewPosition;

            // Set up the scene to a "clean" state.
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

			glm::mat4 viewMatrix;
			if (this->views.size() > 0) {
				viewMatrix = this->views[this->views.size() - 1]->GetViewMatrix();
				viewPosition = this->views[this->views.size() - 1]->Transform.GetPosition();
			}

			// Loop through each light, rendering all components
			// TODO: Cull components based on light
			// TODO: Implement scissors test
			// Potentially move to deferred shading depending on
			// visual style needs
			
			// Ambient Pass
			// Loop through and draw each component.

			glm::mat4 viewProj = viewMatrix;
			viewProj *= this->ProjectionMatrix;

			// Calculate frustum for culling
			this->GetView(0)->CalculateFrustum(viewProj);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
				for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
					IGLComponent *glComp = dynamic_cast<IGLComponent *>(citr->second.get());

					if(glComp) {
						glComp->GetShader()->Use();

						// Set view position
						glUniform3f(glGetUniformBlockIndex(glComp->GetShader()->GetProgram(), "viewPosW"), viewPosition.x, viewPosition.y, viewPosition.z);

						// For now, turn on ambient intensity and turn off lighting
						glUniform1f(glGetUniformLocation(glComp->GetShader()->GetProgram(), "ambLightIntensity"), 0.05f);
						glUniform1f(glGetUniformLocation(glComp->GetShader()->GetProgram(), "diffuseLightIntensity"), 0.0f);
						glUniform1f(glGetUniformLocation(glComp->GetShader()->GetProgram(), "specularLightIntensity"), 0.0f);
						glComp->Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);
					}
				}
			}

			// Light passes
			for(auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
				for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
					// Check if this component is a light
					PointLight *light = dynamic_cast<PointLight*>(citr->second.get());

					// If it is a light, and it intersects the frustum, then render
					if(light/* && this->GetView(0)->CameraFrustum.isectSphere(light->position, light->radius)*/) {
						// Modify depth test to allow for overlaying
						// lights
						glDepthFunc(GL_EQUAL);

						// Make sure additive blending is enabled
						glEnable(GL_BLEND);
						glBlendFunc(GL_ONE, GL_ONE);

						// Loop through and draw each component.
						for (auto child_eitr = this->_Components.begin(); child_eitr != this->_Components.end(); ++child_eitr) {
							for (auto child_citr = child_eitr->second.begin(); child_citr != child_eitr->second.end(); ++child_citr) {
								IGLComponent *glComp = dynamic_cast<IGLComponent *>(child_citr->second.get());

								if(glComp && glComp->IsLightingEnabled()) {
									glComp->GetShader()->Use();

									// Turn off ambient light for additive blending
									glUniform1f(glGetUniformLocation(glComp->GetShader()->GetProgram(), "ambLightIntensity"), 0.0f);

									// Set view position
									glUniform3f(glGetUniformBlockIndex(glComp->GetShader()->GetProgram(), "viewPosW"), viewPosition.x, viewPosition.y, viewPosition.z);

									// Activate the current point light for this shader
									light->Activate(glComp->GetShader().get());

									// Render
									glComp->Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);
								}
							}
						}

						// Remove blending
						glDisable(GL_BLEND);

						// Re-enabled depth test
						glDepthFunc(GL_LESS);
					}
				}
			}

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
		if (GLEW_AMD_seamless_cubemap_per_texture) {
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // allows for cube-mapping without seams
		}
		if (GLEW_ARB_multisample) {
			glEnable(GL_MULTISAMPLE_ARB);
		}
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

		// Create main framebuffer (index 0)
		//this->createRenderTarget(1024, 768, GL_RGBA8);

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
