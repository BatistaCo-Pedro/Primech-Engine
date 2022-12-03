#pragma once

#ifdef PM_PLATFORM_WINDOWS

extern PriMech::Application* PriMech::CreateApplication();

int main(int argc, char** argv) {
	PriMech::Log::Init();
	PM_CORE_WARN("Initalized ClientLog");
	int a = 5;
	PM_INFO("App var = {0}", a);

	auto app = PriMech::CreateApplication();
	app->Run();
	delete app;
}

#else 
	#error Primech only supports windows!
#endif