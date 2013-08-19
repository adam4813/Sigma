#include <iostream>

#include "Systems/OpenGLSystem.h"
#include "win32.h"

int main(int argCount, char **argValues) {
	OpenGLSystem glsys;
	win32 win;
	const int* version = glsys.Start(win.CreateWin32Window());
	if (version[0] == -1) {
		std::cout<< "Error starting OpenGL!"<<std::endl;
	} else {
		std::cout<<"OpenGL version: " << version[0] << "." << version[1] << std::endl;
	}
	std::vector<Property> props;
	glsys.Factory("GLSprite", 1, props);
	glsys.Factory("GLSprite", 2, props);
	Property prop1("scale");
	prop1.Set<float>(10.0f);
	props.push_back(prop1);
	glsys.Factory("GLIcoSphere", 0, props);

	win.SetupTimer();
	double delta;
	while (win.MessageLoop()) {
		delta = win.GetDeltaTime();
		glsys.Update(delta); // Render our scene (which also handles swapping of buffers)

		// Translation keys
		if (win.KeyDown('W')) { // Move forward
			glsys.Translate(0.0f,0.0f,10.0f * (float)delta / 1000.0f);
		} else if (win.KeyDown('S')) { // Move backward
			glsys.Translate(0.0f,0.0f,-10.0f * (float)delta / 1000.0f);
		}
		if (win.KeyDown('A')) { 
			glsys.Rotate(0.0f,-90.0f * (float)delta / 1000.0f,0.0f); // Yaw left.
		} else if (win.KeyDown('D')) {
			glsys.Rotate(0.0f,90.0f * (float)delta / 1000.0f,0.0f); // Yaw right.
		}
		if (win.KeyDown('F')) { 
			glsys.Translate(-10.0f * (float)delta / 1000.0f,0.0f,0.0f); // Strafe Left
		} else if (win.KeyDown('G')) {
			glsys.Translate(10.0f * (float)delta / 1000.0f,0.0f,0.0f); // Strafe Right
		}
		if (win.KeyDown('E')) { // Move up
			glsys.Translate(0.0f,10.0f * (float)delta / 1000.0f,0.0f);
		} else if (win.KeyDown('C')) { // Move down
			glsys.Translate(0.0f,-10.0f * (float)delta / 1000.0f,0.0f);
		}
		if (win.KeyDown('Q')) { // Pitch Up
			glsys.Rotate(-90.0f * (float)delta / 1000.0f,0.0f,0.0f);
		} else if (win.KeyDown('Z')) { // Pitch Down
			glsys.Rotate(90.0f * (float)delta / 1000.0f,0.0f,0.0f);
		}

		if (win.KeyDown('R')) { // Roll left
			glsys.Rotate(0.0f,0.0f,-90.0f * (float)delta / 1000.0f);
		} else if (win.KeyDown('T')) { // Roll right
			glsys.Rotate(0.0f,0.0f,90.0f * (float)delta / 1000.0f);
		}
	}

	return 0;
}
