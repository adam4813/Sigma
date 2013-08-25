#include "SDLSys.h"

void* SDLSys::CreateGraphicsWindow() {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_Window = SDL_CreateWindow("Sigma Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
	m_Context = SDL_GL_CreateContext(m_Window);

	return &m_Context;
}

bool SDLSys::MessageLoop() {
	SDL_Event *event;

	if (SDL_PollEvent(event)) {
		if(event->type == SDL_QUIT) {
			return false;
		}
	}

	return true;
}

bool SDLSys::SetupTimer() {
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li)) {
		return false;
	}

	this->frequency = static_cast<double>(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	this->lastTime = li.QuadPart;
	return true;
}

double SDLSys::GetDeltaTime() {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double delta = static_cast<double>(li.QuadPart - this->lastTime);
	this->lastTime = li.QuadPart;
	return delta/this->frequency;
}

bool SDLSys::KeyDown(int key, bool focused) {
	return false;
}

void SDLSys::Present() {
	SDL_GL_SwapWindow(m_Window);
	//SDL_RenderPresent(this->m_Renderer);
}