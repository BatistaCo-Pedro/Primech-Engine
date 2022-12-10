#pragma once

#ifdef PM_PLATFORM_WINDOWS

extern PriMech::Application* PriMech::CreateApplication();

int main(int argc, char** argv) {
	//Initalize Logger
	PriMech::Log::Init();
	PM_CORE_WARN("Initalized CoreLog"); //Log something as test

	//Calls extern method and Application constructor
	PriMech::Application* app = PriMech::CreateApplication(); 
	app->Run();
	delete app;
}

#else 
	#error Primech only supports windows!
#endif