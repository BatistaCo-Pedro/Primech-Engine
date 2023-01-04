#include "Sandbox2D.h"

#include <chrono>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& function) : name_(name), function_(function), stopped_(false)  {
		startTimepoint_ = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		if (!stopped_)
			Stop();
	}

	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint_).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
		float duration = (end - start) * 0.001f;

		stopped_ = true;
		function_({ name_, duration });
	}
private:
	const char* name_;
	Fn function_;
	std::chrono::time_point<std::chrono::steady_clock> startTimepoint_;
	bool stopped_;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { profileResults_.push_back(profileResult); })

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController_(1920.0f / 1080.0f, true) {}

void Sandbox2D::OnAttach() {
	checkerboardTexture_ = PriMech::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnDetach() {
	PriMech::Renderer2D::Shutdown();
}

void Sandbox2D::OnUpdate(PriMech::Timestep timestep) {
	PROFILE_SCOPE("Sandbox2D::OnUpdate()");
	{
		PROFILE_SCOPE("Sandbox2D::CameraController::OnUpdate()");
		cameraController_.OnUpdate(timestep);
	}
	PriMech::RendererCommand::ClearWithColor({0.1f, 0.1f, 0.1f, 0.0f});

	{
		PROFILE_SCOPE("Sandbox2D::Rendering::OnUpdate()");
		PriMech::Renderer2D::BeginScene(cameraController_.GetCamera());

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

	for (ProfileResult& result : profileResults_) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name_);

		ImGui::Text(label, result.Time_);
	}
	profileResults_.clear();
	ImGui::End();
}
