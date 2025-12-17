#pragma once

#include "Misc/Timestamp.h"

#include <atomic>
#include <vector>
#include <mutex>
#include <map>
#include <new>

namespace HSFramework
{
	/**
	 * @brief Standardized exit codes by the HSFramework.
	 * Not a strong type because the client codes would need to be casted all over the place.
	 * Also raw enum with prefix look cooler.
	 * Any unregistered code will be turned to EC_UNSPECIFIED by ExitMgmt anyway.
	 * 
	 * Most OSes do not a signed int32 for exitcodes. So, these are supposed to be inhouse codes for the framework and client ABI.
	 * ExitMgmt will provide code 0 for EC_SUCCESSFUL_EXECUTION to the OS and code 1 for any other int32 code. These values are internal.
	 * No other code than 0 and 1 will be used when reporting to the OS.
	 * 
	 * int is used as most interfaces use int to cover exitcode fronts.
	 */
	typedef int ExitCode;
	enum : ExitCode
	{
		// Exit code was unspecified, used as an internal ExitCode state.
		EC_UNSPECIFIED                  = -1,

		EC_SUCCESSFUL_EXECUTION         =  0,
		EC_GENERIC_FAILURE              =  1,
		EC_INVALID_SYSTEM_CONFIGURATION =  2,
		EC_INVALID_ENVIRONMENT          =  3,
		EC_INCAPABLE_PROCESSOR          =  4,
		EC_ASSERTION_FAILURE            =  5,
		EC_VERIFICATION_FAILURE         =  6,
		EC_FATAL_LOG_RECORDED           =  7,
		EC_UNSUPPORTED_RENDERER         =  8,
		EC_OUT_OF_MEMORY                =  9,
		EC_ALLOCATION_FAILURE           = 10,
		EC_THIRD_PARTY_LIBRARY_ERROR    = 11,
		EC_DEVICE_LOST                  = 12,

		// Every exit code in range [0, 0xFFFF) and also specially -1 (which is EC_UNSPECIFIED) is reserved by the HSFramework.
		// Clients must start with this code specifically and increment from this as a base.
		// This itself is not reserved as depicted in the range above.
		// Range [0xFFFF, INT32_MAX] is free to any registeration by the client.
		EC_CLIENT_BASE                  = 0xFFFF
	};
	// Converts HSFramework exitcode to a basic 0 or 1 code for standard EXIT_SUCCESS or EXIT_FAILURE match.
	static int NormalizeExitCode(ExitCode exitcode) { return exitcode == EC_SUCCESSFUL_EXECUTION ? 0 : 1; }

	/**
	 * @brief Represents diagnostic information about an exitcode.
	 * Disclaimer: Make sure string fields are valid texts within the binary itself and not runtime strings.
	 * This is unenforcable by language standard so I'll scream: MAKE SURE STRINGS ARE IN THE BINARY.
	 */
	struct ExitInfo
	{
		ExitCode    Code = EC_UNSPECIFIED; // Code associated with this ExitInfo.
		const char* Name = "";             // Enum name or just name of the ExitCode, like EC_SUCCESSFUL_EXECUTION.
		const char* Descriptor = "";       // Long text describing what this kind of exit code implies.

		constexpr ExitInfo() = default;
		constexpr ExitInfo(const ExitInfo&) = default;
		constexpr ExitInfo& operator=(const ExitInfo&) = default;

		// cool templated constructor to really incentivize compile time strings
		template <std::size_t N1, std::size_t N2>
		constexpr ExitInfo(ExitCode code, const char (&name)[N1], const char (&descriptor)[N2])
			: Code(code), Name(name), Descriptor(descriptor) { }

		constexpr bool operator<(const ExitInfo& other) const
		{
			return Code < other.Code;
		}
		constexpr bool operator==(const ExitInfo& other) const
		{
			// Name & descriptor should not matter for an ExitInfo associated with a unique ExitCode.
			// If that is the issue then it is kind of on you, you kinda fucked up somewhere.
			return Code == other.Code;
		}
	};

	/**
	 * @brief Represents an exit request made to ExitMgmt.
	 */
	struct ExitRecord
	{
		ExitCode  Code = EC_UNSPECIFIED;
		ExitInfo  Info; // ExitInfo associated with the ExitCode within an ExitRegistry.
		Timestamp TsFlagged;

		// Whether or not this exit cause was fatal.
		// The first fatal ExitRecord always wins the primary exit status.
		// Other ones are notified to the end user but the first fatal is always the primary.
		bool      Fatal = false;

		ExitRecord(ExitCode code, const ExitInfo& info, const Timestamp& tsFlagged, bool fatal)
			: Code(code), Info(info), TsFlagged(tsFlagged), Fatal(fatal) { }
	};

	class ExitRegistry
	{
	public:
		inline static const ExitInfo INFO_NONE = { EC_UNSPECIFIED, "((UNSPECIFIED_EXIT_CODE))", "Given exitcode is not registered within the ExitRegistry." };
	public:
		ExitRegistry();
		~ExitRegistry();

