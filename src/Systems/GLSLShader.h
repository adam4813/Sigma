//A simple class for handling GLSL shader compilation
//Auhtor: Movania Muhammad Mobeen
//Last Modified: February 2, 2011

#pragma once
#include "GL/glew.h"
#include <map>
#include <string>

class GLSLShader
{
public:
	GLSLShader(void);
	~GLSLShader(void);
	void LoadFromString(GLenum whichShader, const std::string source);
	void LoadFromFile(GLenum whichShader, const std::string filename);
	void CreateAndLinkProgram();
	void Use();
	void UnUse();
	void AddAttribute(const std::string attribute);
	void AddUniform(const std::string uniform);
	GLuint GetProgram() const;
	//An indexer that returns the location of the attribute/uniform
	GLuint operator[](const std::string attribute);
	GLuint operator()(const std::string uniform);
	//Program deletion
	void DeleteProgram() {glDeleteProgram(_program);_program=-1;}
private:
	enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
	GLuint	_program;
	int _totalShaders;
	GLuint _shaders[3];//0->vertexshader, 1->fragmentshader, 2->geometryshader
	std::map<std::string,GLuint> _attributeList;
	std::map<std::string,GLuint> _uniformLocationList;
};
