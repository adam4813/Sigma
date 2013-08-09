#pragma once

#include "../ISystem.h"
#include "../SceneManager.h"

#define GLEW_STATIC

#include "GL\glew.h"
#include "GL\wglew.h"

class OpenGLSystem : public ISystem {
public:
	OpenGLSystem();

	// Iterates over all components and deletes them.
	~OpenGLSystem();


	/**
	 * \brief Starts the OpenGL rendering system.
	 *
	 * Starts the OpenGL rendering system and creates a rendering context. It will also attempt to create a newer rendering context (>3) if available.
	 * \param[in] HWND hwnd The window handle to associate the rendering context with.
	 * \returns   const int* Returns -1 in the 0 index on failure, else the major and minor version in index 0 and 1 respectively.
	 */
	const int* Start(HWND hwnd);

	/**
	 * \brief A factory to create new components of a given type.
	 *
	 * A factory method to create various components and add them to the system. These components will be used during the system update method
	 * \param[in] const std::string type The type of componenet to create
	 * \param[in] const int entityID The ID of the entity this component belongs to.
	 * \returns   IComponent* The newly create component
	 */
	IComponent* Factory(const std::string type, const unsigned int entityID) ;

	/**
	 * \brief Causes an update in the system based on the change in time.
	 *
	 * Updates the state of the system based off how much time has elapsed since the last update.
	 * \param[in] const float delta The change in time since the last update
	 * \returns   void
	 */
	void Update(const float delta);

	/**
	 * \brief Retrieve the component that belongs to the given entity ID
	 * 
	 * \param[in] int entityID
	 * \returns   IComponent* The component that belongs to the entity ID or nullptr if no component exists for the given ID.
	 */
	IComponent* GetComponent(int entityID);


	SceneManager* GetScene();
private:
	SceneManager scene;

	HGLRC hrc; // OpenGL Rendering Context.
	HDC hdc; // Handle to the device context.
	HWND hwnd; // Window identifier.

	int windowWidth; // Store the width of our window  
	int windowHeight; // Store the height of our window 

	GLuint m_vaoID[2]; // two vertex array objects, one for each drawn object
	GLuint m_vboID[3]; // three VBOs
	int OpenGLVersion[2];
};
