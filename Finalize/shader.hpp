#ifndef shader_hpp
#define shader_hpp
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* fragment = "#version 330\n"
"in vec4 vertexColor;\n"
"in vec3 normal;\n"
"in vec3 fragPosition;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
	"vec3 ambient = 0.25f * lightColor;\n"
	"vec3 objColor = vec3(vertexColor.x, vertexColor.y, vertexColor.z);\n"
	"vec3 lightDirection = normalize(lightPos - fragPosition);\n"
	"vec3 norm = normalize(normal);\n"
	"vec3 reflectDir = reflect(-lightDirection, norm);\n"
	"vec3 viewDir = normalize(viewPos - fragPosition);\n"
"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);\n"
"	vec3 specular = .35f * spec * lightColor;\n"
"	float diff = max(dot(norm, lightDirection), 0.0);\n"
"	vec3 diffuse = diff * lightColor;\n"
"	vec3 result = (ambient + diffuse + specular) * objColor;\n"
"	FragColor = vec4(result, 1.0f);\n"
"}\0";

const char* vertex = "#version 330\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 0) in vec3 aNormal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec4 vertexColor;\n"
"out vec3 normal;\n"
"out vec3 fragPosition;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
"    vertexColor = vec4(1.f, 0.1f, 0.1f, 1.0f);\n"
"    normal = aNormal;\n"
"    fragPosition = vec3(model * vec4(aPos, 1.0f));\n"
"}\0";

class Shader
{

public:
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
	{
		std::string v;
		std::string f;
		/*std::ifstream vs;
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
			exit(-1);
		}*/


		int status;
		unsigned int vsi = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fsi = glCreateShader(GL_FRAGMENT_SHADER);
		const char* vsc = vertex, *fsc = fragment;
		glShaderSource(vsi, 1, &vsc, NULL);
		glCompileShader(vsi);
		glGetShaderiv(vsi, GL_COMPILE_STATUS, &status);
		if (!status) {
			std::cout << "[Error] Failed to compile vertex shader."; exit(-1);
		}

		glShaderSource(fsi, 1, &fsc, NULL);
		glCompileShader(fsi);
		glGetShaderiv(fsi, GL_COMPILE_STATUS, &status);
		if (!status) {
			std::cout << "[Error] Failed to compile fragment shader."; exit(-1);
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

	void setUv3f(const char* name, const float* value) const
	{
		glUniform3fv(glGetUniformLocation(shaderProgramID, name), 1, value);
	}

private:
	unsigned int shaderProgramID;
};


#endif