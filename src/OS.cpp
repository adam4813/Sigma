#include "OS.h"

namespace Sigma {

	bool OS::InitializeWindow(const int width, const int height, const std::string title, const unsigned int glMajor /*= 3*/, const unsigned int glMinor /*= 2*/) {
		// Initialize the library.
		if (!glfwInit()) {
			return false;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);

#ifdef __APPLE__
        // Must use the Core Profile on OS X to get GL 3.2.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        
		// Create a windowed mode window and its OpenGL context.
		this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (!this->window) {
			return false;
		}

		this->width = width;
		this->height = height;

		// Make the window's context current.
		glfwMakeContextCurrent(this->window);
		
#ifndef __APPLE__
		// setting glewExperimental fixes a glfw context problem
		// (tested on Ubuntu 13.04)
		glewExperimental = GL_TRUE;
		
		// Init GLEW.
		GLuint error = glewInit();
		if (error != GLEW_OK) {
			return false;
		}
#endif

		// Associate a pointer for this instance with this window.
		glfwSetWindowUserPointer(this->window, this);

		// Set up some callbacks.
		glfwSetWindowSizeCallback(this->window, &OS::windowResized);
		glfwSetKeyCallback(this->window, &OS::keyboardEvent);
		glfwSetCursorPosCallback(this->window, &OS::mouseMoveEvent);
		glfwSetCharCallback(this->window, &OS::characterEvent);
		glfwSetMouseButtonCallback(this->window, &OS::mouseButtonEvent);

		glfwGetCursorPos(this->window, &this->oldMouseX, &this->oldMouseY);

		return true;
	}

	bool OS::Closing() {
		return glfwWindowShouldClose(this->window) > 0;
	}

	void OS::SwapBuffers() {
		glfwSwapBuffers(this->window);
	}

	void OS::OSMessageLoop() {
		glfwPollEvents();
	}

	int OS::GetWindowWidth() {
		return this->width;
	}

	int OS::GetWindowHeight() {
		return this->height;
	}

	double OS::GetDeltaTime() {
		double time = glfwGetTime();
		double delta = time - this->lastTime;
		this->lastTime = time;
		return delta;
	}

	void OS::windowResized(GLFWwindow* window, int width, int height) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->UpdateWindowSize(width, height);
		}
	}

	void OS::keyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchKeyboardEvent(key, scancode, action, mods);
		}
	}

	void OS::characterEvent(GLFWwindow* window, unsigned int uchar) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchCharacterEvent(uchar);
		}
	}

	void OS::mouseMoveEvent(GLFWwindow* window, double x, double y) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchMouseMoveEvent(x, y);
		}
	}

	void OS::mouseButtonEvent(GLFWwindow* window, int button, int action, int mods) {
		// Get the user pointer and cast it.
		OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

		if (os) {
			os->DispatchMouseButtonEvent(button, action, mods);
		}
	}

	void OS::RegisterKeyboardEventHandler(event::IKeyboardEventHandler* handler) {
		this->KeyboardEventSystem.Register(handler);
	}

	void OS::RegisterMouseEventHandler(event::IMouseEventHandler* handler) {
		this->MouseEventSystem.Register(handler);
	}

	void OS::UpdateWindowSize(const int width, const int height) {
		this->width = width;
		this->height = height;
	}

	void OS::DispatchKeyboardEvent(const int key, const int scancode, const int action, const int mods) {
		if (action == GLFW_PRESS) {
			KeyboardEventSystem.KeyDown(key);
		}
		else if (action == GLFW_REPEAT) {
			KeyboardEventSystem.KeyDown(key);
		}
		else if (action == GLFW_RELEASE) {
			KeyboardEventSystem.KeyUp(key);
		}
	}

	void OS::DispatchCharacterEvent(const unsigned int uchar) {
		this->KeyboardEventSystem.CharDown(uchar);
	}

	void OS::DispatchMouseMoveEvent(const double x, const double y) {
		// If we are in mouse lock we will snap the mouse to the middle of the screen.
		if (this->mouseLock) {
			this->oldMouseX = this->width / 2;
			this->oldMouseY = this->height / 2;
			glfwSetCursorPos(this->window, this->oldMouseX, this->oldMouseY);
		}
		else {
		this->oldMouseX = x;
		this->oldMouseY = y;
	}
		this->MouseEventSystem.MouseMove(static_cast<float>(x / this->width), static_cast<float>(y / this->height), static_cast<float>((x - this->oldMouseX) / this->width), static_cast<float>((y - this->oldMouseY) / this->height));
	}

	void OS::DispatchMouseButtonEvent(const int button, const int action, const int mods) {
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				this->MouseEventSystem.MouseDown(event::LEFT, static_cast<float>(oldMouseX / width), static_cast<float>(oldMouseY / height));
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				this->MouseEventSystem.MouseDown(event::RIGHT, static_cast<float>(oldMouseX / width), static_cast<float>(oldMouseY / height));
			}
			else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
				this->MouseEventSystem.MouseDown(event::MIDDLE, static_cast<float>(oldMouseX / width), static_cast<float>(oldMouseY / height));
			}
		}
		else if (action == GLFW_RELEASE) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				this->MouseEventSystem.MouseUp(event::LEFT, static_cast<float>(oldMouseX / width), static_cast<float>(oldMouseY / height));
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				this->MouseEventSystem.MouseUp(event::RIGHT, static_cast<float>(oldMouseX / width), static_cast<float>(oldMouseY / height));
			}
			else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
				this->MouseEventSystem.MouseUp(event::MIDDLE, static_cast<float>(oldMouseX / width), static_cast<float>(oldMouseY / height));
			}
		}
	}

	void OS::ToggleMouseLock(bool hideCursor) {
		this->mouseLock = !this->mouseLock;
		if (this->mouseLock) {
			glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (hideCursor) {
				glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
		}
		else {
			glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

}
