#ifndef SDL_SYS_H
#define SDL_SYS_H

#include "IOpSys.h"
#include "SDL.h"
#include "SDL_opengl.h"

class SDLSys : public IOpSys {
public:
	SDLSys() { SDL_Init(SDL_INIT_EVERYTHING); }
	virtual ~SDLSys() { SDL_GL_DeleteContext(m_Context); SDL_DestroyWindow(m_Window); SDL_Quit(); }

	virtual void* CreateGraphicsWindow();
	virtual bool MessageLoop();
	virtual bool SetupTimer();
	virtual double GetDeltaTime();
	virtual bool KeyDown(int key, bool focused = false);
	virtual void Present();

private:
	SDL_Window *m_Window;
	SDL_Renderer *m_Renderer;
	SDL_GLContext m_Context;

	double frequency;
	long long lastTime;
};

#endif