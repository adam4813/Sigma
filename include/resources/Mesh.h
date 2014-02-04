#pragma once
#ifndef GLMESH_H
#define GLMESH_H

#ifndef __APPLE__
#include "GL/glew.h"
#endif

#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Sigma{
	namespace resource {
		// A struct to store which index each of its verts are.
		struct Face {
			Face(unsigned int v1, unsigned int v2, unsigned int v3) : v1(v1), v2(v2), v3(v3) { }
			unsigned int v1, v2, v3;
		};

		// A struct to store the location for each vertex.
		struct Vertex {
			Vertex(float x, float y, float z) : x(x), y(y), z(z) { }
			float x,y,z;
		};

		// A struct to store the color value for each vertex
		struct Color {
			Color(float r, float g, float b) : r(r), g(g), b(b) { }
			float r,g,b;
		};

		// A struct to store 2D or 1D texture coordinates for each vertex
		struct TexCoord {
			TexCoord(float u, float v = 0.0f) : u(u), v(v) { }
			float u, v;
		};

		// A struct to store material information for each vertex.
		// Wikipedia explains what's going on:
		// http://en.wikipedia.org/wiki/Wavefront_.obj_file#Basic_materials
		struct Material {
			Material();
			float ka[3];
			float kd[3];
			float ks[3];
			float tr; // Aka d
			float hardness;
			int illum;
			// TODO: Add maps
			GLuint ambientMap;
			GLuint diffuseMap;
			GLuint specularMap;
			GLuint normalMap;
			GLuint cubeMap;
			GLuint cubeNormalMap;
		};

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

		class Mesh {
		public:
			Mesh();
			virtual ~Mesh();

			static unsigned int const TypeID = 1000;

			/**
			 * \brief Returns the number of elements to draw for this component.
			 *
			 * Call this multiple times to get the elements for each mesh group. A return of 0 indicates all
			 * mesh groups have been returned, and the call loop should end.
			 * \param group The mesh group to render.
			 * \return unsigned int The number of elements to draw for the given mesh group.
			 */
			unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const;

			/**
			 * \brief Loads and parses an OBJ mesh file.
			 *
			 * Automatically loads and parses MTL files as well.
			 * \param[in] std::string fname The filename of the OBJ file to load and parse.
			 * \return bool True if the mesh file was loaded and parsed successfully.
			 */
			bool Load(std::string fname);

			/**
			 * \brief Loads and parses a MTL file.
			 *
			 * \param[in] std::string fname The filename of the MTL file to load and parse.
			 * \return void 
			 * \exception  
			 */
			void LoadMTL(std::string fname);

			/**
			 * \brief Add a vertex to the list.
			 *
			 * \param const Vertex v The vertex to add. It is copied.
			 */
			void AddVertex(const Vertex& v);

			/**
			 * \brief Gets a vertex.
			 *
			 * Returns the vertex at the specific index.
			 * \param const size_t index The index of the vertex to get.
			 * \return const Vertex* The vertex at the index or nullptr if the index was invalid.
			 */
			const Vertex* GetVertex(const size_t index) const;

			/**
			 * \brief Returns the size of the vertex buffer.
			 *
			 * \return const size_t The size of the vertex buffer.
			 */
			const size_t VertexCount() const;

			/**
			 * \brief Gets a reference to the start of the vertex buffer.
			 *
			 * This can be used for large operations where you need to iterate over all of the vertices in the buffer.
			 * Use this in conjunction with the count method.
			 * \return std::vector<Vertex>::reference A reference to the start of the vertex buffer.
			 */
			std::vector<Vertex>::reference GetVertexBuffer();

			/**
			 * \brief Add a face to the list.
			 *
			 * \param const Face f The face to add. It is copied.
			 */
			void AddFace(const Face& f);

			/**
			 * \brief Gets a face.
			 *
			 * Returns the face at the specific index.
			 * \param const size_t index The index of the face to get.
			 * \return const Face* The face at the index or nullptr if the index was invalid.
			 */
			const Face* GetFace(const size_t index) const;

			//bool RemoveFace(const unsigned int index);

			/**
			 * \brief Returns the size of the face buffer.
			 *
			 * \return const size_t The size of the face buffer.
			 */
			const size_t FaceCount() const;
		
			/**
			 * \brief Gets a reference to the start of the face buffer.
			 *
			 * This can be used for large operations where you need to iterate over all of the faces in the buffer.
			 * Use this in conjunction with the count method.
			 * \return std::vector<Vertex>::reference A reference to the start of the face buffer.
			 */
			std::vector<Face>::reference GetFaceBuffer();

			/**
			 * \brief Add a vertex normal to the list.
			 *
			 * \param v The vertex normal to add. It is copied.
			 */
			void AddVertexNormal(const Vertex& vn);
		
			/**
			 * \brief Gets a vertex normal.
			 *
			 * Returns the vertex normal at the specific index.
			 * \param const size_t index The index of the face to get.
			 * \return const Vertex* The vertex normal at the index or nullptr if the index was invalid.
			 */
			const Vertex* GetVertexNormal( const size_t index );

			/**
			 * \brief Returns the size of the vertex normal buffer.
			 *
			 * \return const size_t The size of the vertex normal buffer.
			 */
			const size_t VertexNormalCount() const;

			/**
			 * \brief Gets a reference to the start of the vertex normal buffer.
			 *
			 * This can be used for large operations where you need to iterate over all of the vertex normal in the buffer.
			 * Use this in conjunction with the count method.
			 * \return std::vector<Vertex>::reference A reference to the start of the vertex normal buffer.
			 */
			std::vector<Vertex>::reference GetVertexNormalBuffer();

			/**
			 * \brief Add a vertex color to the list.
			 *
			 * \param const Color& c The vertex color to add. It is copied.
			 */
			void AddVertexColor(const Color& c);

			/**
			 * \brief Gets a vertex color.
			 *
			 * Returns the vertex color at the specific index.
			 * \param const size_t index The index of the color to get.
			 * \return const Color* The color at the index or nullptr if the index was invalid.
			 */
			const Color* GetVertexColor(const size_t index) const;

			/**
			 * \brief Returns the size of the vertex color buffer.
			 *
			 * \return const size_t The size of the vertex color buffer.
			 */
			const size_t VertexColorCount() const;

			/**
			 * \brief Gets a reference to the start of the vertex color buffer.
			 *
			 * This can be used for large operations where you need to iterate over all of the vertex color in the buffer.
			 * Use this in conjunction with the count method.
			 * \return std::vector<Vertex>::reference A reference to the start of the vertex color buffer.
			 */
			std::vector<Color>::reference GetVertexColorBuffer();
		
			/**
			 * \brief Add a texture coordinate to the list.
			 *
			 * \param const TexCoord& c The texture coordinate to add. It is copied.
			 */
			void AddTexCoord(const TexCoord& t);

			/**
			 * \brief Gets a texture coordinate.
			 *
			 * Returns the texture coordinate at the specific index.
			 * \param const size_t index The index of the texture coordinate to get.
			 * \return const TexCoord* The texture coordinate at the index or nullptr if the index was invalid.
			 */
			const TexCoord* GetTexCoord(const size_t index);

			/**
			 * \brief Returns the size of the vertex color buffer.
			 *
			 * \return const size_t The size of the vertex color buffer.
			 */
			const size_t GetTexCoordCount() const;

			/**
			 * \brief Gets a reference to the start of the texture coordinate buffer.
			 *
			 * This can be used for large operations where you need to iterate over all of the  texture coordinate in the buffer.
			 * Use this in conjunction with the count method.
			 * \return std::vector<Vertex>::reference A reference to the start of the  texture coordinate buffer.
			 */
			std::vector<TexCoord>::reference GetTexCoordBuffer();

			/**
			 * \brief Gets a material with the given name.
			 *
			 * \param[in] const std::string & name The name of the material to find
			 * \return const Material* The material with the specified name.
			 */
			const Material* GetMaterialGroup(const std::string& name);

			/**
			 * \brief Adds a mesh group index.
			 *
			 * Adds a material group starting index to materialGroupIndex. This is the starting face index for the material group.
			 * \param const size_t index the index of the new material group
			 */
			void AddMaterialGroupIndex(const size_t index);

			/**
			 * \brief Gets the material name for the specified material group index.
			 *
			 * \param[in] const size_t groupIndex
			 * \return const std::string* The name of the material used for the specified group.
			 */
			const std::string* GetMaterialGroupName(const size_t groupIndex);

			/**
			 * \brief Returns the number of material groups.
			 *
			 * \return const size_t The number of material groups.
			 */
			const size_t MaterialGroupsCount();
		protected:
			// Note that these values are protected, not private! Inheriting classes get access to these
			//  basic drawing elements.
			std::vector<Face> faces; // Stores vectors of face groupings.
			std::vector<Vertex> verts; // The verts that the faces refers to. Can be used for later refinement.
			std::vector<Vertex> vertNorms;  // The vertex normals for each vertex. Note that by some sleight of hand,
											// we are using a vertex as a vector, since both are just 3 floats..
			std::vector<TexCoord> texCoords; // The texture coordinates for each vertex.
			std::vector<Color> colors;
			std::map<std::string, Material> mats; // Loaded materials mapped to material name.
			std::map<unsigned int, std::string> materialGroups; // Stores a mapping of material name to material (face) grouping.
			std::vector<unsigned int> materialGroupIndex; // Stores which index each material (face) group starts at.
		}; // class Mesh
	}
} // namespace Sigma

#endif // GLMESH_H
