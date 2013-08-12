#include <iostream>

#include "Systems/OpenGLSystem.h"
#include "win32.h"

int main(int argCount, char **argValues) {
	OpenGLSystem glsys;
	win32 win;
	win.SetupTimer();
	const int* version = glsys.Start(win.CreateWin32Window());
	if (version[0] == -1) {
		std::cout<< "Error starting OpenGL!"<<std::endl;
	} else {
		std::cout<<"OpenGL version: " << version[0] << "." << version[1] << std::endl;
	}
	glsys.Factory("GLSprite", 1);
	glsys.Factory("GLSprite", 2);

	while (win.MessageLoop()) {
		glsys.Update(win.GetDeltaTime()); // Render our scene (which also handles swapping of buffers)
	}

	return 0;
}
