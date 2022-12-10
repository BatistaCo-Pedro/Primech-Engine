#pragma once

#include "PriMech/Core.h"
#include "ppch.h"

namespace PriMech {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0), 
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse= BIT(3),
		EventCategoryMouseButton= BIT(4),
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class PRIMECH_API Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;  //check the current category for the searched one
		}

		inline bool IsHandled() { return m_Handled; }
	protected:
		bool m_Handled = false;
	};

	class EventDispatcher {		
		//returns bool and takes in T reference
		//T can be any Event type
		template<typename T>
		using EventFunction = std::function<bool(T&)>; 
	public:
		EventDispatcher(Event& event) : event_(event) {}

		template<typename T>
		bool Dispatch(EventFunction<T> func) {
			//check if eventtype passed to constructor is equal to the eventtype of the function<T>
			if (event_.GetEventType() == T::GetStaticType()) {
				//call function to determine if event was handled
				event_.m_Handled = func(*(T*)&event_);
				return true;
			}
			return false;
		}
	private:
		Event& event_;
	};

	//used in the Logging library to call ToString() 
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}