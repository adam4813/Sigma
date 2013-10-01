#pragma once
#ifndef GLMESH_H
#define GLMESH_H

#include "../GLTransform.h"
#include "../IGLComponent.h"
#include "../systems/GLSLShader.h"

#include "GL/glew.h"
#include "SOIL/SOIL.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <memory>

#include <fstream>
#include <iostream>
#include <sstream>

namespace Sigma{

    // Helper structs for OBJ loading
    // Stores unique combinations of indices
    struct VertexIndices {
            unsigned int vertex;
            unsigned int normal;
            unsigned int uv;
            unsigned int color;
    };

    // Stores a face with all indices (also used for OBJ loading)
    struct FaceIndices {
        VertexIndices v[3];
    };

    class GLMesh : public IGLComponent {
    public:
        typedef std::unordered_map<std::string, std::shared_ptr<GLSLShader>> ShaderMap;
        SET_COMPONENT_ID("GLMesh");
        GLMesh(const int entityID);
        virtual ~GLMesh(){}

        /**
         * \brief Initializes the mesh in the OpenGL context.
         *
         * \param entityID The entity this component belongs to.
         */
        void InitializeBuffers();

        /** \brief render the mesh with
         *
         * Uses the given view and projection matrices. additional uniforms may be set in the args array.
         *
         * \param view the view matrix
         * \param proj the projection matrix
         * \return void
         */
        virtual void Render(glm::mediump_float *view, glm::mediump_float *proj);

        /**
         * \brief Returns the number of elements to draw for this component.
         *
         * Call this multiple times to get the elements for each mesh group. A return of 0 indicates all
         * mesh groups have been returned, and the call loop should end.
         * \param group The mesh group to render.
         * \return unsigned int The number of elements to draw for the given mesh group.
         */
        unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const {
            if (this->groupIndex.size() == 0) {
                return 0;
            }
            if ((group + 1) < (this->groupIndex.size())) {
                return (this->groupIndex[group+1] - this->groupIndex[group]) * 3;
            } else if (group > (this->groupIndex.size() - 1)) {
                return 0;
            } else {
                return (this->faces.size() - this->groupIndex[group]) * 3;
            }
        }

        void LoadMesh(std::string fname);

        void ParseMTL(std::string fname);

        /**
         * \brief Add a vertex to the list.
         *
         * \param v The vertex to add. It is copied.
         */
        void AddVertex(const Vertex& v) {
            this->verts.push_back(v);
        }

        /**
         * \brief Gets a vertex.
         *
         * Returns the vertex at the specific index.
         * \param index The index of the vertex to get.
         * \return   const Vertex* The vertex at the index or nullptr if the index was invalid.
         */
        const Vertex* GetVertex(const unsigned int index) const {
            if(0 <= index && index < this->verts.size())
                return &this->verts[index];
            return nullptr;
        }

        /**
         * \brief Add a face to the list.
         *
         * \param f The face to add. It is copied.
         */
        void AddFace(const Face& f) {
            this->faces.push_back(f);
        }

        /**
         * \brief Gets a face.
         *
         * Returns the face at the specific index.
         * \param index The index of the face to get.
         * \return   const Face* The face at the index or nullptr if the index was invalid.
         */
        const Face* GetFace(const unsigned int index) const {
            if(0 <= index && index < this->faces.size())
                return &this->faces[index];
            return nullptr;
        }

        bool RemoveFace(const unsigned int index) {
            if(0 <= index && index < this->faces.size()){
                this->faces.erase(this->faces.begin() + index);
                return true;
            }
            return false;
        }


        unsigned int GetFaceCount() const {
            return this->faces.size();
        }

        /**
         * \brief Adds a mesh group index.
         *
         * Adds a mesh group starting index to groupIndex. This is the starting face index for the mesh group.
         * \param index the index of the new mesh group
         */
        void AddMeshGroupIndex(const unsigned int index) {
            this->groupIndex.push_back(index);
        }

        /**
         * \brief Add a vertex normal to the list.
         *
         * \param v The vertex normal to add. It is copied.
         */
        void AddVertexNormal(const Vector3f& vn) {
            this->vertNorms.push_back(vn);
        }

        /**
         * \brief Add a vertex color to the list.
         *
         * \param v The vertex color to add. It is copied.
         */
        void AddVertexColor(const Color& c) {
            this->colors.push_back(c);
        }

        /**
         * \brief Gets a vertex color.
         *
         * Returns the vertex color at the specific index.
         * \param index The index of the color to get.
         * \return   const Color* The color at the index or nullptr if the index was invalid.
         */
        const Color* GetVertexColor(const unsigned int index) const {
            if(0 <= index && index < this->colors.size())
                return &this->colors[index];
            return nullptr;
        }

        /** \brief load the given shader
         *
         * \param filename the base name of the shader. loads filename.vert and filename.frag.
         *  filename should be a relative path, like "shaders/mesh"
         * \return void
         */
        void LoadShader(const std::string& filename);

        /** \brief load the default shader "shaders/mesh" */
        void LoadShader();

        static const std::string DEFAULT_SHADER;

    protected:
        // Note that these values are protected, not private! Inheriting classes get access to these
        //  basic drawing elements.
        std::vector<unsigned int> groupIndex; // Stores which index in faces a group starts at.
        std::vector<Face> faces; // Stores vectors of face groupings.
        std::map<unsigned int, std::string> faceGroups; // Stores a mapping of material name to face grouping
        std::vector<Vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
        std::vector<Vector3f> vertNorms; // The vertex normals for each vert.
        std::vector<TexCoord> texCoords; // The texture coords for each vertex.
        std::vector<Color> colors;
        std::map<std::string, Material> mats;

        std::shared_ptr<GLSLShader> shader; // A mesh may use any shader, and they may be shared
        // name-->shader map to look up already-loaded shaders
        static ShaderMap loadedShaders;
    }; // class GLMesh

} // namespace Sigma

#endif // GLMESH_H
