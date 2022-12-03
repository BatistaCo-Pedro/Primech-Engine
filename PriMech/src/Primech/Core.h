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
