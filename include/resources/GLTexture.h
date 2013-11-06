#pragma once
#include "GL/glew.h"
#include <string>
#include "SOIL/SOIL.h"

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
				int width, height;
				unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, nullptr, false);
				if (data) {
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
