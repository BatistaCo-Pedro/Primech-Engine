#pragma once

#include "ppch.h"

#include "Primech/Core.h"
#include "PriMech/Events/Event.h"

namespace PriMech {
	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& titleIn = "PriMech Engine",
					unsigned int heightIn = 720,
					unsigned int widthIn = 1280)
			: title(titleIn), width(widthIn), height(heightIn) {}
	};

	//Interface representing desktop system based Windows
	class PRIMECH_API Window {
	public:
		using EventCallbackFunction = std::function<void(Event&)>; 

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}