#pragma once

#ifdef PM_PLATFORM_WINDOWS

extern PriMech::Application* PriMech::CreateApplication();

int main(int argc, char** argv) {
	printf("Started Engine\n");
	auto app = PriMech::CreateApplication();
	app->Run();
	delete app;
}

#else 
	#error Primech only supports windows!
#endif