

#if __EMSCRIPTEN__
#include <emscripten.h>
#endif


//------------------------------------------------------------------------------
inline double TimeUtility::GetTime()
{
#if __EMSCRIPTEN__
	return emscripten_get_now();
#else
	return TimeUtility::TicksToSeconds(SystemClock::GetTickCounter());
#endif
}

//------------------------------------------------------------------------------
inline void TimeUtility::Wait(double seconds)
{
	PerformanceStopWatch timer;

	timer.Start();
	while (timer.GetElapsedSeconds() < seconds){}
} // ()

//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
inline PerformanceStopWatch::PerformanceStopWatch()
{
	Reset();
	m_running = false;
}

inline PerformanceStopWatch::PerformanceStopWatch(int t)
{
	Restart();
}

//------------------------------------------------------------------------------
inline void PerformanceStopWatch::Reset()
{
	m_startTime_ticks = 0;
	m_totalTime_ticks = 0;
	m_splitTime_ticks = 0;
	m_totalSplitTime_ticks = 0;
}

//------------------------------------------------------------------------------
inline void PerformanceStopWatch::Restart()
{
	Reset();
	Start();
}

//------------------------------------------------------------------------------
inline void PerformanceStopWatch::Start()
{
	m_startTime_ticks = SystemClock::GetTickCounter();
	m_splitTime_ticks = m_startTime_ticks;
	m_running = true;
}

//------------------------------------------------------------------------------
inline void PerformanceStopWatch::Stop()
{
	uint64 now = SystemClock::GetTickCounter();
	m_totalTime_ticks += now - m_startTime_ticks;
	m_totalSplitTime_ticks += now - m_splitTime_ticks;
	m_running = false;
}

//------------------------------------------------------------------------------
inline uint64 PerformanceStopWatch::GetSplitTicks()
{
	uint64 t = m_totalSplitTime_ticks;

	if (m_running)
	{
		uint64 now = SystemClock::GetTickCounter();
		t += now - m_splitTime_ticks;
		m_splitTime_ticks = now;
	}

	m_totalSplitTime_ticks = 0;
	return t;
}

//------------------------------------------------------------------------------
inline uint64 PerformanceStopWatch::GetElapsedTicks()
{
	uint64 ticks = m_totalTime_ticks;

	if (m_running)
	{
		ticks += SystemClock::GetTickCounter() - m_startTime_ticks;
	}

	return ticks;
}

//------------------------------------------------------------------------------
inline double PerformanceStopWatch::GetSplitSeconds()
{
	return double(GetSplitTicks()) * SystemClock::GetInverseTicksPerSecond();
}

//------------------------------------------------------------------------------
inline double PerformanceStopWatch::GetElapsedSeconds()
{
	return double(GetElapsedTicks()) * SystemClock::GetInverseTicksPerSecond();
}

//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
inline LeanStopWatch::LeanStopWatch()
{
	m_startTime_ticks = 0;
	m_totalTime_ticks = 0;
	m_splitTime_ticks = 0;
	m_splits = 0;
	m_running = false;
} // LeanStopWatch()

//------------------------------------------------------------------------------
inline void LeanStopWatch::Start()
{
	m_startTime_ticks = SystemClock::GetTickCounter();
	m_splitTime_ticks = m_startTime_ticks;
	m_totalTime_ticks = 0;
	m_splits = 0;
	m_running = true;
} // Start()

//------------------------------------------------------------------------------
inline void LeanStopWatch::Stop()
{
	uint64 now = SystemClock::GetTickCounter();
	m_totalTime_ticks += now - m_startTime_ticks;
	m_running = false;
} // Stop()

//------------------------------------------------------------------------------
inline uint64 LeanStopWatch::Split()
{
	uint64 t = 0;

	if (m_running)
	{
		uint64 now = SystemClock::GetTickCounter();
		t += now - m_splitTime_ticks;
		m_totalTime_ticks += now - m_startTime_ticks;
		m_splitTime_ticks = now;
		m_splits++;
	}

	return t;
} // Split()

