#pragma  once
#include <windows.h>
#include "IOpSys.h"

class win32 : public IOpSys {
public:
	win32() { Sigma::event::KEY_ESCAPE = VK_ESCAPE; }
	~win32();

	void* CreateGraphicsWindow(const unsigned int width = 800, const unsigned int height = 600);

	void ToggleFullscreen();

	const int* StartOpengGL();

	bool MessageLoop();

	bool SetupTimer();

	double GetDeltaTime();

	bool KeyDown(int key, bool focused = false);

	bool KeyUp(int key, bool focused = false);

	bool KeyReleased( int key, bool focused = false );

	virtual void Present(int fbo_id=-1);

	virtual void LoadFont(std::string filename, unsigned int ptSize) {};
	virtual void RenderText(std::string text, float x, float y, unsigned int texture_id) {};

	virtual unsigned int GetWindowWidth();
	virtual unsigned int GetWindowHeight();

private:
	HWND hwnd; // The current window handle.
	HGLRC hrc; // OpenGL Rendering Context.
	HDC hdc; // Handle to the device context.
	HINSTANCE hInstance; // The HINSTANCE of this application  
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Standard window callback

	double frequency; // The frequency of our timer in ticks/second.
	LONGLONG lastTime; // The time of the last call to GetDeltaTime.

	int OpenGLVersion[2];
	static int keyUp[256]; // Stores which keys were released since the last MessageLoop call.
	static int mousePos[2]; // Stores the last mouse position.

	bool fullscreen;
	RECT windowedSize;
	static bool mouselook;
};
