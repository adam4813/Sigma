#pragma once

#include "Property.h"
#include "../IFactory.h"
#include "../ISystem.h"
#include "../IGLComponent.h"

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

struct IGLView;

class OpenGLSystem
    : public Sigma::IFactory, public ISystem<Sigma::IGLComponent> {
public:
	OpenGLSystem();

	// Iterates over all components and deletes them.
	~OpenGLSystem();


	/**
	 * \brief Starts the OpenGL rendering system.
	 *
	 * Starts the OpenGL rendering system and creates a rendering context. It will also attempt to create a newer rendering context (>3) if available.
	 * \param[in] HWND hwnd The window handle to associate the rendering context with.
	 * \return   const int* Returns -1 in the 0 index on failure, else the major and minor version in index 0 and 1 respectively.
	 */
	const int* Start();

	/**
	 * \brief Causes an update in the system based on the change in time.
	 *
	 * Updates the state of the system based off how much time has elapsed since the last update.
	 * \param[in] const float delta The change in time since the last update
	 * \return bool Returns true if we had an update interval passed.
	 */
	bool Update(const double delta);

	/**
	 * \brief Move the camera around the world.
	 *
	 * Moves the camera around by the specified amounts.
	 * \param[in/out] float x
	 * \param[in/out] float y
	 * \param[in/out] float z
	 * \return   void
	 */
	void Move(float x, float y, float z);

	/**
	 * \brief Rotates the camera around the world.
	 *
	 * Rotates the camera around by the specified amounts.
	 * \param[in/out] float x
	 * \param[in/out] float y
	 * \param[in/out] float z
	 * \return   void
	 */
	void Rotate(float x, float y, float z);

	/**
	 * \brief Sets the window width and height for glViewport
	 *
	 * \param[in] int width
	 * \param[in] int height
	 * \return void
	 */
	void SetWindowDim(int width, int height) { this->windowWidth = width; this->windowHeight = height; }

	IGLView *GetView() { return this->view; }

	/**
	 * \brief Sets the viewport width and height.
	 *
	 * \param[in/out] const unsigned int width
	 * \param[in/out] const unsigned int height
	 */
	void SetViewportSize(const unsigned int width, const unsigned int height);

    std::map<std::string,FactoryFunction> getFactoryFunctions();

	void createGLSprite(const std::string type, const unsigned int entityID, std::vector<Property> &properties) ;
	void createGLIcoSphere(const std::string type, const unsigned int entityID, std::vector<Property> &properties) ;
	void createGLCubeSphere(const std::string type, const unsigned int entityID, std::vector<Property> &properties) ;
	void createGLMesh(const std::string type, const unsigned int entityID, std::vector<Property> &properties) ;

	IGLView* View() const { return view; }
private:
	unsigned int windowWidth; // Store the width of our window
	unsigned int windowHeight; // Store the height of our window

	GLuint m_vaoID[2]; // two vertex array objects, one for each drawn object
	GLuint m_vboID[3]; // three VBOs
	int OpenGLVersion[2];

	glm::mat4 ProjectionMatrix;
	IGLView* view;
	double deltaAccumulator;
};
