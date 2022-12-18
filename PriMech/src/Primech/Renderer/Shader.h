#pragma once

#include <string>
#include <glm/mat4x4.hpp>

namespace PriMech {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const glm::mat4& matrix, const std::string& name = name_);
	private:
		uint32_t rendererID_;
		static const std::string& name_;
	};
}