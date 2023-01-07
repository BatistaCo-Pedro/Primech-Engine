#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController_(1920.0f / 1080.0f, true) {}

void Sandbox2D::OnAttach() {
	PM_PROFILE_FUNCTION();
	checkerboardTexture_ = PriMech::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnDetach() {
	PM_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(PriMech::Timestep timestep) {
	PM_PROFILE_FUNCTION();
	cameraController_.OnUpdate(timestep);
	PM_WARN("FPS: {0}", 1.0f / timestep);
	PriMech::Renderer2D::ResetStatistics();

	PriMech::RendererCommand::ClearWithColor({0.1f, 0.1f, 0.1f, 0.0f});
	{
		static float rotation = 0.0f;
		rotation += timestep * 60;
		
		PM_PROFILE_SCOPE("Sandbox2D::Rendering::OnUpdate()");
		PriMech::Renderer2D::BeginScene(cameraController_.GetCamera());

		PriMech::Renderer2D::DrawRotatedQuad({ 1.3f, 0.3f }, { 0.5f, 0.75f }, rotation, { 0.2f, 0.3f, 0.8f, 1.0f });
		PriMech::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 30.0f, 30.0f }, checkerboardTexture_, 100.0f);
		PriMech::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		PriMech::Renderer2D::DrawQuad({ 0.5f, -0.2f }, { 0.8f, 0.5f }, squareColor_);
		PriMech::Renderer2D::DrawQuad({ -1.2f, -1.2f, 0.1f }, { 1.0f, 1.0f }, checkerboardTexture_, 20.0f);
		PriMech::Renderer2D::DrawRotatedQuad({ -0.0f, 0.0f }, { 1.0f, 1.0f }, 45.0f, checkerboardTexture_, 10.0f, glm::vec4(0.5f, 0.9f, 0.5f, 1.0f));
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.65f };
				PriMech::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		PriMech::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(PriMech::Event& event) {
	cameraController_.OnEvent(event);
}

void Sandbox2D::OnImGuiRender() {
	PM_PROFILE_FUNCTION();
	PriMech::Renderer2D::Statistics stats = PriMech::Renderer2D::GetStatistics();

	ImGui::Begin("Settings");

	ImGui::Text("Renderer2D Stats: ");
	ImGui::Text("Draw Calls:: %d", stats.drawCalls);
	ImGui::Text("Quad Count: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor_));

	ImGui::End();
}
