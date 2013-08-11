#include "OpenGLSystem.h"
#include "../Components/GLSprite.h"
#include "GLSLShader.h"

OpenGLSystem::OpenGLSystem() : windowWidth(800), windowHeight(600) { }

OpenGLSystem::~OpenGLSystem() {
	for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
		for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
			delete *vecitr;
		}
	}

	wglMakeCurrent(this->hdc, 0); // Remove the rendering context from our device context  
	wglDeleteContext(this->hrc); // Delete our rendering context  

	ReleaseDC(this->hwnd, this->hdc); // Release the device context from our window  
}

IComponent* OpenGLSystem::Factory(const std::string type, const unsigned int entityID) {
	if (type == "GLSprite") {
		GLSprite* spr = GLSprite::Factory(entityID);
		this->components[entityID].push_back(spr);
		return spr;
	}
	return nullptr;
}

void OpenGLSystem::Update(const float delta) {
	// Set up the scene to a "clean" state.
	glClearColor (0.0f,0.0f,1.0f,0.0f);
	glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

	GLSprite::shader.Use();

	// Set the ViewProjection matrix to be used in the shader.
	glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_VP"), 1, GL_FALSE, &GetVPMatrix()[0][0]);

	// Loop through and draw each component.
	for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
		for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
			try {
				GLSprite* sprite = dynamic_cast<GLSprite*>(*vecitr);
				sprite->OffsetX(0.05f);
				sprite->OffsetY(0.05f);
				glBindVertexArray(sprite->Vao());
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ElemBuf());
				glUniform2d(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_Offset"), sprite->OffsetX(), sprite->OffsetY());
				glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
				glBindVertexArray(0);
			} catch (std::bad_cast b) {

			}
		}
	}
	GLSprite::shader.UnUse();

	SwapBuffers(hdc); // Swap buffers so we can see our rendering  
}

IComponent* OpenGLSystem::GetComponent(int entityID) {
	if (this->components.find(entityID) != this->components.end()) {
		return this->components[entityID][0];
	}
	return nullptr;
}

SceneManager* OpenGLSystem::GetScene() {
	return &this->scene;
}

const int* OpenGLSystem::Start(HWND hwnd) {
	this->OpenGLVersion[0] = -1;
	this->OpenGLVersion[1] = -1;
	this->hwnd = hwnd; // Set the HWND for our window  

	this->hdc = GetDC(hwnd); // Get the device context for our window  

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
		return OpenGLVersion;
	}

	BOOL bResult = SetPixelFormat (this->hdc, nPixelFormat, &pfd);

	if (!bResult) {
		return OpenGLVersion;
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

	// Now that GL is up and running load the shaders
	GLSprite::LoadShader();

	this->CameraMatrix = glm::lookAt(
		glm::vec3(4,3,3),
		glm::vec3(0,0,0),
		glm::vec3(0,1,0)
		);

	// Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
	this->projectionMatrix = glm::perspective(
		45.0f,
		4.0f / 3.0f,
		0.1f,
		100.0f
		);

	return OpenGLVersion;
}

glm::mat4 OpenGLSystem::GetVPMatrix() {
	return this->projectionMatrix * this->CameraMatrix;;
}
