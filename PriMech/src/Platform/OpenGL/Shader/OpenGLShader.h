#pragma once

#include <string>
#include <glm/glm.hpp>
#include <Primech/Renderer/Shader.h>

typedef unsigned int GLenum;

namespace PriMech {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat3(const glm::mat3& value, const std::string& name) override;
		virtual void SetMat4(const glm::mat4& value, const std::string& name) override;

		virtual void SetInt(int value, const std::string& name) override;
		virtual void SetIntArray(int* values, uint32_t count, const std::string& name) override;
		virtual void SetFloat(float value, const std::string& name) override;
		virtual void SetFloat2(const glm::vec2& value, const std::string& name) override;
		virtual void SetFloat3(const glm::vec3& value, const std::string& name) override;
		virtual void SetFloat4(const glm::vec4& value, const std::string& name) override;

		void UploadUniformMat3(const glm::mat3& matrix, const std::string& name);
		void UploadUniformMat4(const glm::mat4& matrix, const std::string& name);

		void UploadUniformInt(int value, const std::string& name);
		void UploadUniformIntArray(int* values, uint32_t count, const std::string& name);
		void UploadUniformFloat(float value, const std::string& name);
		void UploadUniformFloat2(const glm::vec2& value, const std::string& name);
		void UploadUniformFloat3(const glm::vec3& value, const std::string& name);
		void UploadUniformFloat4(const glm::vec4& value, const std::string& name);

		inline virtual const std::string& GetName() const override { return name_; };
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcessFile(const std::string& shaderSource);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSourceMap);

		uint32_t rendererID_;	
		std::string name_;
	};
}