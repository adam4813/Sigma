#pragma once

#include "../Property.h"

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

struct IGLView;
class IGLComponent;

class OpenGLSystem {
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
	const int* Start();

	/**
	 * \brief A factory to create new components of a given type.
	 *
	 * A factory method to create various components and add them to the system. These components will be used during the system update method
	 * \param[in] const std::string type The type of componenet to create
	 * \param[in] const int entityID The ID of the entity this component belongs to.
	 * \param[in] std::vector<Property> &properties A vector containing the properties to apply to the created component.
	 * \returns   IComponent* The newly create component
	 */
	IGLComponent* Factory(const std::string type, const unsigned int entityID, std::vector<Property> &properties) ;

	/**
	 * \brief Causes an update in the system based on the change in time.
	 *
	 * Updates the state of the system based off how much time has elapsed since the last update.
	 * \param[in] const float delta The change in time since the last update
	 * \returns bool Returns true if we had an update interval passed.
	 */
	bool Update(const double delta);

	/**
	 * \brief Retrieve the component that belongs to the given entity ID
	 * 
	 * \param[in] const unsigned int entityID
	 * \param[in] const unsigned int componentID
	 * \returns   IComponent* The component that belongs to the entity ID or nullptr if no component exists for the given ID.
	 */
	IGLComponent* GetComponent(const unsigned int entityID, const unsigned int componentID);

	/**
	 * \brief Move the camera around the world.
	 *
	 * Moves the camera around by the specified amounts.
	 * \param[in/out] float x
	 * \param[in/out] float y
	 * \param[in/out] float z
	 * \returns   void 
	 */
	void Move(float x, float y, float z);

	/**
	 * \brief Rotates the camera around the world.
	 *
	 * Rotates the camera around by the specified amounts.
	 * \param[in/out] float x
	 * \param[in/out] float y
	 * \param[in/out] float z
	 * \returns   void
	 */
	void Rotate(float x, float y, float z);

	/**
	 * \brief Sets the viewport width and height.
	 *
	 * \param[in/out] const unsigned int width
	 * \param[in/out] const unsigned int height
	 */
	void SetViewportSize(const unsigned int width, const unsigned int height);


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
	std::map<int, std::map<int, IGLComponent*> > components; // A mapping of entity ID to a mapping of component ID to component.
};
