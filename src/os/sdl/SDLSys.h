#ifndef SDL_SYS_H
#define SDL_SYS_H

#include <iostream>
#include <cstring>
#include <map>

#include "IOpSys.h"
#include "SDL2/SDL.h"

#ifdef __linux__
// used for putenv below
#include <cstdlib>
#endif // __linux__

class SDLSys : public IOpSys {
public:
	SDLSys() {
#ifdef __linux__
        // this magical environment variable makes fullscreen work on linux
        char buffer[50];
        strcpy(buffer, "SDL_VIDEO_X11_LEGACY_FULLSCREEN=0");
        putenv(buffer);
#endif // __linux__
	    SDL_Init(SDL_INIT_EVERYTHING);
	    this->Fullscreen = false;
	    Sigma::event::KEY_ESCAPE = SDLK_ESCAPE;
    }
	virtual ~SDLSys() { SDL_GL_DeleteContext(this->_Context); SDL_DestroyWindow(this->_Window); SDL_Quit(); }

	virtual void* CreateGraphicsWindow(const unsigned int width = 800, const unsigned int height = 600);
	virtual void ToggleFullscreen();

	virtual bool MessageLoop();

	virtual bool SetupTimer();
	virtual double GetDeltaTime();

	virtual bool KeyDown(int key, bool focused = false);
	virtual bool KeyUp( int key, bool focused = false );

	virtual bool KeyReleased( int key, bool focused = false );

	virtual void Present();
	virtual unsigned int GetWindowWidth();
	virtual unsigned int GetWindowHeight();

private:
	SDL_Window *_Window;
	SDL_GLContext _Context;

	// Keyboard state
	std::map<char, bool> _KeyStates;
	std::map<char, bool> _KeyReleased;

	int lastmouserx;
	int lastmousery;
	double _Frequency;
	long long _LastTime;

	bool Fullscreen;
};

#endif
