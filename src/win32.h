#pragma  once
#include <windows.h>
#include "IOpSys.h"

class win32 : public IOpSys {
public:
	win32() { }
	~win32() { }

	void* CreateGraphicsWindow();

	bool MessageLoop();

	bool SetupTimer();

	double GetDeltaTime();

	bool KeyDown(int key);

private:

	HINSTANCE hInstance; // The HINSTANCE of this application  
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Standard window callback

	double frequency; // The frequency of our timer in ticks/second.
	LONGLONG lastTime; // The time of the last call to GetDeltaTime.
};

