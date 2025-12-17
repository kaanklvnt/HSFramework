#pragma once

#include "Core/ExitMgmt.h"
#include "Misc/CommandLine.h"

#include <string>

namespace HSFramework
{
	struct ApplicationInfo
	{
		std::string Name;
		std::string WorkingDir;
		CommandLine CmdLine;
		void (*ExitRegistrySetupEnvironment)(ExitRegistry& registry) = nullptr;
	};

	class Application
	{
	public:
		Application(const ApplicationInfo& info);
		virtual ~Application();

		void Run();
		void Quit();

		const ApplicationInfo& GetInfo() const { return m_Info; }
		const CommandLine& GetCommandLine() const { return m_Info.CmdLine; }
		bool IsRunning() const { return m_Running; }

		static Application* Get() { return s_Instance; }
	private:
		void CleanUp();
	private:
		ApplicationInfo m_Info;
		bool m_Running = false;
	private:
		inline static Application* s_Instance = nullptr;
	};

	// Must be provided by the client constructing its own implementation of Application.
	Application* CreateApplication(const CommandLine& cmdline);
}
