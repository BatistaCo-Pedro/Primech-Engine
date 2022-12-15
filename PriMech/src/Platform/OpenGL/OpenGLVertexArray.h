#pragma once
#include "Primech/Renderer/VertexArray.h"

namespace PriMech {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray()  {}

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		static VertexArray* Create();
	private:
		uint32_t rendererID_;
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers_;
		std::shared_ptr<IndexBuffer> indexBuffer_;
	};
}