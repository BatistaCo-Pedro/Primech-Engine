#pragma once
#include "Primech/Window.h"
#include <Primech/Renderer/GraphicsContext.h>

#include "GLFW/glfw3.h"

namespace PriMech {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetHeight() const override { return wData_.height_; }
		inline unsigned int GetWidth() const override { return wData_.width_; }
		inline virtual void* GetNativeWindow() const { return window_; }

		inline void SetEventCallback(const EventCallbackFunction& callback) override { wData_.EventCallback = callback; };
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

		GLFWwindow* window_;
		GraphicsContext* context_;

		struct WindowData {
			std::string title_;
			unsigned int width_, height_;
			bool VSync;

			EventCallbackFunction EventCallback;
		};
		WindowData wData_;
	};
}