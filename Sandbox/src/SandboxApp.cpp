#include <Primech.h>
#include <Primech/Core/Entrypoint.h>

#include "Sandbox2D.h"
#include "Platform/OpenGL/Shader/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public PriMech::Layer {
public:
	ExampleLayer() : Layer("Example"), cameraController_(1920.0f / 1080.0f, true) {
		vertexArray_ = PriMech::VertexArray::Create();

		//defining the points pos
		/*
			Offset -> item1, item2, item3, item4
			Choice: item4 -> offset item1 + item2 + item3 in bits
			Choice: item2 -> offset item1 in bits
			Offset is absolute not relative, always from the start: pos 0

			Stride -> item1, item2, item3
					  item1, item2, item3
			The Stride is the difference from vertex to vertex in bits,
			in this case item1 + item2 + item3 in Bits
		*/
		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.3f, 0.3f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 0.3f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
		};

		uint32_t indices[3] = { 0, 1, 2 };

		//Creating new unqiue pointer
		PriMech::Ref<PriMech::VertexBuffer> vertexBuffer;
		vertexBuffer = PriMech::VertexBuffer::Create(vertices, sizeof(vertices));

		PriMech::BufferLayout layout = {
			{ PriMech::ShaderDataType::Float3, "attributePosition" },
			{ PriMech::ShaderDataType::Float4, "attributeColor" },
		};
		vertexBuffer->SetLayout(layout);
		vertexArray_->AddVertexBuffer(vertexBuffer);

		//Creating new unqiue pointer
		PriMech::Ref<PriMech::IndexBuffer> indexBuffer;
		indexBuffer = PriMech::IndexBuffer::Create(indices, (sizeof(indices) / sizeof(indices[0])));
		vertexArray_->SetIndexBuffer(indexBuffer);

		squareVertexArray_ = PriMech::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		PriMech::Ref<PriMech::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = PriMech::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		PriMech::BufferLayout squareLayout = {
			{ PriMech::ShaderDataType::Float3, "attributePosition" },
			{ PriMech::ShaderDataType::Float2, "attributeTextureCoord"},
		};
		squareVertexBuffer->SetLayout(squareLayout);

		squareVertexArray_->AddVertexBuffer(squareVertexBuffer);

		PriMech::Ref<PriMech::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = PriMech::IndexBuffer::Create(squareIndices, (sizeof(squareIndices) / sizeof(squareIndices[0])));
		squareVertexArray_->SetIndexBuffer(squareIndexBuffer);

		//Temp
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 attributePosition;
			layout(location = 1) in vec4 attributeColor;

			uniform mat4 uniformViewProjection;
			uniform mat4 uniformTransform;

			out vec3 varPosition;
			out vec4 varColor;

			void main() {
				varPosition = attributePosition;
				varColor = attributeColor;
				gl_Position = uniformViewProjection * uniformTransform * vec4(attributePosition, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 outColor;

			in vec3 varPosition;
			in vec4 varColor;

			void main() {
				vec4 color = vec4(varPosition * 0.5 + 0.5, 1.0);
				outColor = color;
				outColor = varColor;
			}
		)";



		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 attributePosition;
			

			uniform mat4 uniformViewProjection;
			uniform mat4 uniformTransform;

			void main() {
				gl_Position = uniformViewProjection * uniformTransform * vec4(attributePosition, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 outColor;

			uniform vec3 uniformColor;
			
			void main() {
				outColor = vec4(uniformColor, 1.0);
			}
		)";

		shader_ = PriMech::Shader::Create("TriangleShader", vertexSrc, fragmentSrc);
		flatColorShader_ = PriMech::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);
		PriMech::Ref<PriMech::Shader> textureShader = shaderLibrary_.Load("assets/shaders/Texture.glsl");

		texture_ = PriMech::Texture2D::Create("assets/textures/checkerboard.png");
		textureTest_ = PriMech::Texture2D::Create("assets/textures/CPic.png");

		std::dynamic_pointer_cast<PriMech::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<PriMech::OpenGLShader>(textureShader)->UploadUniformInt(0, "uniformTexture"); //uploading to slot 0

	}

	void OnUpdate(PriMech::Timestep timestep) override {
		cameraController_.OnUpdate(timestep);

		//Shader code
		PriMech::RendererCommand::ClearWithColor({ 0.1f, 0.1f, 0.1f, 0.0f });

		PriMech::Renderer::BeginScene(cameraController_.GetCamera());

		float scaleMultiplier = 1.0f;
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f) * scaleMultiplier);

		std::dynamic_pointer_cast<PriMech::OpenGLShader>(flatColorShader_)->Bind();
		std::dynamic_pointer_cast<PriMech::OpenGLShader>(flatColorShader_)->UploadUniformFloat3(squareColor_, "uniformColor");

		/* Material API may look like:
			PriMech::MaterialRef material = new PriMech::Material(flatColorShader_);
			PriMech::MaterialInstanceRef materialInstance = new PriMech::MaterialInstance(material);

			materialInstance->Set("uniformColor", redColor);
			materialInstance->Set("uniformAlbedoMap", albedoMapTexture);
			squareMesh->SetMaterial(materialInstance);
		*/
		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 20; y++) {
				glm::vec3 pos(x * 0.11f * scaleMultiplier, y * 0.11f * scaleMultiplier, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				PriMech::Renderer::Submit(squareVertexArray_, flatColorShader_, transform);
			}		
		}	
		PriMech::Ref<PriMech::Shader> textureShader = shaderLibrary_.Get("Texture");
		
		//Triangle rendering
		//PriMech::Renderer::Submit(vertexArray_, shader_);
		texture_->Bind();
		PriMech::Renderer::Submit(squareVertexArray_, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		textureTest_->Bind();
		PriMech::Renderer::Submit(squareVertexArray_, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		PriMech::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor_));
		ImGui::End();
	}

	void OnEvent(PriMech::Event& event) override {
		PriMech::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<PriMech::KeyPressedEvent>(PM_BIND_EVENT_FUNCTION(ExampleLayer::OnKeyPressedEvent));
		cameraController_.OnEvent(event);
	}

	bool OnKeyPressedEvent(PriMech::KeyPressedEvent& event) {
		PM_INFO("{0} was pressed", event.GetKeyCode());
		return false;
	}
private:
	PriMech::ShaderLibrary shaderLibrary_;
	PriMech::Ref<PriMech::Shader> shader_;
	PriMech::Ref<PriMech::Shader> flatColorShader_;

	PriMech::Ref<PriMech::VertexArray> vertexArray_;
	PriMech::Ref<PriMech::VertexArray> squareVertexArray_;

	PriMech::Ref<PriMech::Texture2D> texture_, textureTest_;
	glm::vec3 squareColor_ = { 0.2f, 0.3f, 0.8f };

	PriMech::OrthographicCameraController cameraController_;

};

class Sandbox : public PriMech::Application
{
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {}

private:

};

PriMech::Application* PriMech::CreateApplication() {
	return new Sandbox(); //Sandbox is of type Application as it inherits from it
}