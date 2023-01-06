#include "ppch.h"
#include "Renderer2D.h"

#include "Primech/Renderer/VertexArray.h"
#include "PriMech/Renderer/Shader.h"
#include "Primech/Renderer/RendererCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace PriMech {
	struct QuadVertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoord;
		//Todo:textureID
	};

	struct Renderer2DData {
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;
	};
	static Renderer2DData s_Data;

	void Renderer2D::Init() {
		PM_PROFILE_FUNCTION();

		s_Data.quadVertexArray = VertexArray::Create();
		s_Data.quadVertexBuffer = VertexBuffer::Create(s_Data.maxVertices * sizeof(QuadVertex));		

		s_Data.quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "attributePosition" },
			{ ShaderDataType::Float4, "attributeColor" },
			{ ShaderDataType::Float2, "attributeTextureCoord"}
		});
		s_Data.quadVertexArray->AddVertexBuffer(s_Data.quadVertexBuffer);

		s_Data.quadVertexBufferBase = new QuadVertex[s_Data.maxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.maxIndices]; //allocate on the heap -> potential for stack overflow
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.maxIndices; i += 6) {
			quadIndices[i] = offset;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.maxIndices);;
		s_Data.quadVertexArray->SetIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;


		s_Data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.textureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.textureShader->Bind();
		s_Data.textureShader->SetInt(0, "uniformTexture");
	}

	void Renderer2D::Shutdown() {
		PM_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PM_PROFILE_FUNCTION();
		s_Data.textureShader->Bind();
		s_Data.textureShader->SetMat4(camera.GetViewProjectionMatrix(), "uniformViewProjection");

		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
		s_Data.quadIndexCount = 0;
	}

	void Renderer2D::EndScene() {
		PM_PROFILE_FUNCTION();
		uint32_t dataSize = (uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase;
		s_Data.quadVertexBuffer->SetData(s_Data.quadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {
		RendererCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		PM_PROFILE_FUNCTION();

		s_Data.quadVertexBufferPtr->position = position;
		s_Data.quadVertexBufferPtr->color = color;
		s_Data.quadVertexBufferPtr->textureCoord = { 0.0f, 0.0f, };
		s_Data.quadVertexBufferPtr++;

		s_Data.quadVertexBufferPtr->position = { position.x + size.x, position.y, 0.0f };
		s_Data.quadVertexBufferPtr->color = color;
		s_Data.quadVertexBufferPtr->textureCoord = { 1.0f, 0.0f, };
		s_Data.quadVertexBufferPtr++;

		s_Data.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.quadVertexBufferPtr->color = color;
		s_Data.quadVertexBufferPtr->textureCoord = { 1.0f, 1.0f, };
		s_Data.quadVertexBufferPtr++;

		s_Data.quadVertexBufferPtr->position = { position.x, position.y + size.y, 0.0f };
		s_Data.quadVertexBufferPtr->color = color;
		s_Data.quadVertexBufferPtr->textureCoord = { 0.0f, 1.0f, };
		s_Data.quadVertexBufferPtr++;

		s_Data.quadIndexCount += 6;

		/*s_Data.textureShader_->SetFloat(1.0f, "uniformTileMultiplier");
		s_Data.whiteTexture_->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data.textureShader_->SetMat4(transform, "uniformTransform");

		s_Data.squareVertexArray_->Bind();
		RendererCommand::DrawIndexed(s_Data.squareVertexArray_);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tileMultiplier, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor) {
		PM_PROFILE_FUNCTION();
		s_Data.textureShader->SetFloat4(tintColor, "uniformColor");
		s_Data.textureShader->SetFloat(tileMultiplier, "uniformTileMultiplier");
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data.textureShader->SetMat4(transform, "uniformTransform");

		s_Data.quadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.quadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		PM_PROFILE_FUNCTION();
		s_Data.textureShader->SetFloat4(color, "uniformColor");
		s_Data.textureShader->SetFloat(1.0f, "uniformTileMultiplier");
		s_Data.whiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		s_Data.textureShader->SetMat4(transform, "uniformTransform");

		s_Data.quadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.quadVertexArray);
	} 
	
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintcolor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tileMultiplier, tintcolor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintcolor) {
		PM_PROFILE_FUNCTION();
		s_Data.textureShader->SetFloat4(tintcolor, "uniformColor");
		s_Data.textureShader->SetFloat(tileMultiplier, "uniformTileMultiplier");
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		s_Data.textureShader->SetMat4(transform, "uniformTransform");

		s_Data.quadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.quadVertexArray);
	}
}