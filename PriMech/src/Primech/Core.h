#pragma once

#ifdef PM_PLATFORM_WINDOWS
	#ifdef PRIMECH_BUILD_DLL
		#define PRIMECH_API __declspec(dllexport)
	#else
		#define PRIMECH_API __declspec(dllimport)
	#endif // DEBUG
#else
	#error PriMech only supports windows!
#endif 

#ifdef PM_ENABLE_ASSERTS
	#define PM_ASSERT(x, ...)		if(!(x)) { PM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }
	#define PM_CORE_ASSERT)(x, ...)	if(!(X)) { PM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }
#else
	#define PM_ASSERT(x, ...)
	#define PM_CORE_ASSERT(x, ...)
#endif // PM_ENABLE_ASSERTS


#define BIT(x) (1 << x)