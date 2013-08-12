#include <windows.h>

class win32 {
public:
	win32() { }
	~win32() { }

	HWND CreateWin32Window();

	bool MessageLoop();

	bool SetupTimer();

	double GetDeltaTime();

private:

	HINSTANCE hInstance; // The HINSTANCE of this application  
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Standard window callback

	double frequency; // The frequency of our timer in ticks/second.
	LONGLONG lastTime; // The time of the last call to GetDeltaTime.
};

