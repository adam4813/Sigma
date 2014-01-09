#include <iostream>

#include "systems/OpenGLSystem.h"
#include "systems/OpenALSystem.h"
#include "systems/BulletPhysics.h"
#include "systems/FactorySystem.h"
#include "controllers/GUIController.h"
#include "controllers/FPSCamera.h"
#include "components/PhysicsController.h"
#include "components/GLScreenQuad.h"
#include "SCParser.h"
#include "systems/WebGUISystem.h"
#include "OS.h"
#include "components/SpotLight.h"

int main(int argCount, char **argValues) {
	Sigma::OS glfwos;
	Sigma::OpenGLSystem glsys;
	Sigma::OpenALSystem alsys;
	Sigma::BulletPhysics bphys;
	Sigma::WebGUISystem webguisys;

	Sigma::FactorySystem& factory = Sigma::FactorySystem::getInstance();
	factory.register_Factory(glsys);
	factory.register_Factory(alsys);
	factory.register_Factory(bphys);
	factory.register_Factory(webguisys);

	if (!glfwos.InitializeWindow(1024, 768, "Sigma GLFW Test Window")) {
		std::cerr << "Failed creating the window or context." << std::endl;
		return -1;
	}

	/////////////////////////////
	// Start the openGL system //
	/////////////////////////////

	std::cout << "Initializing OpenGL system." << std::endl;
	const int* version = glsys.Start();

	glsys.SetViewportSize(glfwos.GetWindowWidth(), glfwos.GetWindowHeight());

	if (version[0] == -1) {
		std::cerr << "Error starting OpenGL!" << std::endl;
		return -1;
	}
	else {
		std::cout << "OpenGL version: " << version[0] << "." << version[1] << std::endl;
	}

	//////////////////////////////
	// Setup deferred rendering //
	//////////////////////////////

	// Create render target for the GBuffer, Light Accumulation buffer, and final composite buffer
	unsigned int geoBuffer = glsys.createRenderTarget(glfwos.GetWindowWidth(), glfwos.GetWindowHeight(), true);
	glsys.createRTBuffer(geoBuffer, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);	// Diffuse texture
	glsys.createRTBuffer(geoBuffer, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);	// Normal texture
	glsys.createRTBuffer(geoBuffer, GL_R32F, GL_RED, GL_FLOAT);				// Depth texture
	glsys.initRenderTarget(geoBuffer); // Create the opengl assets

	///////////////////
	// Setup physics //
	///////////////////

	bphys.Start();

	///////////////
	// Setup GUI //
	///////////////

	webguisys.Start();
	webguisys.SetWindowSize(glfwos.GetWindowWidth(), glfwos.GetWindowHeight());

	/////////////////
	// Setup Sound //
	/////////////////

	std::cout << "Initializing OpenAL system." << std::endl;
	alsys.Start();
	alsys.test(); // try sound
	
	////////////////
	// Load scene //
	////////////////

	// Parse the scene file to retrieve entities
	Sigma::parser::SCParser parser;

	std::cout << "Parsing test.sc scene file." << std::endl;
	if (!parser.Parse("test.sc")) {
		assert(0 && "Failed to load entities from file.");
	}

	std::cout << "Generating Entities." << std::endl;

	// Create each entity's components
	for (unsigned int i = 0; i < parser.EntityCount(); ++i) {
		Sigma::parser::Entity* e = parser.GetEntity(i);
		for (auto itr = e->components.begin(); itr != e->components.end(); ++itr) {

			// Currently, physicsmover components must come after gl* components
			if((*itr).type == "PhysicsMover") {
				Sigma::GLTransform *transform = glsys.GetTransformFor(e->id);
				if(transform) {
					Property p("transform", transform);
					itr->properties.push_back(p);
				}
				else {
					assert(0 && "Invalid entity id");
				}
			}

			factory.create(itr->type,e->id, const_cast<std::vector<Property>&>(itr->properties));
		}
	}

	//////////////////////
	// Setup user input //
	//////////////////////

	// View and ViewMover creation has been moved to test.sc, but for
	// now provide sensible defaults. Final engine should require
	// definition in scene file. Currently entity ID for view must be 1
	// for this to work.

	// No view provided, create a default FPS view
	if(!glsys.GetView()) {
		std::vector<Property> props;

		Property p_x("x", 0.0f);
		Property p_y("y", 0.0f);
		Property p_z("z", 0.0f);

		props.push_back(p_x);
		props.push_back(p_y);
		props.push_back(p_z);

		glsys.createGLView(1, props);
	}

	//Create the controller
	//Perhaps a little awkward currently, should create a generic
	//controller class ancestor
	bphys.initViewMover(*glsys.GetView()->Transform());

	Sigma::event::handler::FPSCamera theCamera(*bphys.getViewMover());
	glsys.GetView()->Transform()->SetEuler(true);
	glsys.GetView()->Transform()->SetMaxRotation(glm::vec3(45.0f,0,0));
	glfwos.RegisterKeyboardEventHandler(&theCamera);
	glfwos.RegisterMouseEventHandler(&theCamera);
	theCamera.os = &glfwos;
	
	// Sync bullet physics object with gl camera

	///////////////////
	// Configure GUI //
	///////////////////

	Sigma::event::handler::GUIController guicon;
	guicon.SetGUI(webguisys.getComponent(100, Sigma::WebGUIView::getStaticComponentTypeName()));
	glfwos.RegisterKeyboardEventHandler(&guicon);
	glfwos.RegisterMouseEventHandler(&guicon);
	
	// Call now to clear the delta after startup.
	glfwos.GetDeltaTime();
	{
		Sigma::ALSound *als = (Sigma::ALSound *)alsys.getComponent(200, Sigma::ALSound::getStaticComponentTypeName());
		if(als) {
			als->Play(Sigma::PLAYBACK_LOOP);
		}
	}

	enum FlashlightState {
		FL_ON,
		FL_TURNING_ON,
		FL_OFF,
		FL_TURNING_OFF
	};

	FlashlightState fs = FL_OFF;

	while (!glfwos.Closing()) {
		// Get time in ms, store it in seconds too
		double deltaSec = glfwos.GetDeltaTime();

		// Process input
		if(glfwos.CheckKeyState(Sigma::event::KS_DOWN, GLFW_KEY_F)) {
			if(fs==FL_OFF) {
				fs=FL_TURNING_ON;
			} else if (fs==FL_ON) {
				fs=FL_TURNING_OFF;
			}
		}

		if(glfwos.CheckKeyState(Sigma::event::KS_UP, GLFW_KEY_F)) {
			if(fs==FL_TURNING_ON) {
				// Enable flashlight
				Sigma::SpotLight *spotlight = static_cast<Sigma::SpotLight *>(glsys.getComponent(151, Sigma::SpotLight::getStaticComponentTypeName()));
				spotlight->enabled = true;
				// Rotate flashlight up
				// Enable spotlight
				fs=FL_ON;
			} else if (fs==FL_TURNING_OFF) {
				// Disable spotlight
				Sigma::SpotLight *spotlight = static_cast<Sigma::SpotLight *>(glsys.getComponent(151, Sigma::SpotLight::getStaticComponentTypeName()));
				spotlight->enabled = false;
				// Rotate flashlight down
				// Disable flashlight
				fs=FL_OFF;
			}
		}

		///////////////////////
		// Update subsystems //
		///////////////////////

		// Pass in delta time in seconds
		bphys.Update(deltaSec);
		webguisys.Update(deltaSec);

		alsys.Update();

		// Update the renderer and present
		if (glsys.Update(deltaSec)) {
			glfwos.SwapBuffers();
		}

		glfwos.OSMessageLoop();
	}

	return 0;
}