//------------------------------------------------------------------------------
inline uint64 LeanStopWatch::GetElapsedTicks()
{
	uint64 t = m_totalTime_ticks;

	if (m_running)
	{
		t += SystemClock::GetTickCounter() - m_startTime_ticks;
	}

	return t;
} // GetElapsedTicks()

//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
inline GameTimer::GameTimer(double idealUpdateTime, double fixedUpdateTime, double maxFPS)
{
	m_idealUpdateTime = idealUpdateTime;
	m_fixedUpdateTime = fixedUpdateTime;
	m_minimumSecsPerFrame = maxFPS;

	m_avgFrameIndex = 0;
	m_avgDeltaTime = 0.0;
	m_deltaTime = idealUpdateTime;
	m_accumulatedTime = 0.0;
} // GameTimer()

//------------------------------------------------------------------------------
inline void GameTimer::Start()
{
	m_engineTimer.Start();
} // Start()

//------------------------------------------------------------------------------
inline void GameTimer::Stop()
{
	m_engineTimer.Stop();
} // Stop()

//------------------------------------------------------------------------------
inline void GameTimer::StartFrame()
{
	m_frameTimer.Start();
	m_accumulatedTime += m_deltaTime;
} // StartFrame()

//------------------------------------------------------------------------------
inline double GameTimer::EndFrame()
{
	uint64 frameTicks = m_frameTimer.Split();

	m_frameAvgTicks -= m_frameTimes[m_avgFrameIndex];
	m_frameAvgTicks += frameTicks;
	m_frameTimes[m_avgFrameIndex] = frameTicks;
	m_avgDeltaTime = 0.0;

	if (++m_avgFrameIndex == FRAMESAMPLES)
	{
		m_avgFrameIndex = 0;
	}

	m_deltaTime = TimeUtility::TicksToSeconds(frameTicks);
	m_fps = (float)(1.0 / m_deltaTime);

	return m_deltaTime;
} // EndFrame()

inline void GameTimer::EndWithNoFrame()
{

}

//inline void GameTimer::EndWithNoFrame()
//{
//	uint64 frameTicks = m_frameTimer.Split();
//
//}

//------------------------------------------------------------------------------
inline double GameTimer::EndFrameAndHoldUntil(double minimumFrameTime)
{
	double clippedDT = EndFrame();

	if (clippedDT < minimumFrameTime)
	{
		TimeUtility::Wait(minimumFrameTime - clippedDT);
		clippedDT = minimumFrameTime;
	}

	return m_deltaTime = clippedDT;
} // HoldFrameUntil()

//------------------------------------------------------------------------------
inline double GameTimer::GetDeltaTime()
{
	return m_deltaTime;
} // GetDeltaTime()

//------------------------------------------------------------------------------
inline double GameTimer::GetElapsedTime()
{
	return TimeUtility::TicksToSeconds(m_engineTimer.GetElapsedTicks());
} // GetElapsedTime()

//------------------------------------------------------------------------------
inline double GameTimer::GetAverageDeltaTime()
{
	if (m_avgDeltaTime == 0.0)
		m_avgDeltaTime = double(TimeUtility::TicksToSeconds(m_frameAvgTicks / FRAMESAMPLES));

	return m_avgDeltaTime;
} // GetAverageDeltaTime()

//------------------------------------------------------------------------------
inline void GameTimer::FinishedSimulation()
{
	// Do other things?
	m_accumulatedTime -= m_fixedUpdateTime;
} // FinishedSimulation()

//------------------------------------------------------------------------------
inline bool GameTimer::SimulationNeedsUpdate()
{
	return (m_accumulatedTime >= m_fixedUpdateTime);
} // SimulationNeedsUpdate()

//------------------------------------------------------------------------------
inline double GameTimer::GetSimAccumulatedTime()
{
	return m_accumulatedTime;
} // GetSimAccumulatedTime()

//------------------------------------------------------------------------------
inline bool GameTimer::IsOkToRender()
{
	return (m_deltaTime >= m_minimumSecsPerFrame);
} // IsOkToRender()
