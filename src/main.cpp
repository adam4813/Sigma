#include <iostream>
#include "sdl.h"
#include "SDL_image.h"
#include "SDLSys.h"

#include "Systems/OpenGLSystem.h"

#ifdef WIN32
#include "win32.h"
#endif

int main(int argCount, char **argValues) {
	OpenGLSystem glsys;
	IOpSys* os = nullptr;
//#ifdef WIN32
//	os = new win32();
//#else
	os = new SDLSys();
//#endif

	os->CreateGraphicsWindow();

	const int* version = glsys.Start();
	
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
		prop2.Set(-500.0f);
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
		prop1.Set<float>(1000.0f);
		props.push_back(prop1);
		Property prop2("x");
		prop2.Set<float>(-500.0f);
		props.push_back(prop2);
		Property prop3("y");
		prop3.Set<float>(0.0f);
		props.push_back(prop3);
		Property prop4("z");
		prop4.Set<float>(8000.0f);
		props.push_back(prop4);
		glsys.Factory("GLIcoSphere", 4, props);
	}
	
	props.clear();
	{
		Property prop1("scale");
		prop1.Set<float>(500.0f);
		props.push_back(prop1);
		Property prop2("x");
		prop2.Set<float>(-500.0f);
		props.push_back(prop2);
		Property prop3("y");
		prop3.Set<float>(0.0f);
		props.push_back(prop3);
		Property prop4("z");
		prop4.Set<float>(2000.0f);
		props.push_back(prop4);
		glsys.Factory("GLIcoSphere", 5, props);
	}

	{
		Property prop1("scale");
		prop1.Set<float>(1500.0f);
		props.push_back(prop1);
		Property prop2("x");
		prop2.Set(2000.0f);
		props.push_back(prop2);
		Property prop3("y");
		prop3.Set<float>(0.0f);
		props.push_back(prop3);
		Property prop4("z");
		prop4.Set<float>(1000.0f);
		props.push_back(prop4);
		glsys.Factory("GLCubeSphere", 3, props);
	}
	
	props.clear();
	{
		Property prop1("scale");
		prop1.Set<float>(1.0f);
		props.push_back(prop1);
		Property prop2("x");
		prop2.Set<float>(30.0f);
		props.push_back(prop2);
		Property prop3("y");
		prop3.Set<float>(0.0f);
		props.push_back(prop3);
		Property prop4("z");
		prop4.Set<float>(0.0f);
		props.push_back(prop4);
		Property prop5("meshFile");
		prop5.Set<std::string>("trillek_dev_clonk2u_tri.obj");
		props.push_back(prop5);
		glsys.Factory("GLMesh", 6, props);
	}

	os->SetupTimer();
	
	double delta;
	bool isWireframe=false;

	while (os->MessageLoop()) {
		delta = os->GetDeltaTime();
		float deltaSec = (float)delta/1000.0f;

		if (glsys.Update(delta)) {
			os->Present();
		}

		// Translation keys
		if (os->KeyDown('W', true)) { // Move forward
			if (os->KeyDown('B', true)) {
				glsys.Move(0.0f, 0.0f, 100.0f*deltaSec);
			} else {
				glsys.Move(0.0f, 0.0f, 10.0f*deltaSec);
			}
		} else if (os->KeyDown('S', true)) { // Move backward
			glsys.Move(0.0f, 0.0f, -10.0f*deltaSec);
		}
		
		if (os->KeyDown('A', true)) { 
			glsys.Rotate(0.0f, -90.0f*deltaSec, 0.0f); // Yaw left.
		} else if (os->KeyDown('D', true)) {
			glsys.Rotate(0.0f, 90.0f*deltaSec, 0.0f); // Yaw right.
		}
		
		if (os->KeyDown('F', true)) { 
			glsys.Move(-10.0f*deltaSec, 0.0f, 0.0f); // Strafe Left
		} else if (os->KeyDown('G', true)) {
			glsys.Move(10.0f*deltaSec, 0.0f, 0.0f); // Strafe Right
		}
		
		if (os->KeyDown('E', true)) { // Move up
			glsys.Move(0.0f, 10.0f*deltaSec, 0.0f);
		} else if (os->KeyDown('C', true)) { // Move down
			glsys.Move(0.0f, -10.0f*deltaSec, 0.0f);
		}
		
		if (os->KeyDown('Q', true)) { // Pitch Up
			glsys.Rotate(-90.0f * deltaSec, 0.0f, 0.0f);
		} else if (os->KeyDown('Z', true)) { // Pitch Down
			glsys.Rotate(90.0f*deltaSec, 0.0f, 0.0f);
		}

		if (os->KeyDown('R', true)) { // Roll left
			glsys.Rotate(0.0f, 0.0f, -90.0f * deltaSec);
		} else if (os->KeyDown('T', true)) { // Roll right
			glsys.Rotate(0.0f, 0.0f, 90.0f*deltaSec);
		}

		if (os->KeyDown('P', true)) { // Wireframe mode
			if (isWireframe) {
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			} else {
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			}
			isWireframe = !isWireframe;
		}

		if (os->KeyDown('M', true)) { // Fullscreen mode
			os->ToggleFullscreen();
			glsys.SetWindowDim(os->GetWindowWidth(), os->GetWindowHeight()); 
		}
	}
	
	delete os;

	return 0;
}