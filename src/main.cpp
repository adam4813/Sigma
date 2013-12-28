#include <iostream>

#include "systems/OpenGLSystem.h"
#include "systems/BulletPhysics.h"
#include "systems/FactorySystem.h"
#include "controllers/GLSixDOFViewController.h"
#include "controllers/GUIController.h"
#include "controllers/FPSCamera.h"
#include "components/BulletMover.h"
#include "components/GLScreenQuad.h"
#include "SCParser.h"
#include "systems/WebGUISystem.h"
#include "OS.h"
#include "components/SpotLight.h"

int main(int argCount, char **argValues) {
	Sigma::OS glfwos;
	Sigma::OpenGLSystem glsys;
	Sigma::BulletPhysics bphys;
	Sigma::WebGUISystem webguisys;

	Sigma::FactorySystem& factory = Sigma::FactorySystem::getInstance();
	factory.register_Factory(glsys);
	factory.register_Factory(bphys);
	factory.register_Factory(webguisys);

	if (!glfwos.InitializeWindow(1024,768, "Sigma GLFW Test Window")) {
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
	glsys.createRTBuffer(geoBuffer, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE); // Diffuse texture
	glsys.createRTBuffer(geoBuffer, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE); // Normal texture
	glsys.createRTBuffer(geoBuffer, GL_R32F, GL_RED, GL_FLOAT);			  // Depth texture
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
				GLTransform *transform = glsys.GetTransformFor(e->id);
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
	// now provide sensible defaults.  Final engine should require
	// definition in scene file.  Currently entity ID for view must be 1
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

		glsys.createGLView(1, props, "FPSCamera");
	}

	// Still hard coded to use entity ID #1
	// Link the graphics view to the physics system's view mover
	Sigma::BulletMover* mover = bphys.getViewMover();

	//Create the controller
	//Perhaps a little awkward currently, should create a generic
	//controller class ancestor
	if(glsys.GetViewMode() == "FPSCamera") {
		using Sigma::event::handler::FPSCamera;
		FPSCamera* theCamera = static_cast<FPSCamera*>(glsys.GetView());
		glfwos.RegisterKeyboardEventHandler(theCamera);
		glfwos.RegisterMouseEventHandler(theCamera);
		theCamera->SetMover(mover);
		//mover->SetTransform(*theCamera->Transform());
	} else if (glsys.GetViewMode() == "GLSixDOFView") {
		Sigma::event::handler::GLSixDOFViewController cameraController(glsys.GetView(), mover);
		glfwos.RegisterKeyboardEventHandler(&cameraController);
	}
	
	// Sync bullet physics object with gl camera
	bphys.initViewMover();

	///////////////////
	// Configure GUI //
	///////////////////

	Sigma::event::handler::GUIController guicon;
	guicon.SetGUI(webguisys.getComponent(100, Sigma::WebGUIView::getStaticComponentTypeName()));
	glfwos.RegisterKeyboardEventHandler(&guicon);
	glfwos.RegisterMouseEventHandler(&guicon);

	Sigma::event::handler::GUIController guicon2;
	guicon2.SetGUI(webguisys.getComponent(101, Sigma::WebGUIView::getStaticComponentTypeName()));
	glfwos.RegisterKeyboardEventHandler(&guicon2);
	glfwos.RegisterMouseEventHandler(&guicon2);

	Sigma::event::handler::GUIController guicon3;
	guicon3.SetGUI(webguisys.getComponent(102, Sigma::WebGUIView::getStaticComponentTypeName()));

	glfwos.RegisterKeyboardEventHandler(&guicon3);
	glfwos.RegisterMouseEventHandler(&guicon3);
	
	// Call now to clear the delta after startup.
	glfwos.GetDeltaTime();

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

		// Framebuffer(s) to draw
		//int fbos[2];
		//fbos[0] = glsys.getRenderTarget(geoBuffer);
		//fbos[1] = glsys.getRenderTarget(lightBuffer);

		// Update the renderer and present
		if (glsys.Update(deltaSec)) {
			glfwos.SwapBuffers();
		}

		glfwos.OSMessageLoop();
	}

	return 0;
}
