#include "win32.h"

HWND win32::CreateWin32Window() {
	WNDCLASS windowClass;
	HWND hwnd;
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	hInstance = GetModuleHandle(NULL);

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC) WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "GL Test Window";

	if (!RegisterClass(&windowClass)) {
		return false;
	}
	hwnd = CreateWindowEx(dwExStyle, windowClass.lpszClassName, windowClass.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 800, 600, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return hwnd;
}

LRESULT CALLBACK win32::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {  
	switch (message) {
	case WM_DESTROY: 
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);  
}  

bool win32::MessageLoop() {
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // If we have a message to process, process it
		if (msg.message == WM_QUIT) {
			return false;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	Sleep(1);
	return true;
}

bool win32::SetupTimer() {
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li)) {
		return false;
	}

	this->frequency = static_cast<double>(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	this->lastTime = li.QuadPart;
	return true;
}

double win32::GetDeltaTime() {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double delta = static_cast<double>(li.QuadPart - this->lastTime);
	this->lastTime = li.QuadPart;
	return delta/this->frequency;
}
