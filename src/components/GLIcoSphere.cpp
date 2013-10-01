#include "GLIcoSphere.h"
#include "GL/glew.h"
#include <map>
#include <stdint.h>

#include <iostream>

namespace Sigma{

    GLIcoSphere::GLIcoSphere( const int entityID ) : GLMesh(entityID) {
        // all initialization handled by GLMesh
    }

    void GLIcoSphere::InitializeBuffers() {
        srand(this->GetEntityID());

        // Create the verts to begin refining at.
        double t = (1.0 + glm::sqrt(5.0)) / 2.0;
        glm::vec2 coordPair = glm::normalize(glm::vec2(1,t));

        AddVertex(Vertex(-coordPair.r, coordPair.g, 0));
        AddVertex(Vertex(coordPair.r, coordPair.g, 0));
        AddVertex(Vertex(-coordPair.r, -coordPair.g, 0));
        AddVertex(Vertex(coordPair.r, -coordPair.g, 0));

        AddVertex(Vertex(0, -coordPair.r, coordPair.g));
        AddVertex(Vertex(0, coordPair.r, coordPair.g));
        AddVertex(Vertex(0, -coordPair.r, -coordPair.g));
        AddVertex(Vertex(0, coordPair.r, -coordPair.g));

        AddVertex(Vertex(coordPair.g, 0, -coordPair.r));
        AddVertex(Vertex(coordPair.g, 0, coordPair.r));
        AddVertex(Vertex(-coordPair.g, 0, -coordPair.r));
        AddVertex(Vertex(-coordPair.g, 0, coordPair.r));

        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));

        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));

        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));
        AddVertexColor((rand() % 6) > 0 ? Color(0,0,1) : Color(0,1,0));

        AddFace(Face(0,11,5));
        AddFace(Face(0,5,1));
        AddFace(Face(0,1,7));
        AddFace(Face(0,7,10));
        AddFace(Face(0,10,11));

        AddFace(Face(1,5,9));
        AddFace(Face(5,11,4));
        AddFace(Face(11,10,2));
        AddFace(Face(10,7,6));
        AddFace(Face(7,1,8));

        AddFace(Face(3,9,4));
        AddFace(Face(3,4,2));
        AddFace(Face(3,2,6));
        AddFace(Face(3,6,8));
        AddFace(Face(3,8,9));

        AddFace(Face(4,9,5));
        AddFace(Face(2,4,11));
        AddFace(Face(6,2,10));
        AddFace(Face(8,6,7));
        AddFace(Face(9,8,1));

        // Refine the IcoSphere by 4 levels. This results in 20*4^4 = 5120 faces.
        Refine(4);
        std::cout << "after refinement, there are " << this->faces.size() << " faces " << std::endl;

        AddMeshGroupIndex(0);

        std::vector<glm::vec3> surfaceNorms;

        // compute surface normals
        for(size_t i = 0; i < GetFaceCount(); i++) {
            glm::vec3 vector1, vector2, cross, normal;
            const Face* f = GetFace(i);
            Vertex vert1(verts[f->v1]), vert2(verts[f->v2]), vert3(verts[f->v3]);

            // make vectors out of 2 of the edges of this face
            vector1 = glm::normalize(glm::vec3(vert2.x-vert1.x, vert2.y-vert1.y, vert2.z-vert1.z));
            vector2 = glm::normalize(glm::vec3(vert3.x-vert1.x, vert3.y-vert1.y, vert3.z-vert1.z));
            // cross of 2 vectors gives a vector perpendicular to both (i.e. the normal vector)
            cross = glm::cross(vector1, vector2);
            normal = glm::normalize(cross);
            surfaceNorms.push_back(normal);
        }

        // compute vertex normals (average of normals of its 3 faces)
        // TODO: should probably compute adjacency first, this could be slow
        // TODO(2): isn't the vertex normal the same as the normalized coordinate of the vertex...?
        for(size_t i = 0; i < verts.size(); i++) {
            glm::vec3 total_normals(0.0f, 0.0f, 0.0f);

            for(size_t j = 0; j < GetFaceCount(); j++) {
                const Face* f = GetFace(j);
                if (f->v1 == i || f->v2 == i || f->v3 == i) {
                    total_normals.x += surfaceNorms[j].x;
                    total_normals.y += surfaceNorms[j].y;
                    total_normals.z += surfaceNorms[j].z;
                }
            }

            glm::vec3 final_normal(total_normals.x, total_normals.y, total_normals.z);
            final_normal = glm::normalize(final_normal);
            AddVertexNormal(Vector3f(final_normal.x, final_normal.y, final_normal.z));
        }

        surfaceNorms.clear();
        GLMesh::InitializeBuffers();
    }

    void GLIcoSphere::Render(glm::mediump_float *view, glm::mediump_float *proj) {
        GLMesh::Render(view, proj);
    }

    Vertex GLIcoSphere::GetUnitSphereMidPoint(const Vertex &v1, const Vertex &v2) const {
        Vector3f vec((v1.x + v2.x) / 2.0, (v1.y + v2.y) / 2.0, (v1.z + v2.z) / 2.0);
        vec.normalize();
        return Vertex(vec.x, vec.y, vec.z);
    }

    void GLIcoSphere::Refine(int level) {
        // cache of midpoints. allows lookup from 2 vertices to their midpoint
        std::map<int64_t, int> cache;

        for (int i = 0; i < level; ++i) {
            std::vector<Face> tempFaces; // placeholder for next level of subdivision
            for (auto faceitr = this->faces.begin(); faceitr != this->faces.end(); ++faceitr) {
                // index of midpoints of v1--v2, v2--v3, and v3--v1, respectively
                int a, b, c;
                {
                    int v1 = (*faceitr).v1;
                    int v2 = (*faceitr).v2;
                    a = this->CreateOrGetMidpoint(cache, v1, v2);
                }
                {
                    int v1 = (*faceitr).v2;
                    int v2 = (*faceitr).v3;
                    b = this->CreateOrGetMidpoint(cache, v1, v2);
                }
                {
                    int v1 = (*faceitr).v3;
                    int v2 = (*faceitr).v1;
                    c = this->CreateOrGetMidpoint(cache, v1, v2);
                }

                tempFaces.push_back(Face((*faceitr).v1, a, c));
                tempFaces.push_back(Face((*faceitr).v2, b, a));
                tempFaces.push_back(Face((*faceitr).v3, c, b));
                tempFaces.push_back(Face(a, b, c));
            }
            this->faces = tempFaces;
        }
    } // function Refine

    int GLIcoSphere::CreateOrGetMidpoint(std::map<int64_t,int> &cache, const int v1, const int v2){
        // cache standard: smaller vertex index first.
        int64_t key = v1 < v2
                ? (static_cast<int64_t>(v1) << 32 | static_cast<int64_t>(v2))
                : (static_cast<int64_t>(v2) << 32 | static_cast<int64_t>(v1));
        // placeholders for random "terrain gen" of land (green) and water (blue)
        float green, blue;
        // look up midpoint based on key hash
        auto found = cache.find(key);
        if(found != cache.end()){
            return found->second;
        } else{
            // create and save vertex
            int next_index = cache[key] = this->verts.size();
            AddVertex(GetUnitSphereMidPoint(this->verts[v1], this->verts[v2]));
            RefineColor(v1, v2, &green, &blue);
            AddVertexColor(Color(0.0f, green, blue));
            return next_index;
        }
    }

    void GLIcoSphere::RefineColor(const int v1, const int v2, float* green, float* blue) const {
        *green = 0.0f; *blue = 0.0f; // exactly one of these will be set to 1 by the end of this function
        int bcount = 0, gcount = 0;
        if (GetVertexColor(v1)->b > 0.0f)
            bcount++;
        if (GetVertexColor(v2)->b > 0.0f)
            bcount++;
        if (GetVertexColor(v1)->g > 0.0f)
            gcount++;
        if (GetVertexColor(v2)->g > 0.0f)
            gcount++;
        // one is ocean, the other is land. refinement is 50% chance of either.
        if (bcount == gcount) {
            int val = rand() % 2;
            if (val > 0) {
                *blue = 1.0f;
            } else {
                *green = 1.0f;
            }
        // we're either mid-ocean with a low chance of making a tiny island, or
        //  mid-land with a small chance of making a lake.
        } else {
            int val = rand() % 6;
            if (val > 1) {
                *blue = 1.0f;
            } else {
                *green = 1.0f;
            }
        }
    } // function RefineColor
} // namespace Sigma
