#include "win32.h"

#include "GL/glew.h"
#include "GL/wglew.h"

Sigma::event::KeyboardInputSystem IOpSys::KeybaordEventSystem; // Handles keyboard events
double IOpSys::curDelta;

win32::~win32() {
	wglMakeCurrent(this->hdc, 0); // Remove the rendering context from our device context  
	wglDeleteContext(this->hrc); // Delete our rendering context  

	ReleaseDC(this->hwnd, this->hdc); // Release the device context from our window  
}

void win32::ToggleFullscreen() {
	if (this->fullscreen) {
		SetWindowLongPtr(this->hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		AdjustWindowRect(&this->winSize, WS_OVERLAPPEDWINDOW, FALSE);
	} else {
		SetWindowLongPtr(this->hwnd, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
	}
	MoveWindow(this->hwnd, 0, 0, this->winSize.right, this->winSize.bottom, TRUE);
	this->fullscreen = !this->fullscreen;
}

void* win32::CreateGraphicsWindow(const unsigned int width, const unsigned int height) {
	WNDCLASS windowClass;
	//HWND hwnd;
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

	this->winSize.right = width;
	this->winSize.bottom = height;

	if (!RegisterClass(&windowClass)) {
		return false;
	}
	this->hwnd = CreateWindowEx(dwExStyle, windowClass.lpszClassName, windowClass.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, this->winSize.right, this->winSize.bottom, NULL, NULL, hInstance, NULL);

	ShowWindow(this->hwnd, SW_SHOW);
	UpdateWindow(this->hwnd);

	StartOpengGL();

	return this->hdc;
}

LRESULT CALLBACK win32::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_KEYUP:
			KeybaordEventSystem.KeyUp(wParam);
			keyUp[wParam] = 1;
			return 0;
		break;
	case WM_KEYDOWN:
		KeybaordEventSystem.KeyDown(wParam);
		return 0;
		break;
	case WM_DESTROY: 
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);  
}  

bool win32::MessageLoop() {
	MSG msg;
	ZeroMemory(keyUp, sizeof(keyUp));
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
	return this->curDelta = delta/this->frequency;
}

bool win32::KeyDown(int key, bool focused) {
	if (focused) {
		if (this->hwnd != GetFocus()) {
			return false;
		}
	}
	short k = GetKeyState(key);
	return (k & 0x80) > 0 ? true : false;
}

const int* win32::StartOpengGL() {
	this->fullscreen = false;
	this->OpenGLVersion[0] = -1;
	this->OpenGLVersion[1] = -1;

	this->hdc = GetDC(this->hwnd); // Get the device context for our window  

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(this->hdc, &pfd);

	if (nPixelFormat == 0) {
		return this->OpenGLVersion;
	}

	BOOL bResult = SetPixelFormat (this->hdc, nPixelFormat, &pfd);

	if (!bResult) {
		return this->OpenGLVersion;
	}

	HGLRC tempContext = wglCreateContext(this->hdc);
	wglMakeCurrent(this->hdc, tempContext);

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		return OpenGLVersion;
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1) {
		this->hrc = wglCreateContextAttribsARB(this->hdc, 0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(this->hdc, this->hrc);
	} else {
		//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		this->hrc = tempContext;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	//Or better yet, use the GL3 way to get the version number
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	// Set options for depth tests.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return OpenGLVersion;
}

void win32::Present() {
	SwapBuffers(this->hdc); // Swap buffers so we can see our rendering.
}

bool win32::KeyUp(int key, bool focused /*= false*/) {
	if (focused) {
		if (this->hwnd != GetFocus()) {
			return false;
		}
	}
	return keyUp[key];
}

int win32::GetWindowWidth() {
	throw std::exception("The method or operation is not implemented.");
}

int win32::GetWindowHeight() {
	throw std::exception("The method or operation is not implemented.");
}

int win32::keyUp[256];
