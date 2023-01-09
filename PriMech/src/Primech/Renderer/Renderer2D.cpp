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
		float textureIndex;
		float tilingFactor;
	};

	struct Renderer2DData {
		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; //texture slot 0 -> whiteTexture

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};
	static Renderer2DData s_Data;


	void Renderer2D::Init() {
		PM_PROFILE_FUNCTION();

		s_Data.quadVertexArray = VertexArray::Create();
		s_Data.quadVertexBuffer = VertexBuffer::Create(s_Data.maxVertices * sizeof(QuadVertex));		

		s_Data.quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "attributePosition" },
			{ ShaderDataType::Float4, "attributeColor" },
			{ ShaderDataType::Float2, "attributeTextureCoord"},
			{ ShaderDataType::Float, "attributeTextureIndex"},
			{ ShaderDataType::Float, "attributeTilingFactor"},
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
		int32_t whiteTextureData = 0xffffffff;
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));


		int32_t samplers[s_Data.maxTextureSlots];
		for (uint32_t i = 0; i < s_Data.maxTextureSlots; i++)
			samplers[i] = i;

		s_Data.textureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.textureShader->Bind();
		s_Data.textureShader->SetIntArray(samplers, s_Data.maxTextureSlots, "uniformTextures");

		s_Data.textureSlots[0] = s_Data.whiteTexture;

		s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f }; //left down
		s_Data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f }; //right down
		s_Data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f }; //right up
		s_Data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f }; //left up
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
		s_Data.textureSlotIndex = 1;
	}

	void Renderer2D::EndScene() {
		PM_PROFILE_FUNCTION();
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase);
		s_Data.quadVertexBuffer->SetData(s_Data.quadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {
		for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) 
			s_Data.textureSlots[i]->Bind(i);
		
		RendererCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
		s_Data.stats.drawCalls++;
	}

	void Renderer2D::StartNewBatch() {
		EndScene();

		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
		s_Data.quadIndexCount = 0;
		s_Data.textureSlotIndex = 1;
	}

	static void AdjustRenderer2DQuadData(const glm::mat4& transform, const glm::vec4& color,const Ref<Texture2D>& texture = nullptr, float tilingFactor = 1.0f) {
		constexpr size_t quadVertexCount = 4;
		float textureIndex = 0; //white texture
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if (texture != nullptr) {
			for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
				if (*s_Data.textureSlots[i].get() == *texture.get()) /*temp -> to ugly*/ {
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f) {
				/*if (s_Data.textureSlotIndex >= Renderer2DData::maxTextureSlots)
					StartNewBatch();*/

				textureIndex = (float)s_Data.textureSlotIndex;
				s_Data.textureSlots[s_Data.textureSlotIndex] = texture; //temp ->needs specific textureID
				s_Data.textureSlotIndex++;
			}
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = color;
			s_Data.quadVertexBufferPtr->textureCoord = textureCoords[i];
			s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
			s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_Data.quadVertexBufferPtr++;
		}
		s_Data.quadIndexCount += 6;
		s_Data.stats.quadCount++;
	}

#pragma region DrawingMethods

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		PM_PROFILE_FUNCTION();
		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			StartNewBatch();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		AdjustRenderer2DQuadData(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tileMultiplier, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor) {
		PM_PROFILE_FUNCTION();
		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			StartNewBatch();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		AdjustRenderer2DQuadData(transform, tintColor, texture, tileMultiplier);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		PM_PROFILE_FUNCTION();
		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			StartNewBatch();

		float textureIndex = 0.0f; //white texture

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		AdjustRenderer2DQuadData(transform, color);
	} 

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tileMultiplier, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor) {
		PM_PROFILE_FUNCTION();
		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices)
			StartNewBatch();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		AdjustRenderer2DQuadData(transform, tintColor, texture, tileMultiplier);
	}

	void Renderer2D::ResetStatistics() {
		memset(&s_Data.stats, 0, sizeof(Renderer2D::Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return s_Data.stats;
	}
}
#pragma endregion
