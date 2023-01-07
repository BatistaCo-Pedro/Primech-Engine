#pragma once
#include "Primech.h"

class Sandbox2D : public PriMech::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(PriMech::Timestep timestep) override;
	virtual void OnEvent(PriMech::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	//Temp
	PriMech::Ref<PriMech::VertexArray> quadVertexArray_;
	PriMech::Ref<PriMech::Shader> shader_;
	PriMech::Ref<PriMech::Texture2D> checkerboardTexture_;
	glm::vec4 squareColor_ = { 157.0f / 255, 0.0f, 1.0f, 1.0f };

	PriMech::OrthographicCameraController cameraController_;
};