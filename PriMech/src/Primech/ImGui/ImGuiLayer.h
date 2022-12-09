#pragma once

#include <Primech/LayerStack/Layer.h>
#include <Primech/Events/ApplicationEvent.h>
#include <Primech/Events/KeyEvent.h>
#include <Primech/Events/MouseEvent.h>
#include <Primech/Core.h>

namespace PriMech {
	class PRIMECH_API ImGuiLayer : public Layer{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);

		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);

		bool OnWindowResizeEvent(WindowResizeEvent& event);

		float time_ = 0.0f;
	};
}

