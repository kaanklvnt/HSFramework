#pragma once

#include <string_view>

namespace HSFramework
{
	struct CommandLine
	{
		static constexpr int INDEX_NONE = -1;

		char** List = nullptr;
		int    Size = 0;

		CommandLine() = default;
		CommandLine(const CommandLine&) = default;
		CommandLine& operator=(const CommandLine&) = default;
		CommandLine(char** list, int size);

		int Find(std::string_view arg) const;
		int Count(std::string_view arg) const;
		bool Contains(std::string_view arg) const;
	};
}
