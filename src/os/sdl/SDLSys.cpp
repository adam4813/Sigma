#include "os/sdl/SDLSys.h"

#include "GL/glew.h"
#include "SDL_opengl.h"


Sigma::event::KeyboardInputSystem IOpSys::KeyboardEventSystem;
Sigma::event::MouseInputSystem IOpSys::MouseEventSystem;

//double IOpSys::curDelta;

void* SDLSys::CreateGraphicsWindow(const unsigned int width, const unsigned int height) {
    std::cout << "Creating Window using SDL..." << std::endl;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetSwapInterval(0);

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
    /*
    if(SDL_SetRelativeMouseMode(SDL_FALSE) < 0){
        std::cerr << "ERROR: SDL could not grab mouse\n" << SDL_GetError() << std::endl;
    } */
    // relative mode was breaking things, just hide mouse.
    if(SDL_ShowCursor(SDL_DISABLE) < 0) {
        std::cerr << "ERROR: SDL could not hide mouse\n" << SDL_GetError() << std::endl;
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
    float dampener = 1.0f;
    bool mouse_moved = false; // if no mouse motion, we will explicitly clear it

    width = (float)GetWindowWidth();
    height = (float)GetWindowHeight();

    this->_KeyReleased.clear();

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            SDL_ShowCursor(SDL_ENABLE); // show mouse, since we are exiting
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
                    event.motion.x, event.motion.y);// */
            if(!(event.motion.xrel || event.motion.yrel)) {
                return true; // Exit if no motion to avoid spamming mouse events
            }
            if(event.motion.xrel == -lastmouserx) {
                event.motion.xrel = 0;
            }
            if(event.motion.yrel == -lastmousery) {
                event.motion.yrel = 0;
            }
            lastmouserx = event.motion.xrel;
            lastmousery = event.motion.yrel;
            // Motion is in pixels, so normalize to -1.0 to 1.0 range
            // Relative mouse motion is restricted, so scale the movement
            // and dampen small movements
            if(abs(event.motion.xrel) < dampener) {
                dx = 0.0f;
            } else {
                dx = mouse_sensitivity * (event.motion.xrel / width);
            }

            if(abs(event.motion.yrel) < dampener) {
                dy = 0.0f;
            } else {
                dy = mouse_sensitivity * (event.motion.yrel / height);
            }

            // Dispatch the event
            MouseEventSystem.MouseMove(
				static_cast<float>(event.motion.x) / static_cast<float>(this->GetWindowWidth()),
				static_cast<float>(event.motion.y) / static_cast<float>(this->GetWindowHeight()),
				dx, dy);

            // if mouse is hidden/grabbed and moved, recenter it so it doesn't appear offscreen
            SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
            SDL_WarpMouseInWindow(this->_Window, (width / 2.0f), (height / 2.0f));
            SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
            break;
        case SDL_MOUSEBUTTONDOWN:
            printf("Mouse button %d pressed at (%d,%d)\n",
                    event.button.button, event.button.x, event.button.y);
            printf("Mouse button pressed \n");

            break;
        }
    }

	// explicitly clear mouse motion if no signal was received
	//if(!mouse_moved){
    //    MouseEventSystem.MouseMove(0.f, 0.f);
	//}
	// if mouse is hidden/grabbed, recenter it so it doesn't appear offscreen
    //if(SDL_GetRelativeMouseMode() == SDL_TRUE){
    //    SDL_WarpMouseInWindow(this->_Window, this->GetWindowWidth()/2, this->GetWindowHeight()/2);
    //}

    return true;
}

bool SDLSys::SetupTimer() {
    this->_LastTime = SDL_GetTicks();
    return true;
}

void SDLSys::ToggleFullscreen() {
	SDL_DisplayMode current;

	if(!this->Fullscreen) {
		SDL_GetDisplayMode(0, 0, &current);
		SDL_SetWindowSize(this->_Window, current.w, current.h);
		SDL_SetWindowFullscreen(this->_Window,SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(this->_Window,0);
		SDL_SetWindowSize(this->_Window, 1024, 768);
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
    double currentTime = static_cast<double>(SDL_GetTicks());
    double delta = currentTime - this->_LastTime;
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

void SDLSys::LoadFont(std::string file, unsigned int ptSize) {
	this->font = TTF_OpenFont(file.c_str(), ptSize);

	if(!this->font) {
		std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
	}

	TTF_SetFontKerning(this->font, 0);
}

void SDLSys::RenderText(std::string text, float x, float y, unsigned int texture_id) {
	SDL_Color color, bgColor;
	SDL_Surface *textSurface;

	color.a = color.g = 255;
	color.r = color.b = 0;

	bgColor.a = bgColor.r = bgColor.g = bgColor.b = 0;

	if(font) {
		// Render text
		textSurface = TTF_RenderText_Blended(this->font, text.c_str(), color);

		if(!textSurface) {
			std::cerr << "TTF Error: " << TTF_GetError();
			return;
		} else {
			// Note: width should not exceed the texture's size,
			// but there's no way to compare just yet without
			// a full fledged Texture class
			int w, h;
			TTF_SizeText(this->font, text.c_str(), &w, &h);

			// Bind the texture for writing
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glActiveTexture(GL_TEXTURE0);

			// Convert text surface to RGBA8 format
			SDL_Surface *convertSurface = SDL_CreateRGBSurface(0, textSurface->w, textSurface->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

			if(convertSurface) {
				//SDL_SetSurfaceAlphaMod(textSurface, 0xFF);
				//SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_NONE);
				SDL_BlitSurface(textSurface, 0, convertSurface, 0);

				// Copy it over
				glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, convertSurface->w, convertSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, convertSurface->pixels);
			} else {
				std::cerr << SDL_GetError() << std::endl;
			}

		
			// Free resources/cleanup
			glBindTexture(GL_TEXTURE_2D, 0);
			SDL_FreeSurface(convertSurface);
			SDL_FreeSurface(textSurface);
		}
	}
}

void SDLSys::Present(int fbo_id) {
	// For now, use glBlitFramebuffer, but rendering using a screen space quad
	// will be faster
	if(fbo_id > 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id);

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, (GLsizei)this->GetWindowWidth(), (GLsizei)this->GetWindowHeight(),
						  0, (GLsizei)(this->GetWindowHeight()/2.0f), (GLsizei)(this->GetWindowWidth()/2.0f), (GLsizei)(this->GetWindowHeight()),
						  GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glBlitFramebuffer(0, 0, this->GetWindowWidth(), this->GetWindowHeight(),
						  (GLsizei)(this->GetWindowWidth()/2.0f), (GLsizei)(this->GetWindowHeight()/2.0f), (GLsizei)(this->GetWindowWidth()), (GLsizei)(this->GetWindowHeight()),
						  GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glBlitFramebuffer(0, 0, this->GetWindowWidth(), this->GetWindowHeight(),
						  0, 0, (GLsizei)(this->GetWindowWidth()/2.0f), (GLsizei)(this->GetWindowHeight()/2.0f),
						  GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
    
	SDL_GL_SwapWindow(this->_Window);
}
