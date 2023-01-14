#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController_(1920.0f / 1080.0f, true) {}

void Sandbox2D::OnAttach() {
	PM_PROFILE_FUNCTION();
	checkerboardTexture_ = PriMech::Texture2D::Create("assets/textures/checkerboard.png");
	spriteSheet_ = PriMech::Texture2D::Create("assets/testGame/textures/RPGpack_sheet.png");

	particle_.colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	particle_.colorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	particle_.sizeBegin = 0.5f, particle_.sizeVariation = 0.3f, particle_.sizeEnd = 0.0f;
	particle_.lifeTime = 3.0f;
	particle_.velocity = { 0.0f, 0.0f };
	particle_.velocityVariation = { 3.0f, 1.0f };
	particle_.position = { 0.0f, 0.0f };
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
#if 0
	{
		static float rotation = 0.0f;
		rotation += timestep * 60;
		
		PM_PROFILE_SCOPE("Sandbox2D::Rendering::OnUpdate()");
		PriMech::Renderer2D::BeginScene(cameraController_.GetCamera());

		PriMech::Renderer2D::DrawRotatedQuad({ 1.3f, 0.3f }, { 0.5f, 0.75f }, glm::radians(rotation), { 0.2f, 0.3f, 0.8f, 1.0f });
		PriMech::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 30.0f, 30.0f }, checkerboardTexture_, 100.0f);
		PriMech::Renderer2D::DrawQuad({ 5.0f, 5.0f, 0.1f }, { 2.0f, 2.0f }, checkerboardTexture_, 5.0f, squareColor_);
		PriMech::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		PriMech::Renderer2D::DrawQuad({ 0.5f, -0.2f }, { 0.8f, 0.5f }, squareColor_);
		PriMech::Renderer2D::DrawQuad({ -1.2f, -1.2f, 0.1f }, { 1.0f, 1.0f }, checkerboardTexture_, 20.0f);
		PriMech::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(45.0f), checkerboardTexture_, 10.0f, glm::vec4(0.5f, 0.9f, 0.5f, 1.0f));
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.65f };
				PriMech::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		PriMech::Renderer2D::EndScene();
	}
#endif
	if (PriMech::Input::IsMouseButtonPressed(PM_MOUSE_BUTTON_LEFT)) {
		auto [x, y] = PriMech::Input::GetMousePos();
		auto width = PriMech::Application::GetApplication().GetWindow().GetWidth();
		auto height = PriMech::Application::GetApplication().GetWindow().GetHeight();

		auto bounds = cameraController_.GetBounds();
		auto pos = cameraController_.GetCamera().GetPostion();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		particle_.position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 10; i++) {
			particleSystem_.Emit(particle_);
		}
	}
	particleSystem_.OnUpdate(timestep);
	particleSystem_.OnRender(cameraController_.GetCamera());

	PriMech::Renderer2D::BeginScene(cameraController_.GetCamera());
	PriMech::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, checkerboardTexture_);
	PriMech::Renderer2D::EndScene();
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
