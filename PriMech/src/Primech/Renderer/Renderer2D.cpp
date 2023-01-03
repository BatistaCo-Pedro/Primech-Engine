#include "ppch.h"
#include "Renderer2D.h"

#include "Primech/Renderer/VertexArray.h"
#include "PriMech/Renderer/Shader.h"
#include "Primech/Renderer/RendererCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace PriMech {
	struct Renderer2DStorage {
		Ref<VertexArray> squareVertexArray_;
		Ref<Shader> shader_;
		Ref<Shader> textureShader_;
	};

	static Renderer2DStorage* staticData;

	void Renderer2D::Init() {
		staticData = new Renderer2DStorage();

		staticData->squareVertexArray_ = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "attributePosition" },
			{ ShaderDataType::Float2, "attributeTextureCoord"}
		};
		squareVertexBuffer->SetLayout(squareLayout);

		staticData->squareVertexArray_->AddVertexBuffer(squareVertexBuffer);

		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(squareIndices, (sizeof(squareIndices) / sizeof(squareIndices[0])));
		staticData->squareVertexArray_->SetIndexBuffer(squareIndexBuffer);

		staticData->shader_ = Shader::Create("assets/shaders/FlatColor.glsl");
		staticData->textureShader_ = Shader::Create("assets/shaders/Texture.glsl");
		staticData->textureShader_->Bind();
		staticData->textureShader_->SetInt(0, "uniformTexture");
	}

	void Renderer2D::Shutdown() {
		delete staticData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		staticData->shader_->Bind();
		staticData->shader_->SetMat4(camera.GetViewProjectionMatrix(), "uniformViewProjection");

		staticData->textureShader_->Bind();
		staticData->textureShader_->SetMat4(camera.GetViewProjectionMatrix(), "uniformViewProjection");
	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		staticData->shader_->Bind();
		staticData->shader_->SetFloat4(color, "uniformColor");

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		staticData->shader_->SetMat4(transform, "uniformTransform");

		staticData->squareVertexArray_->Bind();
		RendererCommand::DrawIndexed(staticData->squareVertexArray_);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
		staticData->textureShader_->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		staticData->textureShader_->SetMat4(transform, "uniformTransform");

		texture->Bind();

		staticData->squareVertexArray_->Bind();
		RendererCommand::DrawIndexed(staticData->squareVertexArray_);
	}
}