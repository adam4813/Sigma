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

#if defined OS_Win32
#include "os/win32/win32.h"
#elif defined OS_SDL
#include "os/sdl/SDLSys.h"
#endif

int main(int argCount, char **argValues) {
	Sigma::OpenGLSystem glsys;
	Sigma::BulletPhysics bphys;
	Sigma::WebGUISystem webguisys;

	Sigma::FactorySystem& factory = Sigma::FactorySystem::getInstance();
	factory.register_Factory(glsys);
	factory.register_Factory(bphys);
	factory.register_Factory(webguisys);

	IOpSys* os = nullptr;

#if defined OS_Win32
	os = new win32();
#elif defined OS_SDL
	os = new SDLSys();
#endif

	// Create the window
	std::cout << "Creating graphics window." << std::endl;
	if(os->CreateGraphicsWindow(1024,768) == 0) {
		std::cerr << "Error creating window!" << std::endl;
		delete os;
		return -1;
	}

	// Start the openGL system
	std::cout << "Initializing OpenGL system." << std::endl;
	const int* version = glsys.Start();
	glsys.SetViewportSize(os->GetWindowWidth(), os->GetWindowHeight());

	if (version[0] == -1) {
		std::cerr << "Error starting OpenGL!" << std::endl;
		delete os;
		return -1;
	} else {
		std::cout << "OpenGL version: " << version[0] << "." << version[1] << std::endl;
	}

	bphys.Start();

	webguisys.Start();
	webguisys.SetWindowSize(os->GetWindowWidth(), os->GetWindowHeight());

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

	// Create the controller
	// Perhaps a little awkward currently, should create a generic
	// controller class ancestor
	if(glsys.GetViewMode() == "FPSCamera") {
	    using Sigma::event::handler::FPSCamera;
        FPSCamera* theCamera = static_cast<FPSCamera*>(glsys.GetView());
		IOpSys::KeyboardEventSystem.Register(theCamera);
		IOpSys::MouseEventSystem.Register(theCamera);
		theCamera->SetMover(mover);
		mover->SetTransform(theCamera->Transform);
	} else if (glsys.GetViewMode() == "GLSixDOFView") {
		Sigma::event::handler::GLSixDOFViewController cameraController(glsys.GetView(), mover);
		IOpSys::KeyboardEventSystem.Register(&cameraController);
	}

	Sigma::event::handler::GUIController guicon;
	guicon.SetGUI(webguisys.getComponent(100, Sigma::WebGUIView::getStaticComponentTypeName()));
	IOpSys::KeyboardEventSystem.Register(&guicon);
	IOpSys::MouseEventSystem.Register(&guicon);

	Sigma::event::handler::GUIController guicon2;
	guicon2.SetGUI(webguisys.getComponent(101, Sigma::WebGUIView::getStaticComponentTypeName()));
	IOpSys::KeyboardEventSystem.Register(&guicon2);
	IOpSys::MouseEventSystem.Register(&guicon2);

	Sigma::event::handler::GUIController guicon3;
	guicon3.SetGUI(webguisys.getComponent(102, Sigma::WebGUIView::getStaticComponentTypeName()));
	IOpSys::KeyboardEventSystem.Register(&guicon3);
	IOpSys::MouseEventSystem.Register(&guicon3);

	// Setup the timer
	os->SetupTimer();

	// Begin main loop
	double delta;
	bool isWireframe=false;

	// Load a font
	while (os->MessageLoop()) {

		// Get time in ms, store it in seconds too
		delta = os->GetDeltaTime();
		double deltaSec = (double)delta/1000.0f;

		// Debug keys
		if (os->KeyReleased('P', true)) { // Wireframe mode
			if (!isWireframe) {
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				isWireframe = true;
			} else {
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				isWireframe = false;
			}
		}

		if (os->KeyReleased('M', true)) {
			os->ToggleFullscreen();
			glsys.SetViewportSize(os->GetWindowWidth(), os->GetWindowHeight());
		}

		// Temporary exit key for when mouse is under control
		if (os->KeyReleased(Sigma::event::KEY_ESCAPE, true)) {
			break;
		}

		// Pass in delta time in seconds
		bphys.Update(deltaSec);
		webguisys.Update(deltaSec);

		// Update the renderer and present
		if (glsys.Update(delta)) {
			os->Present(glsys.getRender());
		}
	}

	delete os;
	return 0;
}
