#include "OpenGLSystem.h"
#include "..\Components\GLSprite.h"
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
		this->sprShade.LoadFromFile(GL_VERTEX_SHADER, "vert.shade");
		this->sprShade.LoadFromFile(GL_FRAGMENT_SHADER, "frag.shade");
		this->sprShade.CreateAndLinkProgram();
		this->sprShade.Use(); // This will need to be moved into the rendering loop once components are rendered based on type..

		GLSprite* spr = new GLSprite(entityID);
		
		static const GLfloat vert[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f,  1.0f, 0.0f,
		};

		static const GLfloat col[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		};

		// We must create a vao and then store it in our GLSprite.
		GLuint vaoID;
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		GLuint vertbuf;
		glGenBuffers(1, &vertbuf);
		glBindBuffer(GL_ARRAY_BUFFER, vertbuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
		GLint posLocation = glGetAttribLocation(this->sprShade.GetProgram(), "in_Position");
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posLocation);

		GLuint colorbuf;
		glGenBuffers(1, &colorbuf);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
		GLint colLocation = glGetAttribLocation(this->sprShade.GetProgram(), "in_Color");
		glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(colLocation);
		glBindVertexArray(0);

		spr->Vao(vaoID);
		spr->VertBuf(vertbuf);
		spr->ColBuf(colorbuf);
		this->components[entityID].push_back(spr);
	}
	return nullptr;
}

void OpenGLSystem::Update(const float delta) {
	glClearColor (0.0f,0.0f,1.0f,0.0f);
	glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

	for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
		for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
			try {
				GLSprite* sprite = dynamic_cast<GLSprite*>(*vecitr);
				glBindVertexArray(sprite->Vao());
				glDrawArrays(GL_TRIANGLES,0,3);
				glBindVertexArray(0);
			} catch (std::bad_cast b) {

			}
		}
	}

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

	return OpenGLVersion;
}
