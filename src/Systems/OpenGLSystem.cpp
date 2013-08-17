#include "OpenGLSystem.h"
#include "GLSLShader.h"
#include "GLSixDOFView.h"
#include "../Components/GLSprite.h"
#include "../Components/GLIcoSphere.h"

OpenGLSystem::OpenGLSystem() : windowWidth(800), windowHeight(600), deltaAccumulator(0.0) {
	this->view = new GLSixDOFView(); 
}

GLIcoSphere* sphere;

OpenGLSystem::~OpenGLSystem() {
	delete this->view;
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
		if (entityID == 2) {
			spr->OffsetX(2);
			spr->OffsetY(2);
		}
		this->components[entityID].push_back(spr);
		return spr;
	} else if (type == "GLIcoSphere") {
		sphere = GLIcoSphere::Factory(entityID);
		sphere->Transform().Scale(100.0f,100.0f,100.0f);
		sphere->Transform().Translate(0.0f,0.0f,1000.0f);
		return sphere;
	}
	return nullptr;
}

void OpenGLSystem::Update(const double delta) {
	this->deltaAccumulator += delta;
	this->view->UpdateViewMatrix();
	// Check if the deltaAccumulator is greater than 1/60 of a second.
	if (deltaAccumulator > 16.7) {
		// Set up the scene to a "clean" state.
		glClearColor(0.0f,0.0f,1.0f,0.0f);
		glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

		GLSprite::shader.Use();
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		// Set the ViewProjection matrix to be used in the shader.
		glm::mat4 ModelMatrix(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Model"), 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_View"), 1, GL_FALSE, &this->view->ViewMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_Proj"), 1, GL_FALSE, &this->ProjectionMatrix[0][0]);
		// Loop through and draw each component.
		for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
			for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
				try {
					GLSprite* sprite = static_cast<GLSprite*>(*vecitr);
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
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		GLIcoSphere::shader.Use();
		sphere->Transform().Rotate(0.0f,0.1f,0.0f);
		glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Model"), 1, GL_FALSE, &sphere->Transform().ModelMatrix()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_View"), 1, GL_FALSE, &this->view->ViewMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(GLIcoSphere::shader.GetProgram(), "in_Proj"), 1, GL_FALSE, &this->ProjectionMatrix[0][0]);
		glBindVertexArray(sphere->Vao());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere->ElemBuf());
		glDrawElements(GL_TRIANGLES, sphere->NumberElements(), GL_UNSIGNED_SHORT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		glBindVertexArray(0);
		GLIcoSphere::shader.UnUse();
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		SwapBuffers(hdc); // Swap buffers so we can see our rendering.
		this->deltaAccumulator = 0.0;
	}
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
	GLIcoSphere::LoadShader();

	// Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
	this->ProjectionMatrix = glm::perspective(
		45.0f,
		4.0f / 3.0f,
		0.1f,
		10000.0f
		);

	// Set options for depth tests.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	this->view->Move(4.0f,3.0f,-10.f);

	return OpenGLVersion;
}

void OpenGLSystem::Translate(float x, float y, float z) {
	this->view->Move(x,y,z);
}

void OpenGLSystem::Rotate(float x, float y, float z) {
	this->view->Rotate(x,y,z);
}
