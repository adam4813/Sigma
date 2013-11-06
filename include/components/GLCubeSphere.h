#pragma once
#ifndef GLCUBESPHERE_H
#define GLCUBESPHERE_H

#include "GLMesh.h"

#include <stdint.h>

namespace Sigma {
    class GLCubeSphere : public GLMesh {
    public:
        SET_COMPONENT_TYPENAME("GLCubeSphere");
        // We have a private ctor so the factory method must be used.
        GLCubeSphere(const int entityID = 0);
        ~GLCubeSphere();

        /**
         * \brief Generates new cube and subdivides; loads textures
         *
         * Creates buffers in the current OpenGL context. Makes VBOs, VAO, etc, and populates
         * them to create the cubesphere.
         * \return   void
         */
        void InitializeBuffers();

        /**
         * \brief Updates the rotation and renders the mesh
         *
         * \return void
         */
        void Render(glm::mediump_float *view, glm::mediump_float *proj);

        /**
         * \brief Returns the number of elements to draw for this component.
         *
         *  Because this is a mesh with one big component, only the 0th group has any elements (#faces * 3).
         * \param Group the index of the mesh group to count
         * \return unsigned int The number of elements to draw.
         */
        virtual unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const {
            if (group > 0) {
                return 0;
			}
            return this->faces.size() * 3;
        }

        /**
         * \brief Helper method to subdivide the cube recursively
         *
         *  Each level of subdivision divides a triangle into 4 parts, so the number
         *  of triangles for this cubesphere will be 12 * 4^levels. Note that with
         *  levels=9, there are over *3 MILLION* tris. do not do this.
         *
         * \param level the number of levels of subdivision to apply
         */
        void Refine(int level);

        /** \brief Load cubemap texture
         *
         * given <texture_name>, loads images <texture_name>1.jpg -- <texture_name>6.jpg as the
         *  6 sides of the cube.
         * \param texture_name the base name of the jpg images to be loaded
         * \return void
         */
        bool LoadTexture(std::string texture_name);

        // helper methods for constructing a GLCubeSphere object from parameters,
        //  like in SCParser
        void SetSubdivisions(int levels) { this->_subdivisionLevels = levels; }
        void SetRotationSpeed(float rot_speed) { this->_rotationSpeed = rot_speed; }
        void SetFixToCamera(bool fix_to_camera) { this->_fixToCamera = fix_to_camera; }
    private:
        // OpenGL IDs of the GL_TEXTURE_CUBE_MAP textures
        GLuint _cubeMap, _cubeNormalMap;
        int _subdivisionLevels;
        float _rotationSpeed;

        // Special setting for skyboxes
        bool _fixToCamera;

        // helper functions for refinement
        static Vertex GetMidPoint(const Vertex& v1, const Vertex& v2);
        int CreateOrGetMidpoint(std::map<int64_t,int> &cache, const int v1, const int v2);
    }; // class GLCubeSphere

} // namespace Sigma

#endif // GLCUBESPHERE_H

