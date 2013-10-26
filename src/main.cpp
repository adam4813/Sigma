#include <iostream>

#include "systems/OpenGLSystem.h"
#include "systems/BulletPhysics.h"
#include "systems/FactorySystem.h"
#include "controllers/GLSixDOFViewController.h"
#include "controllers/FPSCamera.h"
#include "components/BulletMover.h"
#include "SCParser.h"

#if defined OS_Win32
#include "os/win32/win32.h"
#elif defined OS_SDL
#include "os/sdl/SDLSys.h"
#endif

int main(int argCount, char **argValues) {
	Sigma::OpenGLSystem glsys;
	Sigma::BulletPhysics bphys;

	Sigma::FactorySystem& factory = Sigma::FactorySystem::getInstance();
	factory.register_Factory(glsys);
	factory.register_Factory(bphys);

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

            factory.create(
                        itr->type,e->id,
                        const_cast<std::vector<Property>&>(itr->properties));
		}
	}

	// View and ViewMover creation has been moved to test.sc, but for
	// now provide sensible defaults.  Final engine should require
	// definition in scene file.  Currently entity ID for view must be 1
	// for this to work.

	// No view provided, create a default FPS view
	if(!glsys.View()) {
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
		Sigma::event::handler::FPSCamera cameraController(mover);
		IOpSys::KeyboardEventSystem.Register(&cameraController);
		IOpSys::MouseEventSystem.Register(&cameraController);
	} else if (glsys.GetViewMode() == "GLSixDOFView") {
		Sigma::event::handler::GLSixDOFViewController cameraController(glsys.View(), mover);
		IOpSys::KeyboardEventSystem.Register(&cameraController);
	}

	// Setup the timer
	os->SetupTimer();

	// Begin main loop
	double delta;
	bool isWireframe=false;

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

		// Update the renderer and present
		if (glsys.Update(delta)) {
			os->Present();
		}
	}

	delete os;
	return 0;
}
