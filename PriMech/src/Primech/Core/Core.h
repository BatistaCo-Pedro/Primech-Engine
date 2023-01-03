#pragma once
#include <memory>

//Platform detection using predefined macros
#ifdef _WIN32
	#ifdef _WIN64
		#define PM_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported!"
	#endif 
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define HZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define HZ_PLATFORM_MACOS
		#error "MacOS is not supported!"
#else
	#error "Unknown Apple platform!"
#endif

#elif defined(__ANDROID__)
	#define HZ_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define HZ_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

#ifdef PM_PLATFORM_WINDOWS
	#if PM_DYNAMIC_LINK
		#ifdef PRIMECH_BUILD_DLL
			#define PRIMECH_API __declspec(dllexport)
		#else
			#define PRIMECH_API __declspec(dllimport)
		#endif // DEBUG
	#else
		#define PRIMECH_API 
	#endif
#else
	#error PriMech only supports windows!
#endif

#ifdef PM_DEBUG
	#define PM_ENABLE_ASSERTS
#endif


#ifdef PM_ENABLE_ASSERTS
	#define PM_ASSERT(x, ...)		if(!(x)) { PM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }
	#define PM_CORE_ASSERT(x, ...)	if(!(x)) { PM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }
#else
	#define PM_ASSERT(x, ...)
	#define PM_CORE_ASSERT(x, ...)
#endif // PM_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define PM_BIND_EVENT_FUNCTION(function)	std::bind(&function, this, std::placeholders::_1)

namespace PriMech {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ...args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}