		bool Register(ExitCode code, const ExitInfo& info);
		template <std::size_t N1, std::size_t N2>
		bool Register(ExitCode code, const char(&name)[N1], const char(&descriptor)[N2])
		{
			return Register(code, { code, name, descriptor });
		}
		bool Unregister(ExitCode code);

		// Freezes the registry forever, no more registeration or unregisteration.
		void Freeze();

		// Returns registered info if IsRegistered(code) is true, INFO_NONE otherwise (including never registered or previously unregistered).
		const ExitInfo& GetInfo(ExitCode code) const;
		bool IsRegistered(ExitCode code) const { return m_Registry.contains(code); }
		bool IsFrozen() const { return m_Frozen; }
	private:
		std::map<ExitCode, ExitInfo> m_Registry;
		bool m_Frozen = false;
	};

	class ExitMgmt
	{
	private:
		inline static void(*s_RegistrySetupEnvironment)(ExitRegistry& registry);
		// Initializes ExitRegistry with standard codes, calls s_RegistrySetupEnvironment (if set) and freezes registry.
		// Also attempts setting up atexit.
		ExitMgmt();
	public:
		// Not copy/move constructible
		ExitMgmt(const ExitMgmt&) = delete;
		// Not copy/move assignable
		ExitMgmt& operator=(const ExitMgmt&) = delete;

		/**
		 * @brief Creates an ExitRecord of the given code and IMMEDIATELY exits from the program.
		 * Given code is considered fatal as an immediate exit is requested and will be pushed as a fatal record.
		 */
		[[noreturn]] void ImmediateExit(ExitCode exitcode);

		/**
		 * @brief Creates an ExitRecord of the given code and ensures the next Application loop tick doesn't happen and program cleans up properly to exit properly.
		 * Essentially this just records and calls Application::Get()->Quit(). If no Application instance is found, neither the record nor Quit is made.
		 * @param exitcode The exitcode to record.
		 * @param fatal Whether or not the requested exit is a fatal cause. If so, the first-fatal record always wins the primary code.
		 */
		void RequestExit(ExitCode exitcode, bool fatal = false);

		/**
		 * @brief End user facing function that notifies the end user about an exitcode.
		 * What it does depends on the build configuration too.
		 * It always logs it to the stdout logger and file logger.
		 * If a shipping build, it also pops a MessageDialog about it (except for EC_SUCCESSFUL_EXECUTION where it won't.)
		 * 
		 * This should not be used directly as ExitMgmt deals with this using AtExit(), however it might be necessary because
		 * atexit() is prone to failing and if IsRegisteredAtExit() is false, EntryPoint() will manually call this at its end,
		 * so will the ExitMgmt with the ImmediateExit() function.
		 */
		void EmitExitNotification(ExitCode exitcode, bool treatAsFatal);

		// Gets exit code info registry.
		const ExitRegistry& GetRegistry() const { return m_Registry; }

		// Sets the function to call after ExitMgmt construction where access to registry is available.
		// This is complemented by the ApplicationSpec supplied to the EntryPoint() before the Application instance is constructed.
		// ApplicationSpec contains a function pointer equivalent of this that Application will route properly.
		// This way if ExitMgmt is used before Application construction, standard codes are still registered through first Get() call.
		// If everything goes okay eventually client codes will be registered through first Get() call on Application.
		static void SetRegistrySetupEnvironment(void(*environment)(ExitRegistry&))
		{
			// TODO: Assert !Get()
			s_RegistrySetupEnvironment = environment;
		}

		static ExitMgmt* Get()
		{
			// This ExitMgmt will be never destructed so we can be certain that in an atexit() environment
			// our unordered_map and ExitRegistry and others will exist as expected.
			alignas(ExitMgmt) static std::byte storage[sizeof(ExitMgmt)];
			static ExitMgmt* instance = new (storage) ExitMgmt();

			// Fraud function alert
			return std::launder(instance); // Tf is this function we laundering money or shi? Thanks compiler.
		}
	private:
		void Record(ExitCode exitcode, bool fatal);

		// This function is for best wishes, we don't rely on it but it is another good thing we can keep track of.
		// If some random code uses std::exit for some reason to bypass our entire central management system we can intercept it (not really, just be notified).
		// So we can still present some sort of output.
		static void AtExit(void);
	private:
		std::vector<ExitRecord> m_Records;
		size_t m_RecordIndexPrimaryFatal = size_t(-1); // Record doomed to be chosen, first-fatal record.

		// Thread safety gurus
		std::mutex m_Mutex;
		std::atomic<bool> m_ExitCommitted = false;

		ExitRegistry m_Registry;

		// Whether or not the atexit() call was successful and AtExit of this class will be called.
		// We do not rely on atexit() single handedly as both EntryPoint() and ImmediateExit()
		// call their own versions if atexit() had failed, this mainly exists to capture abrupt std::exit() calls.
		bool m_RegisteredAtExit = false;
	};
}
