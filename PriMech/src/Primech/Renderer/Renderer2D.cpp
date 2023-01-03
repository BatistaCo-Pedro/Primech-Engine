#include "ppch.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/Shader/OpenGLShader.h"
#include "Primech/Renderer/VertexArray.h"
#include "PriMech/Renderer/Shader.h"
#include "Primech/Renderer/RendererCommand.h"

namespace PriMech {
	struct Renderer2DStorage {
		Ref<VertexArray> squareVertexArray_;
		Ref<Shader> shader_;
	};

	static Renderer2DStorage* staticData;

	void Renderer2D::Init() {
		staticData = new Renderer2DStorage();

		staticData->squareVertexArray_ = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "attributePosition" },
		};
		squareVertexBuffer->SetLayout(squareLayout);

		staticData->squareVertexArray_->AddVertexBuffer(squareVertexBuffer);

		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(squareIndices, (sizeof(squareIndices) / sizeof(squareIndices[0])));
		staticData->squareVertexArray_->SetIndexBuffer(squareIndexBuffer);

		staticData->shader_ = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown() {
		delete staticData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		std::dynamic_pointer_cast<PriMech::OpenGLShader>(staticData->shader_)->Bind();
		std::dynamic_pointer_cast<PriMech::OpenGLShader>(staticData->shader_)->UploadUniformMat4(camera.GetViewProjectionMatrix(), "uniformViewProjection");
		std::dynamic_pointer_cast<PriMech::OpenGLShader>(staticData->shader_)->UploadUniformMat4(glm::mat4(1.0f), "uniformTransform");
	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		std::dynamic_pointer_cast<PriMech::OpenGLShader>(staticData->shader_)->Bind();
		std::dynamic_pointer_cast<PriMech::OpenGLShader>(staticData->shader_)->UploadUniformFloat4(color, "uniformColor");

		staticData->squareVertexArray_->Bind();
		RendererCommand::DrawIndexed(staticData->squareVertexArray_);
	}
}