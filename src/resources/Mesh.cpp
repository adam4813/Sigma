#include "resources/Mesh.h"

#include "strutils.h"
#include "systems/OpenGLSystem.h"

#include <algorithm>
#include <stdexcept>

#include <fstream>
#include <iostream>
#include <sstream>

namespace Sigma{
	Mesh::Mesh() { }

	Mesh::~Mesh() { }

	unsigned int Mesh::MeshGroup_ElementCount(const unsigned int group /*= 0*/) const {
		if (this->materialGroupIndex.size() == 0) {
			return 0;
		}

		if ((group + 1) < (this->materialGroupIndex.size())) {
			return (this->materialGroupIndex[group+1] - this->materialGroupIndex[group]) * 3;
		}
		else if (group > (this->materialGroupIndex.size() - 1)) {
			return 0;
		}
		else {
			return (this->faces.size() - this->materialGroupIndex[group]) * 3;
		}
	}

	bool operator ==(const VertexIndices &lhs, const VertexIndices &rhs) {
		return (lhs.vertex==rhs.vertex &&
				lhs.normal==rhs.normal &&
				lhs.uv==rhs.uv &&
				lhs.color==rhs.color);
	}

	bool Mesh::LoadObjMesh(std::string fname) {
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
				this->materialGroupIndex.push_back(temp_face_indices.size());
			}
			else if (line.substr(0, line.find(' ')) == "mtllib") { // Material library
				// Add the path to the filename to load it relative to the obj file.
				LoadMTL(path + line.substr(line.find(' ') + 1));
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
				this->materialGroups[temp_face_indices.size()] = currentMtlGroup;
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

	void Mesh::LoadMTL(std::string fname) {
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
						m.hardness = ns;
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

	void Mesh::AddVertex(const Vertex& v) {
		this->verts.push_back(v);
	}

	const Vertex* Mesh::GetVertex(const unsigned int index) const {
		if(index < this->verts.size()) {
			return &this->verts[index];
		}
		return nullptr;
	}

	const size_t Mesh::VertexCount() const {
		return this->verts.size();
	}

	std::vector<Vertex>::reference Mesh::GetVertexBuffer() {
		return this->verts.front();
	}

	void Mesh::AddFace(const Face& f) {
		this->faces.push_back(f);
	}

	const Face* Mesh::GetFace(const size_t index) const {
		if(index < this->faces.size()) {
			return &this->faces[index];
		}
		return nullptr;
	}

	/*bool Mesh::RemoveFace(const unsigned int index) {
		if(index < this->faces.size()) {
			this->faces.erase(this->faces.begin() + index);
			return true;
		}
		return false;
	}*/

	const size_t Mesh::FaceCount() const {
		return this->faces.size();
	}

	std::vector<Face>::reference Mesh::GetFaceBuffer() {
		return this->faces.front();
	}

	void Mesh::AddVertexNormal(const Vertex& vn) {
		this->vertNorms.push_back(vn);
	}

	const Sigma::Vertex* Mesh::GetVertexNormal(const size_t index) {
		if (index < this->vertNorms.size()) {
			return &this->vertNorms[index];
		}
		return nullptr;
	}

	const size_t Mesh::VertexNormalCount() const {
		return this->vertNorms.size();
	}

	std::vector<Vertex>::reference Mesh::GetVertexNormalBuffer() {
		return this->vertNorms.front();
	}

	void Mesh::AddVertexColor(const Color& c) {
		this->colors.push_back(c);
	}

	const Color* Mesh::GetVertexColor(const size_t index) const {
		if (index < this->colors.size()) {
			return &this->colors[index];
		}
		return nullptr;
	}

	const size_t Mesh::VertexColorCount() const {
		return this->colors.size();
	}

	std::vector<Color>::reference Mesh::GetVertexColorBuffer() {
		return this->colors.front();
	}

	void Mesh::AddTexCoord(const TexCoord& t) {
		this->texCoords.push_back(t);
	}

	const TexCoord* Mesh::GetTexCoord(const size_t index) {
		if (index < this->texCoords.size()) {
			return &this->texCoords[index];
		}
		return nullptr;
	}

	const size_t Mesh::GetTexCoordCount() const {
		return this->texCoords.size();
	}

	std::vector<TexCoord>::reference Mesh::GetTexCoordBuffer() {
		return this->texCoords.front();
	}

	void Mesh::AddMaterialGroupIndex(const size_t index) {
		this->materialGroupIndex.push_back(index);
	}

	const std::string* Mesh::GetMaterialGroupName(const size_t groupIndex) {
		if (this->materialGroups.find(groupIndex) != this->materialGroups.end()) {
			return &this->materialGroups[groupIndex];
		}
		return nullptr;
	}

	const size_t Mesh::MaterialGroupsCount() {
		return this->materialGroups.size();
	}

	const Material* Mesh::GetMaterialGroup(const std::string& name) {
		if (this->mats.find(name) != this->mats.end()) {
			return &this->mats[name];
		}
		return nullptr;
	}

	Material::Material() {
		ka[0] = 1.0f; ka[1] = 1.0f; ka[2] = 1.0f;
		kd[0] = 1.0f; kd[1] = 1.0f; kd[2] = 1.0f;
		ks[0] = 1.0f; ks[1] = 1.0f; ks[2] = 1.0f;
		tr = 1.0f;
		hardness = 64.0f;
		illum = 1;
		this->ambientMap = 0;
		this->diffuseMap = 0;
		this->specularMap = 0;
		this->normalMap = 0;
		this->cubeMap = 0;
		this->cubeNormalMap = 0;
	}

} // namespace Sigma
