#pragma once

#include "../Core.h"

#include <string>
#include <functional>

namespace PriMech {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPress, KeyReleased,
		MouseButtonpressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};
}