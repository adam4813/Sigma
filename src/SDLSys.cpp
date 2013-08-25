#include <iostream>

#include "SDLSys.h"

#define GLEW_STATIC

#include "GL/glew.h"
#include "SDL_opengl.h"

void* SDLSys::CreateGraphicsWindow() {
	std::cout << "Creating Window using SDL...";

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	this->_Window = SDL_CreateWindow("Sigma Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	this->_Context = SDL_GL_CreateContext(this->_Window);

	GLenum glew_err = glewInit();
	if (glew_err != GLEW_OK) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(glew_err));
		return 0;
	}
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return &this->_Context;
}

bool SDLSys::MessageLoop() {
	SDL_Event event;
	SDL_Keycode key;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			// hack to do case insensitive lookup
			key = event.key.keysym.sym;
			if (key > 96 && key < 123) {
				key -= 32;
			}
			this->_KeyStates[key] = true;
			break;
		case SDL_KEYUP:
			// hack to do case insensitive lookup
			key = event.key.keysym.sym;
			if (key > 96 && key < 123) {
				key -= 32;
			}
			this->_KeyStates[key] = false;
			break;
		}
	}

	return true;
}

bool SDLSys::SetupTimer() {
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li)) {
		return false;
	}

	this->_Frequency = static_cast<double>(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	this->_LastTime = li.QuadPart;
	return true;
}

double SDLSys::GetDeltaTime() {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double delta = static_cast<double>(li.QuadPart - this->_LastTime);
	this->_LastTime = li.QuadPart;
	return delta/this->_Frequency;
}

bool SDLSys::KeyDown(int key, bool focused) {
	// Check if the focused bool is true, and that
	// the window we are using has keyboard focus
	if(focused && this->_Window == SDL_GetKeyboardFocus()) {
		return this->_KeyStates[key];
	}
	else {
		return false;
	}
}

void SDLSys::Present() {
	SDL_GL_SwapWindow(this->_Window);
}