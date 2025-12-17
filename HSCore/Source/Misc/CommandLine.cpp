#include "CommandLine.h"

namespace HSFramework
{
	CommandLine::CommandLine(char** list, int size)
		: List(list), Size(size)
	{
	}

	int CommandLine::Find(std::string_view arg) const
	{
		for (int i = 1; i < Size; i++)
		{
			if (arg == List[i])
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	int CommandLine::Count(std::string_view arg) const
	{
		int counter{};

		for (int i = 1; i < Size; i++)
		{
			if (arg == List[i])
			{
				counter++;
			}
		}

		return counter;
	}

	bool CommandLine::Contains(std::string_view arg) const
	{
		return Find(arg) != INDEX_NONE;
	}
}
