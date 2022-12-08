#pragma once

#include <Primech/LayerStack/Layer.h>

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
		float time_ = 0.0f;
	};
}

