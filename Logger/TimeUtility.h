#pragma once

#define FRAMESAMPLES 60

typedef unsigned long long uint64;

namespace SystemClock
{
	uint64 GetTickCounter();

	uint64 GetTicksPerSecond();
	uint64 GetTicksPerMilliSecond();
	uint64 GetTicksPerMicroSecond();
	uint64 GetTicksPerNanoSecond();

	double GetInverseTicksPerSecond();
	double GetInverseTicksPerMilliSecond();
	double GetInverseTicksPerMicroSecond();
	double GetInverseTicksPerNanoSecond();

	float GetInverseTicksPerSecondF();
	float GetInverseTicksPerMilliSecondF();
	float GetInverseTicksPerMicroSecondF();
	float GetInverseTicksPerNanoSecondF();
};

class PerformanceStopWatch;

namespace TimeUtility
{
	double GetTime();

	void Wait(double seconds);

	double TicksToSeconds(uint64 ticks);
	double TicksToMilliSeconds(uint64 ticks);
	double TicksToMicroSeconds(uint64 ticks);
	double TicksToNanoSeconds(uint64 ticks);

	float TicksToSecondsF(uint64 ticks);
	float TicksToMilliSecondsF(uint64 ticks);
	float TicksToMicroSecondsF(uint64 ticks);
	float TicksToNanoSecondsF(uint64 ticks);

	uint64 SecondsToTicks(double seconds);
	uint64 MilliSecondsToTicks(double ms);
	uint64 MicroSecondsToTicks(double mcs);
	uint64 NanoSecondsToTicks(double ns);

	uint64 SecondsToTicksF(float seconds);
	uint64 MilliSecondsToTicksF(float ms);
	uint64 MicroSecondsToTicksF(float mcs);
	uint64 NanoSecondsToTicksF(float ns);
};


/*
Stop Watch that is slightly more sophisticated
in time keeping and reusability.

Not Yet Complete.
*/
class PerformanceStopWatch
{
public:

	PerformanceStopWatch();
	PerformanceStopWatch(int t);

	void Reset();
	void Restart();
	void Start();
	void Stop();

	uint64 GetSplitTicks();
	uint64 GetElapsedTicks();

	double GetSplitSeconds();
	double GetElapsedSeconds();

private:

	uint64 m_startTime_ticks;	// The ticks at the start()
	uint64 m_splitTime_ticks;	// The ticks at the current split()
	uint64 m_totalTime_ticks;	// The total ticks between start() and stop()
	uint64 m_totalSplitTime_ticks;
	bool m_running;

};


/*
Lean Stop Watch that only handles straight away counting of time
and splitting it's current run.

start will reset the timer to that point in time.
stop will halt all counting and hold the state of the total
time recorded and last split time as well as the times it was
split.
*/
class LeanStopWatch
{
public:

	LeanStopWatch();

	void Start();
	void Stop();


	uint64 Split();
	uint64 GetElapsedTicks();

	uint64 GetSplitTicks() { return m_splitTime_ticks; }
	bool IsRunning() { return m_running; }
	uint64 NumSplits() { return m_splits; }

private:

	uint64 m_startTime_ticks;	// The ticks at the start()
	uint64 m_splitTime_ticks;	// The ticks at the current split()
	uint64 m_totalTime_ticks;	// The total ticks between start() and stop()
	uint64 m_splits;
	bool m_running;

};

class GameTimer
{
public:
	GameTimer(double idealUpdateTime, double fixedUpdateTime, double maxFPS);

	void Start();
	void Stop();

	void SetDeltaTime(double dt) { m_deltaTime = dt; }
	void SetAccumulatedTime(double a) { m_accumulatedTime = a; }

	void StartFrame();
	double EndFrame();
	void EndWithNoFrame();

	double EndFrameAndHoldUntil(double time);

	double GetDeltaTime();
	double GetElapsedTime();

	double GetFrameDeltaTime();
	double GetFrameElapsedTime();
	double GetEngineElapsedTime();

	double GetAverageDeltaTime();

	void FinishedSimulation();
	bool SimulationNeedsUpdate();
	double GetSimAccumulatedTime();
	bool IsOkToRender();

	float GetFramesPerSecond() { return m_fps; }
	int GetFrameNumber() { m_frameTimer.NumSplits(); }

private:

	float m_fps;
	LeanStopWatch m_engineTimer;
	LeanStopWatch m_frameTimer;
	uint64 m_frameTimes[FRAMESAMPLES];
	uint64 m_frameAvgTicks;
	int m_avgFrameIndex;
	double m_avgDeltaTime;
	double m_deltaTime;

	double m_accumulatedTime;

	double m_idealUpdateTime;
	double m_fixedUpdateTime;
	double m_minimumSecsPerFrame;
};

#include "TimeUtility.inl"