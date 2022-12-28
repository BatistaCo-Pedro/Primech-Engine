#pragma once
#include "ppch.h"
#include "RendererAPI.h"

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace PriMech {
	class RendererAPI {
	public:
		enum class API {
			None = 0, OpenGL = 1, //directx, etc..
		};

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void ClearWithColor(const glm::vec4& color) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return staticAPI_; }
	private:
		static API staticAPI_;
	};
}
