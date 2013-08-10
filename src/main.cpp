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

	while (win.MessageLoop()) {
		glsys.Update(0); // Render our scene (which also handles swapping of buffers)
	}

	return 0;
}
