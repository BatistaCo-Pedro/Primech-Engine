#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController_(1920.0f / 1080.0f, true) {}

void Sandbox2D::OnAttach() {
	PriMech::Renderer::Init();
}

void Sandbox2D::OnDetach() {
	PriMech::Renderer2D::Shutdown();
}

void Sandbox2D::OnUpdate(PriMech::Timestep timestep) {
	cameraController_.OnUpdate(timestep);

	PriMech::RendererCommand::ClearWithColor({ 0.1f, 0.1f, 0.1f, 0.0f });

	PriMech::Renderer2D::BeginScene(cameraController_.GetCamera());

	PriMech::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });

	PriMech::Renderer2D::EndScene();
	
	//TODO: Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<PriMech::OpenGLShader>(shader_)->Bind();
	//std::dynamic_pointer_cast<PriMech::OpenGLShader>(shader_)->UploadUniformFloat4(squareColor_, "uniformColor");
}

void Sandbox2D::OnEvent(PriMech::Event& event) {
	cameraController_.OnEvent(event);
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor_));
	ImGui::End();
}
