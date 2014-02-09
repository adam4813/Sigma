#pragma once
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include "GL/glew.h"
#endif

#include <string>
#include <cassert>
#include <vector>

#include "SOIL/SOIL.h"
#include "systems/ResourceSsytem.h"
#include "Property.h"

namespace Sigma {
	namespace resource {
		/**
		 * Represents a OpenGL Texture
		 */
		class Texture {
		public:
			Texture() : id(0), autogen_mipmaps(true) { 
				glGenTextures(1, &this->id);
				int_format = GL_RGBA8;           /// Internal format in the GPU

				// Note: Preferred format of the GPU could be get using ARB_internalformat_query2 extension  :
				// glGetInternalFormativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &preferred_format)
				// But using this could required adapting the Texture to this format
				format     = GL_RGBA;            /// Pixel data format (format of the loaded data)
				type       = GL_UNSIGNED_BYTE;   /// Data Type per pixel (float, byte, etc)

				wrap_s      = GL_REPEAT;          /// Sets the wrap parameter for texture coordinate s 
				wrap_t      = GL_REPEAT;          /// Sets the wrap parameter for texture
				wrap_r      = GL_REPEAT;          /// Sets the wrap parameter for texture

				mag_filter  = GL_LINEAR;          /// Filter using when magnify texture
				min_filter  = GL_LINEAR_MIPMAP_LINEAR;          /// Filter using when minimize texture

			}

			/**
			 * Generates a empty texture of the desired size
			 */
			void GenerateGLTexture(unsigned int width, unsigned int height) {
				if (this->id > 0) {
					glDeleteTextures(1, &this->id);
				}
				glGenTextures(1, &this->id);
				this->width = width;
				this->height = height;

				glBindTexture(GL_TEXTURE_2D, this->id);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mag_filter);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->min_filter);
				glTexImage2D(GL_TEXTURE_2D, 0, this->int_format, this->width, this->height, 0, this->format, GL_UNSIGNED_BYTE, NULL);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			/**
			 * Loads and create a texture from a bitmap in RAM
			 * \param data Ptr. to the bitmap
			 * \param width
			 * \param height
			 */
			void LoadDataFromMemory(const unsigned char* data, unsigned int width, unsigned int height) {
				this->width = width;
				this->height = height;

				glBindTexture(GL_TEXTURE_2D, this->id);

				glTexImage2D(GL_TEXTURE_2D, 0, this->int_format , this->width, this->height, 0, this->format, this->type, data);

				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, this->wrap_r);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);

