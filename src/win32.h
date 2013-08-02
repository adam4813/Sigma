#include <windows.h>

class win32 {
public:
	win32() { }
	~win32() { }

	HWND CreateWin32Window();

private:

	HINSTANCE hInstance; // The HINSTANCE of this application  
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Standard window callback
};

