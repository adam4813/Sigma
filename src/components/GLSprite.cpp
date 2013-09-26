#include "GLSprite.h"
#include "GL/glew.h"

#ifdef OS_Win32
	#include "SOIL/SOIL.h"
#elif OS_SDL
	#include "SDL/SDL.h"
	#include "SDL/SDL_image.h"
#endif

GLSprite::GLSprite( const int entityID /*= 0*/ ) : Sigma::IGLComponent(entityID)  {
	this->drawMode = GL_TRIANGLES;
	this->ElemBufIndex = 2;
	this->ColorBufIndex = 1;
	this->VertBufIndex = 0;
	this->UVBufIndex = 3;
}


void GLSprite::InitializeBuffers() {
	texture_ = LoadTexture();
	static const GLfloat vert[] = {
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f,  -1.0f, 0.0f,
		1.0f,  -1.0f, 0.0f,
	};

	static const GLfloat col[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	};

	static const GLfloat uv[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	// We must create a vao and then store it in our GLSprite.
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->buffers[this->VertBufIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	GLint posLocation = glGetAttribLocation(GLSprite::shader.GetProgram(), "in_Position");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posLocation);

	static const GLushort elem[] = { 0, 1, 2, 1, 3, 2 }; 
	glGenBuffers(1, &this->buffers[this->ElemBufIndex]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elem), elem, GL_STATIC_DRAW);

	glGenBuffers(1, &this->buffers[this->ColorBufIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->ColorBufIndex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
	GLint colLocation = glGetAttribLocation(GLSprite::shader.GetProgram(), "in_Color");
	glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colLocation);

	glGenBuffers(1, &this->buffers[this->UVBufIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->UVBufIndex]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);
	GLint uvlocation = glGetAttribLocation(GLSprite::shader.GetProgram(),"in_UV");
	glVertexAttribPointer(uvlocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(uvlocation);
	
	glBindVertexArray(0);

	this->texture_ = LoadTexture();
}

void GLSprite::LoadShader() {
	GLSprite::shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/vert.shade");
	GLSprite::shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/frag.shade");
	GLSprite::shader.CreateAndLinkProgram();
}

void GLSprite::Render(glm::mediump_float *view, glm::mediump_float *proj) {
	GLSprite::shader.Use();

	glUniform1i(glGetUniformLocation(GLSprite::shader.GetProgram(), "tex"), GL_TEXTURE0);
	glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_Model"), 1, GL_FALSE, &this->Transform()->ModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_View"), 1, GL_FALSE, view);
	glUniformMatrix4fv(glGetUniformLocation(GLSprite::shader.GetProgram(), "in_Proj"), 1, GL_FALSE, proj);
	
	glBindVertexArray(this->Vao());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->GetBuffer(this->ElemBufIndex));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->GetTexture());

	glDrawElements(this->DrawMode(), this->MeshGroup_ElementCount(), GL_UNSIGNED_SHORT, (void*)0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLSprite::shader.UnUse();
}

unsigned int GLSprite::LoadTexture() {
	GLuint textureID;
#ifdef OS_Win32
	textureID = SOIL_load_OGL_texture("test.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
#elif OS_SDL
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	SDL_Surface *img;
	//img = IMG_Load("test.jpg");
	
	if(img==0) {
		assert(0 && "Failed to load test.jpg");
	} else {
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_RGB,GL_UNSIGNED_BYTE,(img->pixels));
		//SDL_FreeSurface(img);
	}

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
#endif

	return textureID;
}

GLSLShader GLSprite::shader;
