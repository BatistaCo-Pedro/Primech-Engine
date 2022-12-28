#pragma once
#include "Primech/Renderer/VertexArray.h"

namespace PriMech {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return vertexBuffers_; }
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return indexBuffer_; }

		static VertexArray* Create();
	private:
		uint32_t rendererID_;
		std::vector<Ref<VertexBuffer>> vertexBuffers_;
		Ref<IndexBuffer> indexBuffer_;
	};
}