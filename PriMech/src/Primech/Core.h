#pragma once
#include <memory>

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

	template<typename T>
	using Ref = std::shared_ptr<T>;
}