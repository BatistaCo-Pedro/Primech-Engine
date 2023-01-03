#pragma once

#include "PriMech/Core/Core.h"

namespace PriMech {
	//static class to be used as global Input Polling
	class PRIMECH_API Input {
	public:
		inline static bool IsKeyPressed(int keycode) { return inputInstance_->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return inputInstance_->IsMouseButtonPressedImpl(button); }

		inline static std::pair<float, float> GetMousePos() { return inputInstance_->GetMousePosImpl(); }
		inline static float GetMouseX() { return inputInstance_->GetMouseXImpl(); }
		inline static float GetMouseY() { return inputInstance_->GetMouseYImpl(); }	
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;

		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Scope<Input> inputInstance_;
	};
}