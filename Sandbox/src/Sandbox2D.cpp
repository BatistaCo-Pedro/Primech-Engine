#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController_(1920.0f / 1080.0f, true) {}

void Sandbox2D::OnAttach() {
	checkerboardTexture_ = PriMech::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnDetach() {
	PriMech::Renderer2D::Shutdown();
}

void Sandbox2D::OnUpdate(PriMech::Timestep timestep) {
	PM_PROFILE_FUNCTION();
	{
		PM_PROFILE_SCOPE("Sandbox2D::CameraController::OnUpdate()");
		cameraController_.OnUpdate(timestep);
	}
	PriMech::RendererCommand::ClearWithColor({0.1f, 0.1f, 0.1f, 0.0f});

	{
		PM_PROFILE_SCOPE("Sandbox2D::Rendering::OnUpdate()");
		PriMech::Renderer2D::BeginScene(cameraController_.GetCamera());

		for (float i = 0.0f; i < 10.0f; i++) {
			PriMech::Renderer2D::DrawQuad({ 0.6f + i/100.0f, -0.6f-i/100.0f }, { 1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f });
		}
		PriMech::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		PriMech::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		PriMech::Renderer2D::DrawQuad({ 0.2f, 0.5f, -0.1f }, { 10.0f, 10.0f }, checkerboardTexture_);

		PriMech::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(PriMech::Event& event) {
	cameraController_.OnEvent(event);
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor_));
	ImGui::End();
}
