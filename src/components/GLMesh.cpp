#include "components/GLMesh.h"

#ifndef __APPLE__
#include "GL/glew.h"
#endif
#include "strutils.h"

#include <algorithm>
#include <stdexcept>

#include <fstream>
#include <iostream>
#include <sstream>
#include "resources/GLTexture.h"
#include "systems/OpenGLSystem.h"

namespace Sigma{

    // static member initialization
    const std::string GLMesh::DEFAULT_SHADER = "shaders/mesh_deferred";

    GLMesh::GLMesh(const int entityID) : IGLComponent(entityID) {
        memset(&this->buffers, 0, sizeof(this->buffers));
        this->vao = 0;
        this->drawMode = GL_TRIANGLES;
        this->ElemBufIndex = 2;
        this->ColorBufIndex = 1;
        this->VertBufIndex = 0;
        this->NormalBufIndex = 3;
        this->UVBufIndex = 4;
    }

    void GLMesh::InitializeBuffers() {

		if(!this->shader) {
			assert(0 && "Shader must be loaded before buffers can be initialized.");
		}

        // We must create a vao and then store it in our GLMesh.
        if (this->vao == 0) {
            glGenVertexArrays(1, &this->vao); // Generate the VAO
        }
        glBindVertexArray(this->vao); // Bind the VAO

        if (this->verts.size() > 0) {
            if (this->buffers[this->VertBufIndex] == 0) {
                glGenBuffers(1, &this->buffers[this->VertBufIndex]); 	// Generate the vertex buffer.
            }
            glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->VertBufIndex]); // Bind the vertex buffer.
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->verts.size(), &this->verts.front(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
            GLint posLocation = glGetAttribLocation((*shader).GetProgram(), "in_Position"); // Find the location in the shader where the vertex buffer data will be placed.
            glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // Tell the VAO the vertex data will be stored at the location we just found.
            glEnableVertexAttribArray(posLocation); // Enable the VAO line for vertex data.
        }
        if (this->texCoords.size() > 0) {
            if (this->buffers[this->UVBufIndex] == 0) {
                glGenBuffers(1, &this->buffers[this->UVBufIndex]); 	// Generate the vertex buffer.
            }
            glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->UVBufIndex]); // Bind the vertex buffer.
            glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord) * this->texCoords.size(), &this->texCoords.front(), GL_STATIC_DRAW); // Stores the verts in the vertex buffer.
            GLint uvLocation = glGetAttribLocation((*shader).GetProgram(), "in_UV"); // Find the location in the shader where the vertex buffer data will be placed.
            glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); // Tell the VAO the vertex data will be stored at the location we just found.
            glEnableVertexAttribArray(uvLocation); // Enable the VAO line for vertex data.
        }
        if (this->colors.size() > 0) {
            if (this->buffers[this->ColorBufIndex] == 0) {
                glGenBuffers(1, &this->buffers[this->ColorBufIndex]);
            }
            glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->ColorBufIndex]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * this->colors.size(), &this->colors.front(), GL_STATIC_DRAW);
            GLint colLocation = glGetAttribLocation((*shader).GetProgram(), "in_Color");
            glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(colLocation);
        }
        if (this->faces.size() > 0) {
            if (this->buffers[this->ElemBufIndex] == 0) {
                glGenBuffers(1, &this->buffers[this->ElemBufIndex]); // Generate the element buffer.
            }
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[this->ElemBufIndex]); // Bind the element buffer.
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * this->faces.size(), &this->faces.front(), GL_STATIC_DRAW); // Store the faces in the element buffer.
        }
        if (this->vertNorms.size() > 0) {
            if (this->buffers[this->NormalBufIndex] == 0) {
                glGenBuffers(1, &this->buffers[this->NormalBufIndex]);
            }
            glBindBuffer(GL_ARRAY_BUFFER, this->buffers[this->NormalBufIndex]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*this->vertNorms.size(), &this->vertNorms[0], GL_STATIC_DRAW);
            GLint normalLocation = glGetAttribLocation((*shader).GetProgram(), "in_Normal");
            glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(normalLocation);
        }

        glBindVertexArray(0); // Reset the buffer binding because we are good programmers.

		this->shader->Use();
		this->shader->AddUniform("in_Model");
		this->shader->AddUniform("in_View");
		this->shader->AddUniform("in_Proj");
		this->shader->AddUniform("texEnabled");
		this->shader->AddUniform("ambientTexEnabled");
		this->shader->AddUniform("diffuseTexEnabled");
		this->shader->AddUniform("texAmb");
		this->shader->AddUniform("texDiff");
		this->shader->UnUse();
    }

    void GLMesh::Render(glm::mediump_float *view, glm::mediump_float *proj) {
        glm::mat4 modelMatrix = this->Transform()->GetMatrix();

		//if(this->parentTransform != 0) {
		//	modelMatrix = this->parentTransform->GetMatrix() * modelMatrix;
		//}

        this->shader->Use();
        glUniformMatrix4fv((*this->shader)("in_Model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv((*this->shader)("in_View"), 1, GL_FALSE, view);
        glUniformMatrix4fv((*this->shader)("in_Proj"), 1, GL_FALSE, proj);

        glBindVertexArray(this->Vao());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->GetBuffer(this->ElemBufIndex));

        if(this->cull_face == 0) {
            glDisable(GL_CULL_FACE);
        }
        else {
            glCullFace(this->cull_face);
        }

        glActiveTexture(GL_TEXTURE0);
        size_t prev = 0;
        for (int i = 0, cur = this->MeshGroup_ElementCount(0); cur != 0; prev = cur, cur = this->MeshGroup_ElementCount(++i)) {
            if (this->faceGroups.size() > 0) {
                Material& mat = this->mats[this->faceGroups[prev]];

				if (mat.ambientMap) {
					glUniform1i((*this->shader)("texEnabled"), 1);
					glUniform1i((*this->shader)("ambientTexEnabled"), 1);
					glUniform1i((*this->shader)("texAmb"), 1);
					glBindTexture(GL_TEXTURE_2D, mat.ambientMap);
					glActiveTexture(GL_TEXTURE1);
				} else {
					glUniform1i((*this->shader)("ambientTexEnabled"), 0);
				}

				if (mat.diffuseMap) {
					glUniform1i((*this->shader)("texEnabled"), 1);
					glUniform1i((*this->shader)("diffuseTexEnabled"), 1);
					glUniform1i((*this->shader)("texDiff"), 0);
					glBindTexture(GL_TEXTURE_2D, mat.diffuseMap);
					glActiveTexture(GL_TEXTURE0);
				} else {
					glUniform1i((*this->shader)("diffuseTexEnabled"), 0);
				}
            }
			else {
				glUniform1i((*this->shader)("texEnabled"), 0);
				glUniform1i((*this->shader)("diffuseTexEnabled"), 0);
				glUniform1i((*this->shader)("ambientTexEnabled"), 0);
			}
            glDrawElements(this->DrawMode(), cur, GL_UNSIGNED_INT, (void*)prev);
        }

        // reset defaults
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        this->shader->UnUse();
    } // function Render

    bool operator ==(const VertexIndices &lhs, const VertexIndices &rhs) {
        return (lhs.vertex==rhs.vertex &&
                lhs.normal==rhs.normal &&
                lhs.uv==rhs.uv &&
                lhs.color==rhs.color);
    }

    bool GLMesh::LoadMesh(std::string fname) {
		// Extract the path from the filename.
		std::string path;
		if (fname.find("/") != std::string::npos) {
			path = fname.substr(0, fname.find_last_of("/") + 1); // Keep the separator.
		}
		else {
			path = fname.substr(0, fname.find_last_of("\\") + 1); // Keep the separator.
		}

        unsigned int current_color = 0;

        std::vector<FaceIndices> temp_face_indices;
        std::vector<VertexIndices> unique_vertices;

        std::vector<Vertex> temp_verts;
        std::vector<TexCoord> temp_uvs;
        std::vector<Vertex> temp_normals;
        std::vector<Color> temp_colors;

        std::string currentMtlGroup = "";

        std::string line;

        // Attempt to load file
        std::ifstream in(fname, std::ios::in);

        if (!in) {
            std::cerr << "Cannot open mesh " << fname << std::endl;
            return false;
        }

        // Default color if no material is provided is white
        temp_colors.push_back(Color(1.0f, 1.0f, 1.0f));

        // Parse line by line
        while (getline(in, line)) {
            // strip whitespace (strutils.h)
            line = trim(line);
            if (line.substr(0,2) == "v ") { // Vertex position
                float x,y,z;
                std::istringstream s(line.substr(2));
                s >> x; s >> y; s >> z;
                temp_verts.push_back(Vertex(x, y, z));
            }
			else if (line.substr(0,2) == "vt") { //  Vertex tex coord
                float u, v = 0.0f;
                std::istringstream s(line.substr(2));
                s >> u; s >> v;
                temp_uvs.push_back(TexCoord(u,v));
            }
			else if (line.substr(0,2) == "vn") { // Vertex normal
                float x, y, z;
                std::istringstream s(line.substr(2));
                s >> x; s >> y; s >> z;
                temp_normals.push_back(Vertex(x,y,z));
            }
            else if (line.substr(0,2) == "f ") { // Face
                FaceIndices current_face;
                bool has_vert_indices=false, has_uv_indices=false, has_normal_indices=false;
                short indicies[3][3];

                std::string cur = line.substr(2, line.find(' ', 2) - 2);
                std::string left = line.substr(line.find(' ', 2) + 1);

                for (int i = 0; i < 3; ++i) { // Each face contains 3 sets of indicies. Each set is 3 indicies v/t/n.
                    std::string first = cur.substr(0, cur.find('/')); // Substring for the v portion
                    indicies[i][0] = atoi(first.c_str());
                    has_vert_indices=true;

                    if ((cur.find('/') + 1) != cur.find('/', cur.find('/') + 1)) { // Check if we have a t portion
                        std::string second = cur.substr(cur.find('/') + 1, cur.find('/', cur.find('/') + 1)); // Substring for the t portion
                        indicies[i][1] = atoi(second.c_str());
                        has_uv_indices=true;
                    }
					else {
                        indicies[i][1] = 0;
                    }

                    if (cur.find('/', cur.find('/')) != cur.find_last_of('/')) { // Check if we have an n portion
                        std::string third = cur.substr(cur.find_last_of('/') + 1); // Substring for the n portion
                        indicies[i][2] = atoi(third.c_str());
                        has_normal_indices=true;
                    }
					else {
                        indicies[i][2] = 0;
                    }

                    cur = left.substr(0, left.find(' '));
                    left = left.substr(left.find(' ') + 1);
                }
                if(has_vert_indices) {
                    GLushort a,b,c;
                    a = indicies[0][0] - 1; b = indicies[1][0] - 1; c = indicies[2][0] - 1;
                    current_face.v[0].vertex = a;
                    current_face.v[1].vertex = b;
                    current_face.v[2].vertex = c;
                }
                if(has_uv_indices) {
                    GLushort ta,tb,tc;
                    ta = indicies[0][1] - 1; tb = indicies[1][1] - 1; tc = indicies[2][1] - 1;
                    current_face.v[0].uv = ta;
                    current_face.v[1].uv = tb;
                    current_face.v[2].uv = tc;
                }
                if(has_normal_indices) {
                    GLushort na,nb,nc;
                    na = indicies[0][2] - 1; nb = indicies[1][2] - 1; nc = indicies[2][2] - 1;
                    current_face.v[0].normal = na;
                    current_face.v[1].normal = nb;
                    current_face.v[2].normal = nc;
                }

                // Add index to currently active color
                current_face.v[0].color = current_color;
                current_face.v[1].color = current_color;
                current_face.v[2].color = current_color;

                // Store the face
                temp_face_indices.push_back(current_face);
            }
			else if (line.substr(0,1) == "g") { // Face group
                this->groupIndex.push_back(temp_face_indices.size());
            }
			else if (line.substr(0, line.find(' ')) == "mtllib") { // Material library
				// Add the path to the filename to load it relative to the obj file.
                ParseMTL(path + line.substr(line.find(' ') + 1));
            }
			else if (line.substr(0, line.find(' ')) == "usemtl") { // Use material
                std::string mtlname = line.substr(line.find(' ') + 1);

                // Set as current material group
                currentMtlGroup = mtlname;

                // Push back color (for now)
                Material m = this->mats[mtlname];
                glm::vec3 amb(m.ka[0], m.ka[1], m.ka[2]);
                glm::vec3 spec(m.ks[0], m.ks[1], m.ks[2]);
                glm::vec3 dif(m.kd[0], m.kd[1], m.kd[2]);

                glm::vec3 color = amb + dif + spec;
                temp_colors.push_back(Color(color.r, color.g, color.b));
                this->faceGroups[temp_face_indices.size()] = currentMtlGroup;
                current_color++;
            }
			else if ((line.substr(0,1) == "#") || (line.size() == 0)) { // Comment or blank line
                /* ignoring this line comment or blank*/
            }
			else { // Unknown
                /* ignoring this line */
                std::string test = line.substr(0, line.find(' '));
                std::cerr << "Unrecognized line " << line << std::endl;
            }
        }

        // Now we have all raw attributes stored in the temp vectors,
        // and the set of indicies for each face.  Opengl only supports
        // one index buffer, so we must duplicate vertices until
        // all the data lines up.
        for(unsigned int i=0; i < temp_face_indices.size(); i++) {
            std::vector<VertexIndices>::iterator result;
            unsigned int v[3];

            for(int j=0; j<3; j++) {
                result = std::find(unique_vertices.begin(),
                              unique_vertices.end(),
                              temp_face_indices[i].v[j]);

                // if this combination of indicies doesn't exist,
                // add the data to the attribute arrays
                if (result == unique_vertices.end()) {
                    v[j] = this->verts.size();

                    this->verts.push_back(temp_verts[temp_face_indices[i].v[j].vertex]);
					if (temp_uvs.size() > 0) {
						this->texCoords.push_back(temp_uvs[temp_face_indices[i].v[j].uv]);
					}
					if (temp_normals.size() > 0) {
						this->vertNorms.push_back(temp_normals[temp_face_indices[i].v[j].normal]);
					}
					if (temp_colors.size() > 0) {
						this->colors.push_back(temp_colors[temp_face_indices[i].v[j].color]);
					}
                    unique_vertices.push_back(temp_face_indices[i].v[j]);
                }
                else {
                    // calculate the correct offset
                    unsigned int unique_vertex_index = std::distance(unique_vertices.begin(), result);
                    v[j] = unique_vertex_index;
                }
            }

            // Push it back
            this->faces.push_back(Face(v[0], v[1], v[2]));
        }

        // Check if vertex normals exist
        if(vertNorms.size() == 0) {
            std::vector<Vertex> surfaceNorms;

            // compute surface normals
            for(size_t i = 0; i < faces.size(); i++) {
                glm::vec3 vector1, vector2, cross, normal;
                Vertex vert1(verts[faces[i].v1]), vert2(verts[faces[i].v2]), vert3(verts[faces[i].v3]);

                vector1 = glm::normalize(glm::vec3(vert2.x-vert1.x, vert2.y-vert1.y, vert2.z-vert1.z));
                vector2 = glm::normalize(glm::vec3(vert3.x-vert1.x, vert3.y-vert1.y, vert3.z-vert1.z));
                cross = glm::cross(vector1, vector2);
                normal = glm::normalize(cross);

                surfaceNorms.push_back(Vertex(normal.x, normal.y, normal.z));
            }

            // compute vertex normals
            // should probably compute adjacency first, this could be slow
            for(size_t i = 0; i < verts.size(); i++) {
                Vertex total_normals(0.0f, 0.0f, 0.0f);

                for(size_t j = 0; j < faces.size(); j++) {
                    if (faces[j].v1 == i || faces[j].v2 == i || faces[j].v3 == i) {
                        total_normals.x += surfaceNorms[j].x;
                        total_normals.y += surfaceNorms[j].y;
                        total_normals.z += surfaceNorms[j].z;
                    }
                }

                if(!(total_normals.x == 0.0f && total_normals.y == 0.0f && total_normals.z == 0.0f)) {
                    glm::vec3 final_normal(total_normals.x, total_normals.y, total_normals.z);
                    final_normal = glm::normalize(final_normal);
                    vertNorms.push_back(Vertex(final_normal.x, final_normal.y, final_normal.z));
                }
                else {
                    vertNorms.push_back(Vertex(total_normals.x, total_normals.y, total_normals.z));
                }
            }

			surfaceNorms.clear();
		}
		return true;
    } // function LoadMesh

    void GLMesh::LoadShader() {
       IGLComponent::LoadShader(GLMesh::DEFAULT_SHADER);
    }

    void GLMesh::ParseMTL(std::string fname) {
		// Extract the path from the filename.
		std::string path;
		if (fname.find("/") != std::string::npos) {
			path = fname.substr(0, fname.find_last_of("/") + 1); // Keep the separator.
		}
		else {
			path = fname.substr(0, fname.find_last_of("\\") + 1); // Keep the separator.
		}

		std::ifstream in(fname, std::ios::in);

        if (!in) {
            std::cerr << "Cannot open material " << fname << std::endl;
            return;
        }

        std::string line;
        while (getline(in, line)) {
            line = trim(line);
            std::stringstream s(line);
            std::string label;
            s >> label;
            if (label == "newmtl") {
                std::string name;
                s >> name;
                Material m;
                getline(in, line);
				s.clear();
                s.str(line);
                s.seekg(0);
                s >> label;
                while (label != "newmtl") {
                    if (label == "Ka") {
                        float r,g,b;
                        s >> r; s >> g; s >> b;
                        m.ka[0] = r; m.ka[1] = g; m.ka[2] = b;
                    }
					else if (label == "Kd") {
                        float r,g,b;
                        s >> r; s >> g; s >> b;
                        m.kd[0] = r; m.kd[1] = g; m.kd[2] = b;
                    }
					else if (label == "Ks") {
                        float r,g,b;
                        s >> r; s >> g; s >> b;
                        m.ks[0] = r; m.ks[1] = g; m.ks[2] = b;
                    }
					else if ((label == "Tr") || (label == "d")) {
                        float tr;
                        s >> tr;
                        m.tr = tr;
                    }
					else if (label == "Ns") {
                        float ns;
                        s >> ns;
                        m.tr = ns;
                    }
					else if (label == "illum") {
                        int i;
                        s >> i;
                        m.illum = i;
                    }
					else if (label == "map_Kd") {
                        std::string filename;
						s >> filename;
						filename = trim(filename);
						filename = convert_path(filename);
						std::cerr << "Loading diffuse texture: " << path + filename << std::endl;
						resource::GLTexture texture;
						if (OpenGLSystem::textures.find(filename) == OpenGLSystem::textures.end()) {
							texture.LoadDataFromFile(path + filename);
							if (texture.GetID() != 0) {
								OpenGLSystem::textures[filename] = texture;
							}
						}

						// Add the path to the filename to load it relative to the mtl file
						if (OpenGLSystem::textures.find(filename) != OpenGLSystem::textures.end()) {
							m.diffuseMap = Sigma::OpenGLSystem::textures[filename].GetID();
						}

						if (m.diffuseMap == 0) {
							std::cerr << "Error loading diffuse texture: " << path + filename << std::endl;
						}
                    }
					else if (label == "map_Ka") {
                        std::string filename;
						s >> filename;
						filename = trim(filename);
						filename = convert_path(filename);
						std::cerr << "Loading ambient texture: " << path + filename << std::endl;
						// Add the path to the filename to load it relative to the mtl file
						resource::GLTexture texture;
						if (OpenGLSystem::textures.find(filename) == OpenGLSystem::textures.end()) {
							texture.LoadDataFromFile(path + filename);
							if (texture.GetID() != 0) {
								OpenGLSystem::textures[filename] = texture;
							}
						}

						// It should be loaded, but in case an error occurred double check for it.
						if (OpenGLSystem::textures.find(filename) != OpenGLSystem::textures.end()) {
							m.ambientMap = Sigma::OpenGLSystem::textures[filename].GetID();
						}

						// Add the path to the filename to load it relative to the mtl file
						if (m.ambientMap == 0) {
							std::cerr << "Error loading ambient texture: " << path + filename << std::endl;
						}
                    }
					else if (label == "map_Bump") {
                        std::string filename;
						s >> filename;
						filename = trim(filename);
						filename = convert_path(filename);
						std::cerr << "Loading normal or bump texture: " << path + filename << std::endl;
						// Add the path to the filename to load it relative to the mtl file
						resource::GLTexture texture;
						if (OpenGLSystem::textures.find(filename) == OpenGLSystem::textures.end()) {
							texture.LoadDataFromFile(path + filename);
							if (texture.GetID() != 0) {
								OpenGLSystem::textures[filename] = texture;
							}
						}

						// It should be loaded, but in case an error occurred double check for it.
						if (OpenGLSystem::textures.find(filename) != OpenGLSystem::textures.end()) {
							m.normalMap = Sigma::OpenGLSystem::textures[filename].GetID();
						}

						// Add the path to the filename to load it relative to the mtl file
						if (m.normalMap == 0) {
							std::cerr << "Error loading normal texture: " << path + filename << std::endl;
						}
                    }
					else {
                        // Blank line
                    }
                    std::streamoff pre = in.tellg();
                    getline(in, line);
                    if (in.eof()) {
                        break;
                    }
					s.clear();
                    s.str(line);
                    s.seekg(0);
                    s >> label;
                    std::string newlabel;
                    if (s.str().find("newmtl") != std::string::npos) {
                        in.seekg(pre);
                        break;
                    }
                }
                this->mats[name] = m;
            }
        }
    } // function ParseMTL

} // namespace Sigma
