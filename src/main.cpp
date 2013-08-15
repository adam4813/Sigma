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
	glsys.Factory("GLSprite", 1);
	glsys.Factory("GLSprite", 2);

	win.SetupTimer();
	double delta;
	while (win.MessageLoop()) {
		delta = win.GetDeltaTime();
		glsys.Update(delta); // Render our scene (which also handles swapping of buffers)

		// Translation keys
		if (win.KeyDown('W')) { // Forward
			glsys.Translate(0.0f,0.0f,10.0f * (float)delta / 1000.0f);
		} else if (win.KeyDown('S')) { // Backward
			glsys.Translate(0.0f,0.0f,-10.0f * (float)delta / 1000.0f);
		}
		if (win.KeyDown('A')) { 
			//glsys.Translate(-10.0f * (float)delta / 1000.0f,0.0f,0.0f); // Strafe Left
			glsys.Rotate(0.0f,-90.0f * (float)delta / 1000.0f,0.0f); // Yaw left.
		} else if (win.KeyDown('D')) {
			//glsys.Translate(10.0f * (float)delta / 1000.0f,0.0f,0.0f); // Strafe Right
			glsys.Rotate(0.0f,90.0f * (float)delta / 1000.0f,0.0f); // Yaw right.
		}
		if (win.KeyDown('E')) { // Up
			glsys.Translate(0.0f,10.0f * (float)delta / 1000.0f,0.0f);
		} else if (win.KeyDown('C')) { // Down
			glsys.Translate(0.0f,-10.0f * (float)delta / 1000.0f,0.0f);
		}
		if (win.KeyDown('Q')) { // Pitch Up
			glsys.Rotate(90.0f * (float)delta / 1000.0f,0.0f,0.0f);
		} else if (win.KeyDown('Z')) { // Pitch Down
			glsys.Rotate(-90.0f * (float)delta / 1000.0f,0.0f,0.0f);
		}
	}

	return 0;
}
