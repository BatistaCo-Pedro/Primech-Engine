#pragma once
#include "Primech/Core/Core.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace PriMech {
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(int value, const std::string& name) = 0;
		virtual void SetFloat(float value, const std::string& name) = 0;
		virtual void SetFloat2(const glm::vec2& value, const std::string& name) = 0;
		virtual void SetFloat3(const glm::vec3& value, const std::string& name) = 0;
		virtual void SetFloat4(const glm::vec4& value, const std::string& name) = 0;

		virtual void SetMat3(const glm::mat3& value, const std::string& name) = 0;
		virtual void SetMat4(const glm::mat4& value, const std::string& name) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary {
	public:
		void Add(const std::string& name,  const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath); //gets name from filepath
		Ref<Shader> Load(const std::string& name, const std::string& filepath); //unique name 

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> shaders_;
	};
}