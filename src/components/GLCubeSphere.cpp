#include "GLCubeSphere.h"

#include "SOIL/SOIL.h"

#include <vector>
#include <sstream>

const float epsilon = 0.0001f;

// For std::find
namespace Sigma {
	bool operator ==(const Vertex &lhs, const Vertex &rhs) { return ((abs(rhs.x - lhs.x) < epsilon) &&
																	 (abs(rhs.y - lhs.y) < epsilon) &&
																	 (abs(rhs.z - lhs.z) < epsilon)); }

    GLCubeSphere::GLCubeSphere( const int entityID ) : GLMesh(entityID) {
        // initialization handled by GLMesh or InitializeBuffers
    }

    GLCubeSphere::~GLCubeSphere() {
        glDeleteTextures(1, &this->_cubeMap);
        if(this->_cubeNormalMap != 0)
            glDeleteTextures(1, &this->_cubeNormalMap);
    }

    void GLCubeSphere::InitializeBuffers() {
        srand(this->GetEntityID());

        // add vertices of cube corners. these are basis of the first faces.
        float t = 1.0f;

        AddVertex(Vertex(-t, -t,  t));
        AddVertex(Vertex( t, -t,  t));
        AddVertex(Vertex( t,  t,  t));
        AddVertex(Vertex(-t,  t,  t));
        AddVertex(Vertex(-t, -t, -t));
        AddVertex(Vertex( t, -t, -t));
        AddVertex(Vertex( t,  t, -t));
        AddVertex(Vertex(-t,  t, -t));

        // front
        AddFace(Face(0,1,2));
        AddFace(Face(2,3,0));
        // top
        AddFace(Face(3,2,6));
        AddFace(Face(6, 7, 3));
        // back
        AddFace(Face(7, 6, 5));
        AddFace(Face(5, 4, 7));
        // bottom
        AddFace(Face(4, 5, 1));
        AddFace(Face(1, 0, 4));
        // left
        AddFace(Face(4, 0, 3));
        AddFace(Face(3, 7, 4));
        // right
        AddFace(Face(1, 5, 6));
        AddFace(Face(6, 2, 1));

        Refine(this->_subdivisionLevels);

        // a cubesphere is a mesh, but it is only one mesh group.
        AddMeshGroupIndex(0);

        // You may notice that in GLIcoSphere, vertex normals are computed here.
        //  Cubesphere normals are computed in the shader 'shaders/cubesphere.vert'.
        //  TODO get GLIcoSphere's normal calculations into a shader too

        GLMesh::InitializeBuffers();

        // shader program was compiled and linked in GLMesh::InitializeBuffers.
        //  Now we can set relevant custom uniform values
        (*shader).Use();
        glUniform1i(glGetUniformLocation((*shader).GetProgram(), "cubeMap"), GL_TEXTURE0);
        glUniform1i(glGetUniformLocation((*shader).GetProgram(), "cubeNormalMap"), GL_TEXTURE1);
        (*shader).UnUse();
    } // function InitializeBuffers

    void GLCubeSphere::LoadTexture(std::string texture_name) {
        // SOIL makes this straightforward..
		char filename[100];
		std::string filenames[6];
        {
            // LOAD CUBE VISUAL TEXTURES
			std::cout << "Loading cube texture: " << texture_name << std::endl;
			sprintf(filename, "%s.dds", texture_name.c_str());

			this->_cubeMap = SOIL_load_OGL_single_cubemap(filename, SOIL_DDS_CUBEMAP_FACE_ORDER, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_DDS_LOAD_DIRECT);
	
			// if that didn't work, load individual files
			if(this->_cubeMap == 0) {
				// There are always six files
				for(int i=0; i < 6; i++) {
					std::stringstream sstm;
					sstm << texture_name << (i+1) << ".jpg";
					filenames[i] = sstm.str();
				}

				// SOIL will load the image files into textures, then return the id
				//  of the GL_TEXTURE_CUBE_MAP
				this->_cubeMap = SOIL_load_OGL_cubemap(filenames[0].c_str(), filenames[1].c_str(), filenames[2].c_str(),
													   filenames[3].c_str(), filenames[4].c_str(), filenames[5].c_str(),
													   SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

				if( 0 == this->_cubeMap ) {
					printf( "SOIL error loading cubemap: '%s'\n", SOIL_last_result() );
				}
			}
        }
        {
			// First try dds file
			sprintf(filename, "%s_nm.dds", texture_name.c_str());

			this->_cubeNormalMap = SOIL_load_OGL_single_cubemap(filename, SOIL_DDS_CUBEMAP_FACE_ORDER, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_DDS_LOAD_DIRECT);
	
			if(this->_cubeNormalMap==0) {
				// LOAD CUBE NORMAL TEXTURES
				for(int i=0; i < 6; i++) {
					std::stringstream sstm;
					sstm << texture_name << "_nm" << (i+1) << ".jpg";
					filenames[i] = sstm.str();
				}

				this->_cubeNormalMap = SOIL_load_OGL_cubemap(filenames[0].c_str(), filenames[1].c_str(), filenames[2].c_str(),
													   filenames[3].c_str(), filenames[4].c_str(), filenames[5].c_str(),
													   SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
				if( 0 == this->_cubeNormalMap ) {
					printf( "SOIL error loading cubemap normals: '%s'\n", SOIL_last_result() );
				}
			}
        }
    } // function LoadTexture

	Vertex GLCubeSphere::GetMidPoint(const Vertex& v1, const Vertex& v2){
        return Vertex((v1.x + v2.x)/2.0f, (v1.y + v2.y)/2.0f, (v1.z + v2.z)/2.0f);
    }

    int GLCubeSphere::CreateOrGetMidpoint(std::map<int64_t,int> &cache, const int v1, const int v2){
        // cache standard: smaller vertex index first.
        int64_t key = v1 < v2
                ? (static_cast<int64_t>(v1) << 32 | static_cast<int64_t>(v2))
                : (static_cast<int64_t>(v2) << 32 | static_cast<int64_t>(v1));
        // look up midpoint based on key hash
        auto found = cache.find(key);
        if(found != cache.end()){
            return found->second;
        } else{
            // create and save vertex
            int next_index = cache[key] = this->verts.size();
            //Vertex midpt();
            AddVertex(GetMidPoint(this->verts[v1], this->verts[v2]));
            return next_index;
        }
    }

    void GLCubeSphere::Refine(int level) {
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

	void GLCubeSphere::Render(glm::mediump_float *view, glm::mediump_float *proj) {
        if(this->_fixToCamera) {
            glm::mediump_float *view_ptr = view;
            glm::mat4 view_matrix;

            for(int i=0; i < 4; i++) {
                for(int j=0; j < 4; j++) {
                    view_matrix[i][j] = (*view_ptr++);
                }
            }

            // Extract position from view matrix
            glm::mat3 rotMat(view_matrix);
            glm::vec3 d(view_matrix[3]);
            glm::vec3 position = -d * rotMat;
            this->Transform()->TranslateTo(position);
        }

        // bind cubemap textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->_cubeMap);
        if(this->_cubeNormalMap != 0){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, this->_cubeNormalMap);
        }

        // render da mesh
        GLMesh::Render(view, proj);

		// unbind that which GLMesh does not unbind
        if(this->_cubeNormalMap != 0){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glActiveTexture(GL_TEXTURE0);
    } // function Render
} // namespace Sigma