#pragma once

#include "Core/Application.h"

namespace HSFramework::Runtime
{
	class RuntimeApplication : public Application
	{
	public:
		RuntimeApplication(const ApplicationInfo& info);
		virtual ~RuntimeApplication() override;
	};
}
