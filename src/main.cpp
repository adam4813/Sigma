#include <iostream>

#include "systems/OpenGLSystem.h"
#include "systems/SimplePhysics.h"
#include "systems/FactorySystem.h"
#include "controllers/GLSixDOFViewController.h"
#include "components/ViewMover.h"
#include "SCParser.h"

#if defined OS_Win32
#include "os/win32/win32.h"
#elif defined OS_SDL
#include "os/sdl/SDLSys.h"
#endif

int main(int argCount, char **argValues) {
	Sigma::OpenGLSystem glsys;
	SimplePhysics physys;
	Sigma::FactorySystem& factory = Sigma::FactorySystem::getInstance();
	factory.register_Factory(glsys);
	factory.register_Factory(physys);
	IOpSys* os = nullptr;
#if defined OS_Win32
	os = new win32();
#elif defined OS_SDL
	os = new SDLSys();
#endif

	if(os->CreateGraphicsWindow(1024,768) == 0) {
		std::cout << "Error creating window!" << std::endl;
	}

	const int* version = glsys.Start();
	glsys.SetViewportSize(os->GetWindowWidth(), os->GetWindowHeight());

	if (version[0] == -1) {
		std::cout << "Error starting OpenGL!" << std::endl;
	} else {
		std::cout << "OpenGL version: " << version[0] << "." << version[1] << std::endl;
	}

	Sigma::parser::SCParser parser;

	if (!parser.Parse("test.sc")) {
		assert(0 && "Failed to load entities from file.");
	}

	for (unsigned int i = 0; i < parser.EntityCount(); ++i) {
		const Sigma::parser::Entity* e = parser.GetEntity(i);
		for (auto itr = e->components.begin(); itr != e->components.end(); ++itr) {
            factory.create(
                        itr->type,e->id,
                        const_cast<std::vector<Property>&>(itr->properties));
		}
	}

	std::vector<Property> props;
	physys.createViewMover(9, props);
	ViewMover* mover = reinterpret_cast<ViewMover*>(physys.getComponent(9,ViewMover::getStaticComponentID()));
	Sigma::event::handler::GLSixDOFViewController cameraController(glsys.View(), mover);
	IOpSys::KeyboardEventSystem.Register(&cameraController);

	os->SetupTimer();

	double delta;
	bool isWireframe=false;

	while (os->MessageLoop()) {
		delta = os->GetDeltaTime();
		double deltaSec = (double)delta/1000.0f;

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

		// Pass in delta time in seconds
		physys.Update(deltaSec);

		if (glsys.Update(delta)) {
			os->Present();
		}
	}

	delete os;

	return 0;
}
