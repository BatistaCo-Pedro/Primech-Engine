#pragma once

#include "Primech/Core/LayerStack/Layer.h"
#include "Primech/Events/ApplicationEvent.h"
#include "Primech/Events/KeyEvent.h"
#include "Primech/Events/MouseEvent.h"
#include "Primech/Core/Core.h"

namespace PriMech {
	class PRIMECH_API ImGuiLayer : public Layer{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:

		float time_ = 0.0f;
	};
}

