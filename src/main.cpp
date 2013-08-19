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
	{
		Property prop1("scale");
		prop1.Set<float>(100.0f);
		props.push_back(prop1);
		Property prop2("x");
		prop2.Set(0.0f);
		props.push_back(prop2);
		Property prop3("y");
		prop3.Set<float>(0.0f);
		props.push_back(prop3);
		Property prop4("z");
		prop4.Set<float>(300.0f);
		props.push_back(prop4);
		glsys.Factory("GLIcoSphere", 3, props);
	}
	props.clear();
	{
		Property prop1("scale");
		prop1.Set<float>(200.0f);
		props.push_back(prop1);
		Property prop2("x");
		prop2.Set<float>(500.0f);
		props.push_back(prop2);
		Property prop3("y");
		prop3.Set<float>(0.0f);
		props.push_back(prop3);
		Property prop4("z");
		prop4.Set<float>(500.0f);
		props.push_back(prop4);
		glsys.Factory("GLIcoSphere", 4, props);
	}

	win.SetupTimer();
	double delta;
	while (win.MessageLoop()) {
		delta = win.GetDeltaTime();
		glsys.Update(delta); // Render our scene (which also handles swapping of buffers)

		// Translation keys
		if (win.KeyDown('W')) { // Move forward
			if (win.KeyDown('B')) {
				glsys.Move(0.0f,0.0f,100.0f * (float)delta / 1000.0f);
			} else {
				glsys.Move(0.0f,0.0f,10.0f * (float)delta / 1000.0f);
			}
		} else if (win.KeyDown('S')) { // Move backward
			glsys.Move(0.0f,0.0f,-10.0f * (float)delta / 1000.0f);
		}
		if (win.KeyDown('A')) { 
			glsys.Rotate(0.0f,-90.0f * (float)delta / 1000.0f,0.0f); // Yaw left.
		} else if (win.KeyDown('D')) {
			glsys.Rotate(0.0f,90.0f * (float)delta / 1000.0f,0.0f); // Yaw right.
		}
		if (win.KeyDown('F')) { 
			glsys.Move(-10.0f * (float)delta / 1000.0f,0.0f,0.0f); // Strafe Left
		} else if (win.KeyDown('G')) {
			glsys.Move(10.0f * (float)delta / 1000.0f,0.0f,0.0f); // Strafe Right
		}
		if (win.KeyDown('E')) { // Move up
			glsys.Move(0.0f,10.0f * (float)delta / 1000.0f,0.0f);
		} else if (win.KeyDown('C')) { // Move down
			glsys.Move(0.0f,-10.0f * (float)delta / 1000.0f,0.0f);
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
