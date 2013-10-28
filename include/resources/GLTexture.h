#pragma once
#include "GL/glew.h"
#include <string>
<<<<<<< HEAD
#include "SOIL/SOIL.h"
=======
#include "SOIL.h"
>>>>>>> Added the GLTexture resource

namespace Sigma {
	namespace resource {
		class GLTexture {
		public:
			GLTexture() : id(0) { }

			unsigned int GenerateGLTexutre(unsigned int width, unsigned int height) {
				glGenTextures(1, &this->id);
				this->width = width;
				this->height = height;
			}
			void LoadDataFromMemory(const unsigned char* data, unsigned int width, unsigned int height) {
				if (id == 0) {
					glGenTextures(1, &this->id);
				}
				this->width = width;
				this->height = height;
				glBindTexture(GL_TEXTURE_2D, this->id);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			void LoadDataFromFile(const std::string& filename) {
<<<<<<< HEAD
				int width, height, channels;
				unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, &channels, false);

				if (data) {
					// Invert Y
					int i, j;
					for( j = 0; j*2 < height; ++j )
					{
						int index1 = j * width * channels;
						int index2 = (height - 1 - j) * width * channels;
						for( i = width * channels; i > 0; --i )
						{
							unsigned char temp = data[index1];
							data[index1] = data[index2];
							data[index2] = temp;
							++index1;
							++index2;
						}
					}

=======
				int width, height;
				unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, nullptr, false);
				if (data) {
>>>>>>> Added the GLTexture resource
					if (width >= 0) {
						this->width = width;
					}
					if (height >= 0) {
						this->height = height;
					}
					LoadDataFromMemory(data, width, height);
					delete data;
				}
			}
			unsigned int GetID() const { return id; }
		private:
			unsigned int id;
			unsigned int width;
			unsigned int height;
		};
	}
}
