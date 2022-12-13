#pragma once

namespace PriMech {
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
		//DirectX,
		//etc...
	};

	class Renderer {
	public:
		inline static RendererAPI GetAPI() { return staticRendererAPI; }
	private:
		static RendererAPI staticRendererAPI;
	};
}

