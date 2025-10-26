#pragma once

#include <chrono>
#include <GameEngine/Core/Log.h>

namespace GameEngine {

	class Timer
	{
	public:
		Timer()
			: m_Name(""), m_StartTimepoint(std::chrono::high_resolution_clock::now()) {}
		Timer(const std::string& name)
			: m_Name(name), m_StartTimepoint(std::chrono::high_resolution_clock::now()) {}

		~Timer()
		{
			Stop();
		}

		void Stop()
		{
			if (m_Stopped)
				return;

			auto endTimepoint = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimepoint - m_StartTimepoint);

			long long us = duration.count();
			double ms = us * 0.001;

			if (m_Name.empty())
				GE_CORE_DEBUG("Timer: {} us ({:.3f} ms)", us, ms);
			else
				GE_CORE_DEBUG("Timer [{}]: {} us ({:.3f} ms)", m_Name, us, ms);

			m_Stopped = true;
		}
	private:
		bool m_Stopped = false;
		std::string m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	};

}