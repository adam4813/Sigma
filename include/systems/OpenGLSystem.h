#pragma once
#ifndef OPENGLSYSTEM_H
#define OPENGLSYSTEM_H

#include "Property.h"
#include "../IFactory.h"
#include "../ISystem.h"
#include "../IGLComponent.h"
#include "../systems/IGLView.h"

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <memory>

struct IGLView;

namespace Sigma{

	struct RenderTarget {
		GLuint texture_id;
		GLuint fbo_id;
		GLuint depth_id;

		RenderTarget() : texture_id(0), fbo_id(0), depth_id(0) {}
		virtual ~RenderTarget();

		void Use(int slot);
	};

    class OpenGLSystem
        : public Sigma::IFactory, public ISystem<IComponent> {
    public:

        OpenGLSystem();

        /**
         * \brief Starts the OpenGL rendering system.
         *
         * Starts the OpenGL rendering system and creates a rendering context. It will also attempt to create a newer rendering context (>3) if available.
         * \return -1 in the 0 index on failure, else the major and minor version in index 0 and 1 respectively.
         */
        const int* Start();

        /**
         * \brief Causes an update in the system based on the change in time.
         *
         * Updates the state of the system based off how much time has elapsed since the last update.
         * \param delta the time (in milliseconds) since the last update
         * \return true if rendering was performed
         */
        bool Update(const double delta);

        /**
         * \brief Move the camera around the world.
         *
         * Moves the camera around by the specified amounts.
         * \param x, y, z direction relative to the camera, with x right, y up..
         */
        void Move(float x, float y, float z);

        /**
         * \brief Rotates the camera around the world.
         *
         * Rotates the camera around by the specified amounts.
         * \param x, y, z rotation amount (in radians) relative to current viewing orientation
         * \return   void
         */
        void Rotate(float x, float y, float z);

        /**
         * \brief Sets the window width and height for glViewport
         *
         * \param width new width for the window
         * \param height new height for the window
         * \return void
         */
        void SetWindowDim(int width, int height) { this->windowWidth = width; this->windowHeight = height; }

        /**
         * \brief Sets the viewport width and height.
         *
         * \param width new viewport width
         * \param height new viewport height
         */
        void SetViewportSize(const unsigned int width, const unsigned int height);

        /**
         * \brief set the framerate at runtime
         *
         *  Note that the constructor sets a default of 60fps
         */
        void SetFrameRate(double fr){ this->framerate = fr; }

        std::map<std::string,FactoryFunction> getFactoryFunctions();

        void createGLSprite(const unsigned int entityID, std::vector<Property> &properties) ;
        void createGLIcoSphere(const unsigned int entityID, std::vector<Property> &properties) ;
        void createGLCubeSphere(const unsigned int entityID, std::vector<Property> &properties) ;
        void createGLMesh(const unsigned int entityID, std::vector<Property> &properties) ;
		void createPointLight(const unsigned int entityID, std::vector<Property> &properties);
		void createScreenQuad(const unsigned int entityID, std::vector<Property> &properties);

		// Views are not technically components, but perhaps they should be
		void createGLView(const unsigned int entityID, std::vector<Property> &properties, std::string mode) ;

		// Managing rendering internals
		/*
		 * \brief creates a new render target of desired size
		 */
		int createRenderTarget(const unsigned int w, const unsigned int h, const unsigned int format);
		
		/*
		 * \brief returns the fbo_id of primary render target (index 0)
		 */
		int getRender() { return this->renderTargets[0]->fbo_id; }
		int getRenderTexture() { return this->renderTargets[0]->texture_id; }

        IGLView* View() const { return this->view.get(); }

		GLTransform* GetTransformFor(const unsigned int entityID);
		std::string GetViewMode() { return this->viewMode; }

		// Rendering methods
		void RenderTexture(GLuint texture_id);

    private:
        unsigned int windowWidth; // Store the width of our window
        unsigned int windowHeight; // Store the height of our window

        int OpenGLVersion[2];

		// Scene matrices
        glm::mat4 ProjectionMatrix;
        std::unique_ptr<IGLView> view;

		// Time tracking to fix render frame rate
        double deltaAccumulator; // milliseconds since last render
        double framerate; // default is 60fps
		
		// Type of view to create
		std::string viewMode;

		// Render targets to draw to
		std::vector<std::unique_ptr<RenderTarget>> renderTargets;
    }; // class OpenGLSystem
} // namespace Sigma
#endif // OPENGLSYSTEM_H