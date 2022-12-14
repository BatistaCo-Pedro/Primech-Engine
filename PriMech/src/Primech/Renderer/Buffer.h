#pragma once

namespace PriMech {

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	//Temp location in Buffer.h
	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 8;
		case ShaderDataType::Float3:	return 12;
		case ShaderDataType::Float4:	return 16;
		case ShaderDataType::Mat3:		return 36;
		case ShaderDataType::Mat4:		return 64;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 8;
		case ShaderDataType::Int3:		return 12;
		case ShaderDataType::Int4:		return 16;
		case ShaderDataType::Bool:		return 1;
		}

		PM_CORE_ASSERT(false, "Unkown ShaderDataType at ShaderDataTypeSize()");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized = false;

		BufferElement() {}
		BufferElement(ShaderDataType inType, const std::string& inName, bool inNormalized = false) :
			name(inName), type(inType), size(ShaderDataTypeSize(type)), offset(0), normalized(inNormalized) { }

		const uint32_t GetComponentCount() const {
			switch (type)
			{
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 9;
			case ShaderDataType::Mat4:		return 16;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
			}
			PM_CORE_ASSERT(false, "Unkown ShaderDataType at GetComponentCount()");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) : elements_(elements) {
			CalculateOffsetAndStride();
		}

		inline const uint32_t GetStride() const { return stride_; }
		inline const std::vector<BufferElement>& GetElements() { return elements_; }

		const std::vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
		const std::vector<BufferElement>::const_iterator end() const { return elements_.end(); }
		std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
		std::vector<BufferElement>::iterator end() { return elements_.end(); }
	private:
		std::vector<BufferElement> elements_;
		uint32_t stride_ = 0;

		inline void CalculateOffsetAndStride() {
			uint32_t offset = 0;
			stride_ = 0;
			for (auto& element : elements_) {
				element.offset = offset;
				offset += element.size;
				stride_ += element.size;
			}
		};
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		static VertexBuffer* Create(float* vertices, uint32_t size);
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};
}