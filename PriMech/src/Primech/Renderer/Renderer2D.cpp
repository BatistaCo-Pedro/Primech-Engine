#include "ppch.h"
#include "Renderer2D.h"

#include "Primech/Renderer/VertexArray.h"
#include "PriMech/Renderer/Shader.h"
#include "Primech/Renderer/RendererCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace PriMech {
	struct Renderer2DStorage {
		Ref<VertexArray> squareVertexArray_;
		Ref<Shader> textureShader_;
		Ref<Texture2D> whiteTexture_;
	};

	static Renderer2DStorage* staticData;

	void Renderer2D::Init() {
		PM_PROFILE_FUNCTION();
		staticData = new Renderer2DStorage();

		staticData->squareVertexArray_ = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<VertexBuffer> squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		squareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "attributePosition" },
			{ ShaderDataType::Float2, "attributeTextureCoord"}
		});
		staticData->squareVertexArray_->AddVertexBuffer(squareVertexBuffer);

		Ref<IndexBuffer> squareIndexBuffer = IndexBuffer::Create(squareIndices, (sizeof(squareIndices) / sizeof(squareIndices[0])));
		staticData->squareVertexArray_->SetIndexBuffer(squareIndexBuffer);

		staticData->whiteTexture_ = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		staticData->whiteTexture_->SetData(&whiteTextureData, sizeof(uint32_t));

		staticData->textureShader_ = Shader::Create("assets/shaders/Texture.glsl");
		staticData->textureShader_->Bind();
		staticData->textureShader_->SetInt(0, "uniformTexture");
	}

	void Renderer2D::Shutdown() {
		PM_PROFILE_FUNCTION();
		delete staticData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PM_PROFILE_FUNCTION();
		staticData->textureShader_->Bind();
		staticData->textureShader_->SetMat4(camera.GetViewProjectionMatrix(), "uniformViewProjection");
	}

	void Renderer2D::EndScene() {
		PM_PROFILE_FUNCTION();

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		PM_PROFILE_FUNCTION();
		staticData->textureShader_->SetFloat4(color, "uniformColor");
		staticData->textureShader_->SetFloat(1.0f, "uniformTileMultiplier");
		staticData->whiteTexture_->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		staticData->textureShader_->SetMat4(transform, "uniformTransform");

		staticData->squareVertexArray_->Bind();
		RendererCommand::DrawIndexed(staticData->squareVertexArray_);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tileMultiplier, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor) {
		PM_PROFILE_FUNCTION();
		staticData->textureShader_->SetFloat4(tintColor, "uniformColor");
		staticData->textureShader_->SetFloat(tileMultiplier, "uniformTileMultiplier");
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		staticData->textureShader_->SetMat4(transform, "uniformTransform");

		staticData->squareVertexArray_->Bind();
		RendererCommand::DrawIndexed(staticData->squareVertexArray_);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		PM_PROFILE_FUNCTION();
		staticData->textureShader_->SetFloat4(color, "uniformColor");
		staticData->textureShader_->SetFloat(1.0f, "uniformTileMultiplier");
		staticData->whiteTexture_->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		staticData->textureShader_->SetMat4(transform, "uniformTransform");

		staticData->squareVertexArray_->Bind();
		RendererCommand::DrawIndexed(staticData->squareVertexArray_);
	} 
	
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintcolor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tileMultiplier, tintcolor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintcolor) {
		PM_PROFILE_FUNCTION();
		staticData->textureShader_->SetFloat4(tintcolor, "uniformColor");
		staticData->textureShader_->SetFloat(tileMultiplier, "uniformTileMultiplier");
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		staticData->textureShader_->SetMat4(transform, "uniformTransform");

		staticData->squareVertexArray_->Bind();
		RendererCommand::DrawIndexed(staticData->squareVertexArray_);
	}
}