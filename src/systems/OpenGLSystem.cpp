#include "systems/OpenGLSystem.h"
#include "systems/GLSLShader.h"
#include "systems/GLSixDOFView.h"
#include "systems/GLFPSView.h"
#include "components/GLSprite.h"
#include "components/GLIcoSphere.h"
#include "components/GLCubeSphere.h"
#include "components/GLMesh.h"
#include "controllers/FPSCamera.h"

namespace Sigma{
    OpenGLSystem::OpenGLSystem() : windowWidth(800), windowHeight(600), deltaAccumulator(0.0),
		framerate(60.0f), view(nullptr), viewMode("") {}

	std::map<std::string, Sigma::IFactory::FactoryFunction>
        OpenGLSystem::getFactoryFunctions() {
		using namespace std::placeholders;

		std::map<std::string, Sigma::IFactory::FactoryFunction> retval;
		retval["GLSprite"] = std::bind(&OpenGLSystem::createGLSprite,this,_1,_2);
		retval["GLIcoSphere"] = std::bind(&OpenGLSystem::createGLIcoSphere,this,_1,_2);
		retval["GLCubeSphere"] = std::bind(&OpenGLSystem::createGLCubeSphere,this,_1,_2);
		retval["GLMesh"] = std::bind(&OpenGLSystem::createGLMesh,this,_1,_2);
		retval["GLFPSView"] = std::bind(&OpenGLSystem::createGLView,this,_1,_2, "GLFPSView");
		retval["GLSixDOFView"] = std::bind(&OpenGLSystem::createGLView,this,_1,_2, "GLSixDOFView");

        return retval;
    }

