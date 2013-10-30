#ifndef SDL_SYS_H
#define SDL_SYS_H

#include <iostream>
#include <cstring>
#include <map>

#include "IOpSys.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#ifdef __linux__
// used for putenv below
#include <cstdlib>
#endif // __linux__

class SDLSys : public IOpSys {
public:
	SDLSys() : font(0), Fullscreen(false) {
#ifdef __linux__
        // this magical environment variable makes fullscreen work on linux
        putenv("SDL_VIDEO_X11_LEGACY_FULLSCREEN=0");
#endif // __linux__
	    if(SDL_Init(SDL_INIT_EVERYTHING)==-1) {
			std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
		}

		if(!TTF_WasInit() && TTF_Init()==-1) {
			std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
		}

	    Sigma::event::KEY_ESCAPE = SDLK_ESCAPE;
    }
	virtual ~SDLSys() { SDL_GL_DeleteContext(this->_Context); SDL_DestroyWindow(this->_Window); TTF_CloseFont(font), TTF_Quit(); SDL_Quit(); }

	virtual void* CreateGraphicsWindow(const unsigned int width = 800, const unsigned int height = 600);
	virtual void ToggleFullscreen();

	virtual bool MessageLoop();

	virtual bool SetupTimer();
	virtual double GetDeltaTime();

	virtual bool KeyDown(int key, bool focused = false);
	virtual bool KeyUp( int key, bool focused = false );

	virtual bool KeyReleased( int key, bool focused = false );

	virtual void RenderText(std::string text, float x, float y, unsigned int texture_id);
	virtual void LoadFont(std::string file, unsigned int ptSize);

	virtual void Present(int fbo_id);
	virtual unsigned int GetWindowWidth();
	virtual unsigned int GetWindowHeight();

private:
	SDL_Window *_Window;
	SDL_GLContext _Context;

	// Fonts
	TTF_Font *font;

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
