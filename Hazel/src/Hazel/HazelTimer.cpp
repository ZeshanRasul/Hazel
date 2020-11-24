#include "hzpch.h"
#include "HazelTimer.h"

namespace Hazel {
	
	HazelTimer::HazelTimer() noexcept
	{
		last = std::chrono::steady_clock::now();
	}

	float HazelTimer::Mark() noexcept
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> frameTime = last - old;
		return frameTime.count();
	}

	float HazelTimer::Peek() noexcept
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
	}
}