	void OpenGLSystem::createGLView(const unsigned int entityID, std::vector<Property> &properties, std::string mode) {
		viewMode = mode;

		if(mode=="FPSCamera") {
			this->view = std::unique_ptr<IGLView>(new Sigma::event::handler::FPSCamera());
		} else if(mode=="GLSixDOFView") {
			this->view = std::unique_ptr<IGLView>(new GLSixDOFView());
		} else {
			std::cerr << "Invalid view type!" << std::endl;
		}

		float x=0.0f, y=0.0f, z=0.0f, rx=0.0f, ry=0.0f, rz=0.0f;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &(*propitr);

			if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			}else if (p->GetName() == "rx") {
				rx = p->Get<float>();
				continue;
			} else if (p->GetName() == "ry") {
				ry = p->Get<float>();
				continue;
			} else if (p->GetName() == "rz") {
				rz = p->Get<float>();
				continue;
			}
		}

		this->view->Transform.Move(x,y,z);
		this->view->Transform.Rotate(rx,ry,rz);
	}

	void OpenGLSystem::createGLSprite(const unsigned int entityID, std::vector<Property> &properties) {
		GLSprite* spr = new GLSprite(entityID);
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		int componentID = 0;
		std::string textureFilename;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &(*propitr);
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
			} else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			} else if (p->GetName() == "textureFilename"){
				textureFilename = p->Get<std::string>();
			}
		}
		spr->LoadTexture(textureFilename);
		spr->Transform()->Scale(glm::vec3(scale));
		spr->Transform()->Translate(x,y,z);
		spr->InitializeBuffers();
		this->addComponent(entityID,spr);
	}

	void OpenGLSystem::createGLIcoSphere(const unsigned int entityID, std::vector<Property> &properties) {
			Sigma::GLIcoSphere* sphere = new Sigma::GLIcoSphere(entityID);
			float scale = 1.0f;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			int componentID = 0;
			std::string shader_name = "shaders/icosphere";

			for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
				Property*  p = &(*propitr);
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
				} else if (p->GetName() == "id") {
					componentID = p->Get<int>();
				} else if (p->GetName() == "shader"){
					shader_name = p->Get<std::string>();
				}
			}
			sphere->Transform()->Scale(scale,scale,scale);
			sphere->Transform()->Translate(x,y,z);
			sphere->LoadShader(shader_name);
			sphere->InitializeBuffers();
			sphere->SetCullFace("back");
			this->addComponent(entityID,sphere);
	}

		void OpenGLSystem::createGLCubeSphere(const unsigned int entityID, std::vector<Property> &properties) {
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
				Property*  p = &(*propitr);
				if (p->GetName() == "scale") {
					scale = p->Get<float>();
				} else if (p->GetName() == "x") {
					x = p->Get<float>();
				} else if (p->GetName() == "y") {
					y = p->Get<float>();
				} else if (p->GetName() == "z") {
					z = p->Get<float>();
				} else if (p->GetName() == "rx") {
					rx = p->Get<float>();
				} else if (p->GetName() == "ry") {
					ry = p->Get<float>();
				} else if (p->GetName() == "rz") {
					rz = p->Get<float>();
				} else if (p->GetName() == "subdivision_levels") {
					subdivision_levels = p->Get<int>();
				} else if (p->GetName() == "texture") {
					texture_name = p->Get<std::string>();
				} else if (p->GetName() == "shader") {
					shader_name = p->Get<std::string>();
				} else if (p->GetName() == "id") {
					componentID = p->Get<int>();
				} else if (p->GetName() == "cullface") {
					cull_face = p->Get<std::string>();
				} else if (p->GetName() == "fix_to_camera") {
					fix_to_camera = p->Get<bool>();
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
	}

	void OpenGLSystem::createGLMesh(const unsigned int entityID, std::vector<Property> &properties) {
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
			} else if (p->GetName() == "rx") {
				rx = p->Get<float>();
				continue;
			} else if (p->GetName() == "ry") {
				ry = p->Get<float>();
				continue;
			} else if (p->GetName() == "rz") {
				rz = p->Get<float>();
				continue;
			} else if (p->GetName() == "meshFile") {
				std::cerr << "Loading mesh: " << p->Get<std::string>() << std::endl;
				mesh->LoadMesh(p->Get<std::string>());
			} else if (p->GetName() == "shader"){
				shaderfile = p->Get<std::string>();
			} else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			} else if (p->GetName() == "cullface") {
				cull_face = p->Get<std::string>();
			}
		}

		mesh->SetCullFace(cull_face);
		mesh->Transform()->Scale(scale,scale,scale);
		mesh->Transform()->Translate(x,y,z);
		mesh->Transform()->Rotate(rx,ry,rz);
		if(shaderfile != "") mesh->LoadShader(shaderfile);
		else mesh->LoadShader(); // load default
        mesh->InitializeBuffers();
        this->addComponent(entityID,mesh);
    }

    bool OpenGLSystem::Update(const double delta) {
        this->deltaAccumulator += delta;

        // Check if the deltaAccumulator is greater than 1/<framerate>th of a second.
        //  ..if so, it's time to render a new frame
        if (this->deltaAccumulator > 1000.0 / this->framerate) {
            // Set up the scene to a "clean" state.
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

			glm::mat4 viewMatrix = this->view->GetViewMatrix();

            // Loop through and draw each component.
            for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
                for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
                    citr->second->Render(&viewMatrix[0][0], &this->ProjectionMatrix[0][0]);
                }
            }

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
			GLTransform *transform = ((*compItr).second)->Transform();
			return transform;
		}

		// no components
		return 0;
	}

    const int* OpenGLSystem::Start() {

        // Use the GL3 way to get the version number
        glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
        glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

        // Generate a projection matrix (the "view") based on basic window dimensions
        this->ProjectionMatrix = glm::perspective(
            45.0f, // field-of-view (height)
            (float)this->windowWidth / (float)this->windowHeight, // aspect ratio
            0.1f, // near culling plane
            10000.0f // far culling plane
            );
        //this->view->Move(0.0f,1.0f,0.0f);

        // App specific global gl settings
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // allows for cube-mapping without seams
        glEnable(GL_MULTISAMPLE_ARB);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        return OpenGLVersion;
    }

	void OpenGLSystem::Move(float x, float y, float z) {
		this->view->Transform.Move(x,y,z);
	}

	void OpenGLSystem::Rotate(float x, float y, float z) {
		this->view->Transform.Rotate(x,y,z);
	}

    void OpenGLSystem::SetViewportSize(const unsigned int width, const unsigned int height) {
        this->windowHeight = height;
        this->windowWidth = width;
        // update projection matrix based on new aspect ratio
        this->ProjectionMatrix = glm::perspective(
            45.0f,
            (float)this->windowWidth / (float)this->windowHeight,
            0.1f,
            10000.0f
            );
    }
} // namespace Sigma
