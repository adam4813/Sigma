#include <iostream>

#include "SDLSys.h"

#include "GL/glew.h"
#include "SDL/SDL_opengl.h"

Sigma::event::KeyboardInputSystem IOpSys::KeybaordEventSystem;
double IOpSys::curDelta;

void* SDLSys::CreateGraphicsWindow() {
	std::cout << "Creating Window using SDL..." << std::endl;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

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

unsigned int SDLSys::GetWindowWidth() {
	unsigned int width;
	SDL_GetWindowSize(this->_Window, &width, 0);
	return width;
}

unsigned int SDLSys::GetWindowHeight() {
	unsigned int height;
	SDL_GetWindowSize(this->_Window, 0, &height);
	return height;
}

void SDLSys::ToggleFullscreen() {
	if(this->_IsFullscreen) {
		SDL_DisplayMode mode;
		SDL_SetWindowFullscreen(this->_Window, SDL_FALSE);
		SDL_GetWindowDisplayMode(this->_Window, &mode);
		mode.w = 800;
		mode.h = 600;
		SDL_SetWindowDisplayMode(this->_Window, &mode);
	}
	else {
		SDL_DisplayMode mode;
		SDL_GetWindowDisplayMode(this->_Window, &mode);
		mode.w = 1280;
		mode.h = 1024;
		SDL_SetWindowDisplayMode(this->_Window, &mode);
		SDL_SetWindowFullscreen(this->_Window, SDL_WINDOW_FULLSCREEN);
	}
	this->_IsFullscreen = !this->_IsFullscreen;
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
	this->_LastTime = SDL_GetTicks();
	return true;
}

double SDLSys::GetDeltaTime() {
	double delta = static_cast<double>(SDL_GetTicks() - this->_LastTime);
	return delta/1000.0f;
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

bool SDLSys::KeyUp(int key, bool focused /*= false */) {
	return true;
}


void SDLSys::Present() {
	SDL_GL_SwapWindow(this->_Window);
}