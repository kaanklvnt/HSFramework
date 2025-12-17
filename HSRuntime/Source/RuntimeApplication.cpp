#include "RuntimeApplication.h"

// ======================================================================== //
// >> Entry Point Function Declared & Implemented Here <<
#pragma region EntryPoint
	#if defined HS_SHIPPING && defined HS_PLATFORM_WINDOWS
		#define HS_USE_NATIVE_ENTRY_POINT
	#endif
	#include "Core/EntryPoint.h"
#pragma endregion EntryPoint
// >> Entry Point Function Declared & Implemented Here <<
// ======================================================================== //

namespace HSFramework
{
	namespace Runtime
	{
		static void ExitRegistrySetupEnvironment(ExitRegistry& registry)
		{

		}
	}
	Application* CreateApplication(const CommandLine& cmdline)
	{
		ApplicationInfo info;
		info.Name = "HSFramework Runtime";
		info.WorkingDir = ".";
		info.CmdLine = cmdline;
		info.ExitRegistrySetupEnvironment = Runtime::ExitRegistrySetupEnvironment;

		return new Runtime::RuntimeApplication(info);
	}
}

namespace HSFramework::Runtime
{
	RuntimeApplication::RuntimeApplication(const ApplicationInfo& info)
		: Application(info)
	{
	}

	RuntimeApplication::~RuntimeApplication()
	{
	}
}
