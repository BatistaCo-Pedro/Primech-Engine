#pragma once

#include "Event.h"
#include <sstream>

namespace PriMech {

	class PRIMECH_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : mousePosX(x), mousePosY(y) {}
		
		inline float GetPosX() const { return mousePosX; }
		inline float GetPosY() const { return mousePosY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mousePosX << "," << mousePosY;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
		EVENT_CLASS_TYPE(MouseMoved)
	private:
		float mousePosX, mousePosY;
	};


	class PRIMECH_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : xOffset_(xOffset), yOffset_(yOffset) {}
	
		inline float GetOffsetX() const { return xOffset_; }
		inline float GetOffsetY() const { return yOffset_; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << xOffset_ << "," << yOffset_;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		float xOffset_, yOffset_;
	};


	class PRIMECH_API MouseButtonEvent : public Event {
	public:
		inline int GetMouseButton() const { return button_; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
	protected:
		MouseButtonEvent(int button) : button_(button) {}
		int button_;
	};


	class PRIMECH_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << button_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	private:

	};


	class PRIMECH_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << button_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	private:

	};
}