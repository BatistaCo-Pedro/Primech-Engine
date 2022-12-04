#pragma once

#ifdef PM_PLATFORM_WINDOWS

extern PriMech::Application* PriMech::CreateApplication();

int main(int argc, char** argv) {
	PriMech::Log::Init();
	PM_CORE_WARN("Initalized CoreLog");

	//calls extern method and Application constructor
	auto app = PriMech::CreateApplication(); 
	app->Run();
	delete app;
}

#else 
	#error Primech only supports windows!
#endif