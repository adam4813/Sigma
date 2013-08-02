#include <iostream>

#include "Entity.h"
#include "Systems\LogSystem.h"
#include "Systems\OpenGLSystem.h"
#include "Components\LogQueue.h"
#include "SceneManager.h"
#include "win32.h"

int main(int argCount, char **argValues) {
	Entity e(0, "test");
	e.NoKNFExcept(true);
	e.AddProperty("bool", true);
	e.RemoveProperty("bool");
	bool b = e.GetBoolProperty("bool");

	// Log system test begin. //
	LogSystem logsys;
	LogQueue* log = static_cast<LogQueue*>(logsys.Factory("LogQueue", 1));
	log->SetTag("Entity: 0");
	log->AddMessage("Testing");
	log->AddMessage("Testing 2");

	logsys.Update(0);
	logsys.Update(0);
	log->AddMessage("Testing 3");
	logsys.Update(0);
	// Log system test end. //

	// OpenGL system test begin. //
	OpenGLSystem glsys;
	win32 win;
	const int* version = glsys.Start(win.CreateWin32Window());
	if (version[0] == -1) {
		std::cout<< "Error starting OpenGL!"<<std::endl;
	} else {
		std::cout<<"OpenGL version: " << version[0] << "." << version[1] << std::endl;
	}
	SceneManager* scene = glsys.GetScene();
	TRANSFORMATION temp = {(0.0f,0.0f,0.0f),(0.0f,0.0f,0.0f),(0.0f,0.0f,0.0f)};
	scene->SetEntity(0, temp);

	MSG msg;
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // If we have a message to process, process it
			if (msg.message == WM_QUIT) {
				break;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}  else { // If we don't have a message to process
			//glsys.Update(0); // Render our scene (which also handles swapping of buffers)
		}
	}

	return 0;
}
