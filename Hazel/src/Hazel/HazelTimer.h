#pragma once
#include <chrono>

namespace Hazel {
	class HazelTimer {
	public:
		HazelTimer() noexcept;
		float Mark() noexcept;
		float Peek() noexcept;

	private:
		std::chrono::steady_clock::time_point last;
	};
}