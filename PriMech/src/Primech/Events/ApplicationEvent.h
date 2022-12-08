#pragma once

#include "Event.h"

namespace PriMech {

	class PRIMECH_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : width_(width), height_(height) {}
		
		inline unsigned int GetWidth() { return width_; }
		inline unsigned int GetHeight() { return height_; }

		//Method used to Log information to the console
		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width_ << "," << height_;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(WindowResize);
	private:
		unsigned int width_, height_;
	};
	
	
	class PRIMECH_API AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(WindowResize);
	};
	
	
	class PRIMECH_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppTick);
	};
	
	
	class PRIMECH_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppUpdate);
	};
	

	class PRIMECH_API AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppRender);
	};


}