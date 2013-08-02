#include "OpenGLSystem.h"
#include "..\Components\GLSprite.h"

OpenGLSystem::OpenGLSystem() : windowWidth(800), windowHeight() { }

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

struct VERTEX {
	float x,y;
};

struct COLOR {
	float r,g,b,a;
};

IComponent* OpenGLSystem::Factory(const std::string type, const unsigned int entityID) {
	if (type == "GLSprite") {
		    /** create some buffers to fill our array buffers with
                (this data is empty, just a placeholder) **/

		VERTEX vert[6];      // vert data
		COLOR color[6];      // color data
		// point 1
		//vert[0].x=xd;
		//vert[0].y=yd;
		//color[0].r=r;
		//color[0].g=g;
		//color[0].b=b;
		//color[0].a=a;
		//// point 2
		//vert[1].x=xs;
		//vert[1].y=yd;
		//color[1].r=r;
		//color[1].g=g;
		//color[1].b=b;
		//color[1].a=a;
		//// point 3
		//vert[2].x=xs;
		//vert[2].y=ys;
		//color[2].r=r;
		//color[2].g=g;
		//color[2].b=b;
		//color[2].a=a;
		//// point 4
		//vert[3].x=xd;
		//vert[3].y=yd;
		//color[3].r=r;
		//color[3].g=g;
		//color[3].b=b;
		//color[3].a=a;
		//// point 5
		//vert[4].x=xs;
		//vert[4].y=ys;
		//color[4].r=r;
		//color[4].g=g;
		//color[4].b=b;
		//color[4].a=a;
		//// point 6
		//vert[5].x=xd;
		//vert[5].y=ys;
		//color[5].r=r;
		//color[5].g=g;
		//color[5].b=b;
		//color[5].a=a;

		/** create a vertex buffer for the sprites **/
		GLuint vertbuf;
		// create a buffer for my vertices
		glGenBuffersARB(1, &vertbuf);
		// put an ID number for it in pVERTEX_BUFFER
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertbuf);
		// fill it with 48 bytes of data
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 48, vert, GL_STREAM_DRAW_ARB);

		/** create a color buffer for the sprites **/
		GLuint colorbuf;
		// create a buffer for my colors
		glGenBuffersARB(1, &colorbuf);
		// put an ID number for it in pcolor_BUFFER
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colorbuf);
		// fill it with 96 bytes of data
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 96, color, GL_STREAM_DRAW_ARB);
	}
	return nullptr;
}
struct attribs {
	unsigned int pVERTEX;
	unsigned int pCOLOUR;
};

void OpenGLSystem::Update(const float delta) {
	glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers


	for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
		for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
			try {
				GLSprite* sprite = dynamic_cast<GLSprite*>(*vecitr);
				attribs attrib;
				attrib.pVERTEX = sprite->VERTEX_BUFFER();
				attrib.pCOLOUR = sprite->COLOUR_BUFFER();

				// set the shader up
				//shd->bSetShader(&attrib);

				glDrawArrays(GL_TRIANGLES,0,6);
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

	if (!this->hrc) {
		return OpenGLVersion;
	}

	return OpenGLVersion;
}

