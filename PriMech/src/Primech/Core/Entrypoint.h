#pragma once

#ifdef PM_PLATFORM_WINDOWS

extern PriMech::Application* PriMech::CreateApplication();

int main(int argc, char** argv) {
	PriMech::Log::Init();

	PM_PROFILE_BEGIN_SESSION("Startup", "PriMechProfile-Startup.json");
	//Calls extern method and Application constructor
	PriMech::Application* app = PriMech::CreateApplication(); 
	PM_PROFILE_END_SESSION();

	PM_PROFILE_BEGIN_SESSION("Runtime", "PriMechProfile-Runtime.json");
	app->Run();
	PM_PROFILE_END_SESSION();

	PM_PROFILE_BEGIN_SESSION("Shutdown", "PriMechProfile-Shutdown.json");
	delete app;
	PM_PROFILE_END_SESSION();
}

#else 
	#error Primech only supports windows!
#endif