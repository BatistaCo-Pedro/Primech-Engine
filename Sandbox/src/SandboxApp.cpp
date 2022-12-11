#include <Primech.h>

class ExampleLayer : public PriMech::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {
		//PM_INFO("ExampleLayer::Update");

		if (PriMech::Input::IsKeyPressed(PM_KEY_TAB)) {
			PM_WARN("Tab is pressed (update)");
		}
	}

	void OnEvent(PriMech::Event& event) override {
		if (event.GetEventType() == PriMech::EventType::KeyPressed) {
			PriMech::KeyPressedEvent& e = (PriMech::KeyPressedEvent&)event;

			if (PriMech::Input::IsKeyPressed(PM_KEY_TAB)) {
				PM_WARN("Tab is pressed (event)");
			}
			PM_INFO("{0}", (char)e.GetKeyCode());		
		}
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