#include "SDLSys.h"

#include "GL/glew.h"
#include "SDL/SDL_opengl.h"

Sigma::event::KeyboardInputSystem IOpSys::KeyboardEventSystem;
Sigma::event::MouseInputSystem IOpSys::MouseEventSystem;

//double IOpSys::curDelta;

void* SDLSys::CreateGraphicsWindow(const unsigned int width, const unsigned int height) {
	std::cout << "Creating Window using SDL..." << std::endl;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	this->_Window = SDL_CreateWindow("Sigma Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	this->_Context = SDL_GL_CreateContext(this->_Window);

	GLenum glew_err = glewInit();
	if (glew_err != GLEW_OK) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(glew_err));
		return 0;
	}

	this->Fullscreen = false;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Set the relative mouse state (hide mouse)
	if(SDL_SetRelativeMouseMode(SDL_TRUE) < 0){
        std::cerr << "ERROR: SDL could not grab mouse\n" << SDL_GetError() << std::endl;
	}

	return &this->_Context;
}

bool SDLSys::MessageLoop() {

	SDL_Event event;

	// Keyboard varables
	SDL_Keycode key;

	// Mouse variables
	float width, height, dx=0.0f, dy=0.0f;
	float mouse_sensitivity = 100.0f;
	float dampener = 2.0f;
	bool mouse_moved = false; // if no mouse motion, we will explicitly clear it

	width = GetWindowWidth();
	height = GetWindowHeight();

	this->_KeyReleased.clear();

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
            KeyboardEventSystem.KeyDown(key);
            break;
		case SDL_KEYUP:
			// hack to do case insensitive lookup
			key = event.key.keysym.sym;
			if (key > 96 && key < 123) {
				key -= 32;
			}
			this->_KeyStates[key] = false;
			this->_KeyReleased[key] = true;
			KeyboardEventSystem.KeyUp(key);
			break;
		case SDL_MOUSEMOTION:
		    mouse_moved = true;
            /*printf("Mouse moved by %d,%d to (%d,%d)\n",
                    event.motion.xrel, event.motion.yrel,
                    event.motion.x, event.motion.y);*/

			// Motion is in pixels, so normalize to -1.0 to 1.0 range
			// Relative mouse motion is restricted, so scale the movement
			// and dampen small movements
			if(abs(event.motion.xrel) < dampener) {
				dx = 0.0f;
			} else {
				dx = mouse_sensitivity * (event.motion.xrel) / width;
			}

			if(abs(event.motion.yrel) < dampener) {
				dy = 0.0f;
			} else {
				dy = mouse_sensitivity * (event.motion.yrel) / height;
			}

			MouseEventSystem.MouseMove(dx, dy);
            break;
        case SDL_MOUSEBUTTONDOWN:
            printf("Mouse button %d pressed at (%d,%d)\n",
                    event.button.button, event.button.x, event.button.y);
            printf("Mouse button pressed \n");

            break;
		}
	}

	// explicitly clear mouse motion if no signal was received
	if(!mouse_moved){
        MouseEventSystem.MouseMove(0.f, 0.f);
	}
	// if mouse is hidden/grabbed, recenter it so it doesn't appear offscreen
    if(SDL_GetRelativeMouseMode() == SDL_TRUE){
        SDL_WarpMouseInWindow(this->_Window, this->GetWindowWidth()/2, this->GetWindowHeight()/2);
    }

	return true;
}

bool SDLSys::SetupTimer() {
	this->_LastTime = SDL_GetTicks();
	return true;
}

void SDLSys::ToggleFullscreen() {
	if(!this->Fullscreen) {
		SDL_SetWindowFullscreen(this->_Window,SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(this->_Window,0);
	}
	this->Fullscreen=!this->Fullscreen;
}

unsigned int SDLSys::GetWindowHeight() {
	int width,height;
	SDL_GetWindowSize(this->_Window,&width,&height);
	return height;
}

unsigned int SDLSys::GetWindowWidth() {
	int width,height;
	SDL_GetWindowSize(this->_Window,&width,&height);
	return width;
}

double SDLSys::GetDeltaTime() {
	int currentTime = SDL_GetTicks();
	double delta = static_cast<double>(currentTime - this->_LastTime);
	this->_LastTime = currentTime;
	return delta;
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
	// Check if the focused bool is true, and that
	// the window we are using has keyboard focus
	if(focused && this->_Window == SDL_GetKeyboardFocus()) {
		return !this->_KeyStates[key];
	}
	else {
		return false;
	}
}


bool SDLSys::KeyReleased(int key, bool focused /*= false */) {
	if(focused && this->_Window == SDL_GetKeyboardFocus()) {
		return this->_KeyReleased[key];
	}
	else {
		return false;
	}
}


void SDLSys::Present() {
	SDL_GL_SwapWindow(this->_Window);
}
