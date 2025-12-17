#include "ExitMgmt.h"

#include "Core/Application.h"

#ifdef HS_PLATFORM_LINUX
	#include <unistd.h>
#endif // HS_PLATFORM_LINUX

namespace HSFramework
{
	ExitRegistry::ExitRegistry()
	{
	}

	ExitRegistry::~ExitRegistry()
	{
	}

	bool ExitRegistry::Register(ExitCode code, const ExitInfo& info)
	{
		if (m_Frozen || IsRegistered(code))
		{
			return false;
		}
		m_Registry[code] = info;
		return true;
	}

	bool ExitRegistry::Unregister(ExitCode code)
	{
		if (m_Frozen || !IsRegistered(code))
		{
			return false;
		}
		m_Registry.erase(code);
		return true;
	}

	void ExitRegistry::Freeze()
	{
		m_Frozen = true;
	}

	const ExitInfo& ExitRegistry::GetInfo(ExitCode code) const
	{
		return IsRegistered(code) ? m_Registry.at(code) : INFO_NONE;
	}

	ExitMgmt::ExitMgmt()
	{
		m_Registry.Register(EC_UNSPECIFIED, "EC_UNSPECIFIED", "No information about this exit was provided.");
		m_Registry.Register(EC_SUCCESSFUL_EXECUTION, "EC_SUCCESSFUL_EXECUTION", "The application has completed its functions during its lifetime successfully.");
		m_Registry.Register(EC_GENERIC_FAILURE, "EC_GENERIC_FAILURE", "Something has went wrong with the program that wasn't explicitly declared.");
		m_Registry.Register(EC_INVALID_SYSTEM_CONFIGURATION, "EC_INVALID_SYSTEM_CONFIGURATION", "The current system configuration is incompatible with this program.");
		m_Registry.Register(EC_INVALID_ENVIRONMENT, "EC_INVALID_ENVIRONMENT", "The current environment is incompatible with this program.");
		m_Registry.Register(EC_INCAPABLE_PROCESSOR, "EC_INCAPABLE_PROCESSOR", "The CPU (Central Processing Unit/Processor) on this machine lacks the attributes that this program requires.");
		m_Registry.Register(EC_ASSERTION_FAILURE, "EC_ASSERTION_FAILURE", "A debug assertion has failed, refer to the log file for more information.");
		m_Registry.Register(EC_VERIFICATION_FAILURE, "EC_VERIFICATION_FAILURE", "A runtime verification/assertion has failed, refer to the log file for more information.");
		m_Registry.Register(EC_FATAL_LOG_RECORDED, "EC_FATAL_LOG_RECORDED", "A fatal log was recorded, refer to the log file for more information.");
		m_Registry.Register(EC_UNSUPPORTED_RENDERER, "EC_UNSUPPORTED_RENDERER", "The renderer (video card/GPU) on this machine is lacks the features needed to run thus program.");
		m_Registry.Register(EC_OUT_OF_MEMORY, "EC_OUT_OF_MEMORY", "The system has run out of memory.");
		m_Registry.Register(EC_ALLOCATION_FAILURE, "EC_ALLOCATION_FAILURE", "A memory allocation has failed, possibly OOM.");
		m_Registry.Register(EC_THIRD_PARTY_LIBRARY_ERROR, "EC_THIRD_PARTY_LIBRARY_ERROR", "An error associated with a third party library this program uses has occurred.");
		m_Registry.Register(EC_DEVICE_LOST, "EC_DEVICE_LOST", "A system device that the program was currently making use of has been lost, possibly via physical ejection or hardware failure.");

		if (s_RegistrySetupEnvironment)
		{
			s_RegistrySetupEnvironment(m_Registry);
		}
		m_Registry.Freeze();

		if (std::atexit(AtExit) == 0)
		{
			m_RegisteredAtExit = true;
		}
	}
	
	void ExitMgmt::ImmediateExit(ExitCode exitcode)
	{
		if (m_ExitCommitted.exchange(true, std::memory_order_acq_rel))
		{
			// If a thread has already called ImmediateExit
			for (;;)
			{
				std::this_thread::yield();
			}
		}



		// exit logic...
	}

	void ExitMgmt::RequestExit(ExitCode exitcode, bool fatal)
	{
	}

	void ExitMgmt::EmitExitNotification(ExitCode exitcode, bool treatAsFatal)
	{
	}

	void ExitMgmt::Record(ExitCode exitcode, bool fatal)
	{
		const ExitInfo& info = m_Registry.IsRegistered(exitcode) ? m_Registry.GetInfo(exitcode) : m_Registry.GetInfo(EC_UNSPECIFIED);
		m_Records.emplace_back(exitcode, info, Timestamp(), fatal);

		if (m_RecordIndexPrimaryFatal == size_t(-1) && fatal)
		{
			m_RecordIndexPrimaryFatal = m_Records.size() - 1;
		}
	}

	void ExitMgmt::AtExit(void)
	{
	}

}
