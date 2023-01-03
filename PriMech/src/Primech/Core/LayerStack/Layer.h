#pragma once
#include "PriMech/Core/Core.h"
#include "Primech/Events/Event.h"
#include "Primech/Core/Timestep.h"

namespace PriMech {

	class PRIMECH_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() { return m_debugName; }
	protected:
		std::string m_debugName;
	};
}