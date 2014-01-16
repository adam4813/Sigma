#include "IGLComponent.h"

namespace Sigma{
	// static member initialization
    Renderable::ShaderMap Renderable::loadedShaders;

    void Renderable::LoadShader(const std::string& filename) {
        // look up shader that is already loaded
        ShaderMap::iterator existingShader = Renderable::loadedShaders.find(filename.c_str());
        if(existingShader != Renderable::loadedShaders.end()) {
            // shader already exists!
			this->shader = existingShader->second;
        }
        else {
            // need to create and save the shader
            std::string vertFilename = filename + ".vert";
            std::string fragFilename = filename + ".frag";

            GLSLShader* theShader = new GLSLShader();
            theShader->LoadFromFile(GL_VERTEX_SHADER, vertFilename);
            theShader->LoadFromFile(GL_FRAGMENT_SHADER, fragFilename);
            theShader->CreateAndLinkProgram();

			if (theShader->isLoaded()) {
				// assign it to this instance
				this->shader = std::shared_ptr<GLSLShader>(theShader);
				// save it in the static map
				Renderable::loadedShaders[filename] = this->shader;
			}
        }
    }

} // namespace Sigma
