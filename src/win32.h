#pragma  once
#include <windows.h>
#include "IOpSys.h"

class win32 : public IOpSys {
public:
	win32() { }
	~win32();

	void* CreateGraphicsWindow();

	const int* StartOpengGL();

	bool MessageLoop();

	bool SetupTimer();

	double GetDeltaTime();

	bool KeyDown(int key, bool focused = false);

	void Present();

	// Stubbed out for now
	virtual void ToggleFullscreen() {}
	virtual int GetWindowWidth() { return 800; }
	virtual int GetWindowHeight() { return 600; }
private:
	HWND hwnd; // The current window handle.
	HGLRC hrc; // OpenGL Rendering Context.
	HDC hdc; // Handle to the device context.
	HINSTANCE hInstance; // The HINSTANCE of this application  
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Standard window callback

	double frequency; // The frequency of our timer in ticks/second.
	LONGLONG lastTime; // The time of the last call to GetDeltaTime.

	int OpenGLVersion[2];
};

