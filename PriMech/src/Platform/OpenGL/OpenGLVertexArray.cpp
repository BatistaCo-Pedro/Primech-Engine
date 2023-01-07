#include "ppch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace PriMech {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}
		PM_CORE_ASSERT(false, "Unkown ShaderDataType at DataTypeConversion()");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		PM_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &rendererID_);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		PM_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &rendererID_);
	}

	void OpenGLVertexArray::Bind() const {
		PM_PROFILE_FUNCTION();
		glBindVertexArray(rendererID_);
	}

	void OpenGLVertexArray::UnBind() const {
		PM_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		PM_PROFILE_FUNCTION();
		glBindVertexArray(rendererID_);
		vertexBuffer->Bind();

		PM_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		const BufferLayout layout = vertexBuffer->GetLayout();
		for (const BufferElement element : layout) {
			for (const BufferElement& element : layout) {
				switch (element.type) {
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(vertexBufferIndex_);
					glVertexAttribPointer(vertexBufferIndex_,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.offset);
					vertexBufferIndex_++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(vertexBufferIndex_);
						glVertexAttribPointer(vertexBufferIndex_,
							count,
							ShaderDataTypeToOpenGLBaseType(element.type),
							element.normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(sizeof(float) * count * i));
						glVertexAttribDivisor(vertexBufferIndex_, 1);
						vertexBufferIndex_++;
					}
					break;
				}
				default:
					PM_CORE_ASSERT(false, "Unknown ShaderDataType!");
				}
			}
		}
		vertexBuffers_.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		PM_PROFILE_FUNCTION();
		glBindVertexArray(rendererID_);
		indexBuffer->Bind();

		indexBuffer_ = indexBuffer;
	}

	VertexArray* OpenGLVertexArray::Create() {
	
		return nullptr;
	}
}