				if (this->autogen_mipmaps) {
					glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
				}

				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mag_filter);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->min_filter);

				glBindTexture(GL_TEXTURE_2D, 0);
			}

			/**
			 * \brief Updates a texture data from a bitmap in RAM.
			 * REQUIRES a previous call of Load or LoadDataFromMemory
			 * The bitmap MUST have the same size that the texture.
			 * \param data Ptr. to the bitmap
			 */
			void UpdateDataFromMemory(const unsigned char* data) {
				if (id != 0) {
					assert (this->width > 0 && this->height > 0);

					glBindTexture(GL_TEXTURE_2D, this->id);
					glTexSubImage2D(	GL_TEXTURE_2D, 0,
										0, 0, this->width, this->height, 
										this->format, this->type, data);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

			/**
			 * Loads and create a texture from a image file
			 * \param filename Path to the image file
			 * \param options Struct that defines the format of the bitmap and how the GPU will interpret it
			 */
			bool Load(const std::string& filename) {
				int width, height, channels;
				unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, &channels, false);

				if (data) {
					// Invert Y (necessary!)
					int i, j;
					for( j = 0; j*2 < height; ++j ) {
						int index1 = j * width * channels;
						int index2 = (height - 1 - j) * width * channels;
						for( i = width * channels; i > 0; --i ) {
							unsigned char temp = data[index1];
							data[index1] = data[index2];
							data[index2] = temp;
							++index1;
							++index2;
						}
					}

					if (width >= 0) {
						this->width = width;
					}
					if (height >= 0) {
						this->height = height;
					}
					LoadDataFromMemory(data, width, height);
					delete data;
				}

				if (id == 0) {
					return false;
				}
				return true;
			}

			virtual bool Create(const std::vector<Property> &properties) {
				std::string fname = "";
				std::string format = "GL_RGBA";
				std::string wrapR = "GL_REPEAT", wrapS = "GL_REPEAT", wrapT = "GL_REPEAT";
				bool inMemory = false;
				bool autogenMipMap = true;

				for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
					const Property*  p = &(*propitr);
					if (p->GetName() == "filename") {
						fname = p->Get<std::string>();
					}
					else if (p->GetName() == "inMemory") {
						inMemory = p->Get<bool>();
					}
					else if (p->GetName() == "width") {
						this->width = p->Get<int>();
					}
					else if (p->GetName() == "height") {
						this->height = p->Get<int>();
					}
					else if (p->GetName() == "format") {
						format =  p->Get<std::string>();
					}
					else if (p->GetName() == "wrap_r") {
						wrapR = p->Get<std::string>();
					}
					else if (p->GetName() == "wrap_S") {
						wrapS = p->Get<std::string>();
					}
					else if (p->GetName() == "wrap_t") {
						wrapT = p->Get<std::string>();
					}
					else if (p->GetName() == "autogen_mipmap") {
						autogenMipMap = p->Get<bool>();
					}
				}
				GenerateGLTexture(this->width, this->height);
				Format(FormatValue(format));
				WrapR(WrapValue(wrapR));
				WrapS(WrapValue(wrapS));
				WrapT(WrapValue(wrapT));
				AutoGenMipMaps(autogenMipMap);

				if (inMemory) {
					return this->id > 0;
				}
				return Load(fname);
			}

			unsigned int GetID() const { return id; }

			/**
			 * Return OpenGL GPU internal format of the Texture
			 */
			GLenum IntFormat() const { return int_format; }

			/**
			 * Sets OpenGL GPU internal format of the Texture
			 */
			void IntFormat(GLenum val)  { int_format = val; }
			
			/**
			 * Changes the format value from string to GLint.
			 */
			GLint FormatValue(const std::string& val) {
				if (val == "GL_RGBA") {
					return GL_RGBA;
				}
				else if (val == "GL_BGRA") {
					return GL_BGRA;
				}
				else if (val == "GL_RED") {
					return GL_RED;
				}
				else if (val == "GL_RG") {
					return GL_RG;
				}
				else if (val == "GL_RGB") {
					return GL_RGB;
				}
				else if (val == "GL_BGR") {
					return GL_BGR;
				}
				else if (val == "GL_DEPTH_COMPONENT") {
					return GL_DEPTH_COMPONENT;
				}
				else if (val == "GL_STENCIL_INDEX") {
					return GL_STENCIL_INDEX;
				}
			}

			/**
			 * Return loaded Pixel data format
			 */
			GLenum Format() const { return format; }

			/**
			 * Sets loaded Pixel data format
			 */
			void Format(GLenum val)  { format = val; }

			/**
			 * Return Data Type per Pixel (float, byte, etc...)
			 */
			GLenum Type() const { return type; }

			/**
			 * Sets Data Type per Pixel (float, byte, etc...)
			 */
			void Type(GLenum val)  { type = val; }

			/**
			 * Changes the wrap value from string to GLint.
			 */
			GLint WrapValue(const std::string& val) {
				if (val == "GL_CLAMP_TO_EDGE") {
					return GL_CLAMP_TO_EDGE;
				}
				else if (val == "GL_CLAMP_TO_BORDER") {
					return GL_CLAMP_TO_BORDER;
				}
				else if (val == "GL_MIRRORED_REPEAT") {
					return GL_MIRRORED_REPEAT;
				}
				else if (val == "GL_REPEAT") {
					return GL_REPEAT;
				}
				else if (val == "GL_MIRROR_CLAMP_TO_EDGE") {
					return GL_MIRROR_CLAMP_TO_EDGE;
				}
			}

			GLint WrapS() const { return wrap_s; }

			void WrapS(GLint val) {
				wrap_s = val;
				if (id != 0) {
					glBindTexture(GL_TEXTURE_2D, this->id);
					glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

			GLint WrapT() const { return wrap_t; }

			void WrapT(GLint val) {
				wrap_t = val;
				if (id != 0) {
					glBindTexture(GL_TEXTURE_2D, this->id);
					glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

			GLint WrapR() const { return wrap_r; }

			void WrapR(GLint val) {
				wrap_r = val;
				if (id != 0) {
					glBindTexture(GL_TEXTURE_2D, this->id);
					glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap_r);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

			bool AutoGenMipMaps() const { return autogen_mipmaps; }

			/**
			 * Should autogenerate mipmaps for the texture
			 * WARING: Is a costly task!
			 */
			void AutoGenMipMaps(bool val) {
				autogen_mipmaps = val;
			}

			GLint MagFilter() const { return mag_filter; }

			void MagFilter(GLint val) {
				mag_filter = val; 
				if (id != 0) {
					glBindTexture(GL_TEXTURE_2D, this->id);
					glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

			GLint MinFilter() const { return min_filter; }

			void MinFilter(GLint val) {
				min_filter = val;
				if (id != 0) {
					glBindTexture(GL_TEXTURE_2D, this->id);
					glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

		private:
			unsigned int id;
			unsigned int width;
			unsigned int height;

			GLenum int_format;    /// Internal format in the GPU
			GLenum format;        /// Pixel data format (format of the loaded data)
			GLenum type;          /// Data Type per pixel (float, byte, etc)

			GLint wrap_s;         /// Sets the wrap parameter for texture coordinate s 
			GLint wrap_t;         /// Sets the wrap parameter for texture
			GLint wrap_r;         /// Sets the wrap parameter for texture

			bool autogen_mipmaps; /// Should autogenerate mipmaps ?

			GLint mag_filter;     /// Filter using when magnify texture
			GLint min_filter;     /// Filter using when minimize texture
		};

		template <> inline const char* GetTypeName<Texture>() { return "Texture"; }
		template <> inline const unsigned int GetTypeID<Texture>() { return 1001; }
	}
}
