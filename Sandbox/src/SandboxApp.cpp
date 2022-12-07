#include <Primech.h>

class ExampleLayer : public PriMech::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {
		PM_INFO("ExampleLayer::Update");
	}

	void OnEvent(PriMech::Event& event) override {
		PM_INFO("{0}", event);
	}
};

class Sandbox : public PriMech::Application
{
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}

private:

};

PriMech::Application* PriMech::CreateApplication() {
	return new Sandbox(); //Sandbox is of type Application as it inherits from it
}