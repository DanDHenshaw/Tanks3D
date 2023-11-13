#include "Timer.h"

Timer::Timer(int numTimes, int skipFrames) : isAverage(true), m_NumTimes(numTimes), m_SkipFrames(skipFrames)
{
	m_ElapsedTimes.reserve(m_NumTimes);
}

void Timer::start()
{
	if (m_FramesSkipped != m_SkipFrames) return;

	m_Start = std::chrono::high_resolution_clock::now();
}

void Timer::end()
{
	if (m_FramesSkipped != m_SkipFrames) { m_FramesSkipped++; return; }

	m_End = std::chrono::high_resolution_clock::now();

	if (!isAverage) return;

	if (m_ElapsedTimes.size() < m_NumTimes && !get_are_all_times_collected())
	{
		m_ElapsedTimes.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(m_End - m_Start));
	}
}

bool Timer::get_are_all_times_collected() const
{
	return m_ElapsedTimes.size() == m_NumTimes;
}