#ifndef shader_hpp
#define shader_hpp
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{

public:
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
	{
		std::string v;
		std::string f;
		std::ifstream vs;
		std::ifstream fs;
		vs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vs.open(vertexShaderPath);
			fs.open(fragmentShaderPath);
			std::stringstream vss, fss;
			vss << vs.rdbuf();
			fss << fs.rdbuf();
			vs.close();
			fs.close();
			v = vss.str();
			f = fss.str();
		}
		catch (const std::exception& e)
		{
			std::cout << "[Error] Failed to load shader file.";
		}
		int status;
		unsigned int vsi = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fsi = glCreateShader(GL_FRAGMENT_SHADER);
		const char* vsc = v.c_str(), *fsc = f.c_str();
		glShaderSource(vsi, 1, &vsc, NULL);
		glCompileShader(vsi);
		glGetShaderiv(vsi, GL_COMPILE_STATUS, &status);
		if (!status) {
			std::cout << "[Error] Failed to compile vertex shader.";
		}

		glShaderSource(fsi, 1, &fsc, NULL);
		glCompileShader(fsi);
		glGetShaderiv(fsi, GL_COMPILE_STATUS, &status);
		if (!status) {
			std::cout << "[Error] Failed to compile fragment shader.";
		}

		shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, vsi);
		glAttachShader(shaderProgramID, fsi);
		glLinkProgram(shaderProgramID);
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &status);
		if (!status) {
			std::cout << "[Error] Failed to link shader program.";
		}
		glDeleteShader(vsi);
		glDeleteShader(fsi);
	}

	~Shader()
	{
		glDeleteProgram(shaderProgramID);
	}

	void use() const
	{
		glUseProgram(shaderProgramID);
	}

	void setUf(char* name, float value) const 
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, name), (GLfloat)value);
	}

	void setUm4f(const char* name, const float* value, unsigned int transpose = GL_FALSE) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name), 1, transpose, value);
	}

private:
	unsigned int shaderProgramID;
};


#endif