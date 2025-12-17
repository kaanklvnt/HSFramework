#pragma once

#include "Core/Application.h"

namespace HSFramework
{
	int EntryPoint(int argc, char** argv)
	{
		Application* application = CreateApplication({ argv, argc });
		application->Run();

		delete application;
		return 0;
	}
}

#ifdef HS_USE_NATIVE_ENTRY_POINT

#include <Windows.h>

int CALLBACK WinMain
(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow
)
{
	return HSFramework::EntryPoint(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return HSFramework::EntryPoint(argc, argv);
}

#endif
