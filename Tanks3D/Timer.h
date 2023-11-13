#pragma once

#include <chrono>
#include <vector>
#include <fstream>

struct Timer
{
public:
	Timer() = default;
	Timer(int numTimes, int skipFrames = 0);

	void start();
	void end();

	// Timer
public:
	template<typename Timescale = std::chrono::duration<float, std::milli>>
	float get_elapsed_time() { return (float)std::chrono::duration_cast<Timescale>(m_End - m_Start).count(); }

	// Average Timer Functions
public:
	template<typename Timescale = std::chrono::duration<float, std::milli>>
	float get_average_time()
	{
		if (!get_are_all_times_collected()) return 0;

		float elapsedTime = 0;

		for (int i = 0; i < m_ElapsedTimes.size(); i++)
		{
			elapsedTime += std::chrono::duration_cast<Timescale>(m_ElapsedTimes[i]).count();
		}

		elapsedTime /= m_ElapsedTimes.size();
		return elapsedTime;
	}

	template<typename Timescale = std::chrono::duration<float, std::milli>>
	float get_max_time()
	{
		std::vector<float> elapsedTimes = get_elapsed_times_vector<Timescale>();

		return *max_element(std::begin(elapsedTimes), std::end(elapsedTimes));
	}

	template<typename Timescale = std::chrono::duration<float, std::milli>>
	float get_min_time()
	{
		std::vector<float> elapsedTimes = get_elapsed_times_vector<Timescale>();

		return *min_element(std::begin(elapsedTimes), std::end(elapsedTimes));
	}

	bool get_are_all_times_collected() const;

private:
	template<typename Timescale = std::chrono::duration<float, std::milli>>
	std::vector<float> get_elapsed_times_vector()
	{
		if (!get_are_all_times_collected()) return std::vector<float>{0};

		std::vector<float> elapsedTimes;

		for (int i = 0; i < m_ElapsedTimes.size(); i++)
		{
			float time = (float)std::chrono::duration_cast<Timescale>(m_ElapsedTimes[i]).count();
			elapsedTimes.push_back(time);
		}

		return elapsedTimes;
	}

	// Save to file
public:
	template<typename Timescale = std::chrono::duration<float, std::milli>>
	void print_to_file(std::string file_name = "average_time", std::string unit = "")
	{
		if (!get_are_all_times_collected()) return;

		std::ofstream file;
		file.open(file_name + ".csv");

		file << "frame,time\n";

		for (int i = 0; i < m_ElapsedTimes.size(); i++)
		{
			file << i << "," << std::chrono::duration_cast<Timescale>(m_ElapsedTimes[i]).count() << unit << "\n";
		}

		file.close();
	}

	// Shared Variables
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start, m_End;

	// Average Timer Variables
private:
	bool isAverage = false;
	int const m_NumTimes = 0, m_SkipFrames = 0;
	int m_FramesSkipped = 0;
	std::vector<std::chrono::nanoseconds> m_ElapsedTimes;
};