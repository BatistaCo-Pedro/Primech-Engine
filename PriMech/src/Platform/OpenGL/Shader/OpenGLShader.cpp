#include "ppch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <fstream>

#include "glm/gtc/type_ptr.hpp"

namespace PriMech {
	OpenGLShader::OpenGLShader(const std::string& filepath) {
		std::string shaderSource = ReadFile(filepath);
		auto shaderSourceMap = PreProcessFile(shaderSource);
		Compile(shaderSourceMap);

		// assets/shaders/Texture.glsl -> get Texture (filename) as name
		//Extract name from filepath
		size_t lastSlashPos = filepath.find_last_of("/\\");
		lastSlashPos = lastSlashPos == std::string::npos ? 0 : lastSlashPos + 1;
		size_t lastDotPos = filepath.rfind('.');
		size_t count = lastDotPos == std::string::npos ? filepath.size() - lastSlashPos : lastDotPos - lastSlashPos;
		name_ = filepath.substr(lastSlashPos, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : name_(name) {
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererID_);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			PM_CORE_ERROR("Could not open file in {0}", filepath);
		}

		return result;
	}

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") 
			return GL_VERTEX_SHADER;
		if (type == "pixel" || type == "fragment")
			return GL_FRAGMENT_SHADER;

		PM_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcessFile(const std::string& shaderSource) {
		std::unordered_map<GLenum, std::string> shaderSourceMap;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = shaderSource.find(typeToken, 0);

		while (pos != std::string::npos) {
			size_t endOfLine = shaderSource.find_first_of("\r\n", pos);
			PM_CORE_ASSERT(endOfLine != std::string::npos, "Syntax Error"); //if no new line exists
			size_t begin = pos + typeTokenLength + 1; //get the beginning of the type string
			std::string type = shaderSource.substr(begin, endOfLine - begin);
			PM_CORE_ASSERT(ShaderTypeFromString(type), "Invalid Shader type specifier");
			
			size_t nextLinePos = shaderSource.find_first_not_of("\r\n", endOfLine);
			pos = shaderSource.find(typeToken, nextLinePos);
			shaderSourceMap[ShaderTypeFromString(type)] = shaderSource.substr(nextLinePos, 
														pos - (nextLinePos == std::string::npos ? shaderSource.size() -1 : nextLinePos));
	
		}
		return shaderSourceMap;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSourceMap) {
		// Get a program object.
		GLuint program = glCreateProgram();
		PM_CORE_ASSERT(shaderSourceMap.size() <= 2, "Only support two Shaders momentarily");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDindex = 0;

		for (auto value : shaderSourceMap) {
			GLenum type = value.first;
			const std::string shaderSource = value.second;

			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = shaderSource.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			//check if the compilation succeeded
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				PM_CORE_ASSERT(false, "Shader compilation failure!");
				PM_CORE_ERROR("{0}", infoLog.data());
				break;
			}

			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDindex++] = shader; //post increment index
		}

		// Link our program
		glLinkProgram(program);

		//check if the linking succeeded
		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto glShaderID : glShaderIDs) {
				glDeleteShader(glShaderID);
			}

			PM_CORE_ASSERT(false, "GlProgram linking failure!");
			PM_CORE_ERROR("{0}", infoLog.data());
			return;
		}

		// Always detach shaders after a successful link.
		for (auto glShaderID : glShaderIDs) {
			glDetachShader(program, glShaderID);
		}

		rendererID_ = program;
	}

	void OpenGLShader::Bind() const {
		glUseProgram(rendererID_);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformMat3(const glm::mat3& matrix, const std::string& name) {
		GLint location = glGetUniformLocation(rendererID_, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const glm::mat4& matrix, const std::string& name) {
		GLint location = glGetUniformLocation(rendererID_, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformInt(int value, const std::string& name) {
		GLint location = glGetUniformLocation(rendererID_, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(float value, const std::string& name) {
		GLint location = glGetUniformLocation(rendererID_, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const glm::vec2& value, const std::string& name) {
		GLint location = glGetUniformLocation(rendererID_, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const glm::vec3& value, const std::string& name) {
		GLint location = glGetUniformLocation(rendererID_, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const glm::vec4& value, const std::string& name) {
		GLint location = glGetUniformLocation(rendererID_, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
}