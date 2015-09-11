#include "TimeUtility.h"

#define TO_SECONDS_D 1.0
#define TO_MILLISECONDS_D 1.0e3
#define TO_MICROSECONDS_D 1.0e6
#define TO_NANOSECONDS_D 1.0e9

#define TO_SECONDS_F 1.0f
#define TO_MILLISECONDS_F 1.0e3f
#define TO_MICROSECONDS_F 1.0e6f
#define TO_NANOSECONDS_F 1.0e9f

#define TO_SECONDS_I 1
#define TO_MILLISECONDS_I 1000
#define TO_MICROSECONDS_I 1000000
#define TO_NANOSECONDS_I 1000000000


#ifdef _WIN32

#include "WindowsSystemClock.h"

#else

namespace SystemClock
{
	uint64 GetTickCounter() { return 0; }
	uint64 GetTicksPerSecond() { return 0; }
	uint64 GetTicksPerMilliSecond() { return 0; }
	uint64 GetTicksPerMicroSecond() { return 0; }
	uint64 GetTicksPerNanoSecond() { return 0; }
	double GetInverseTicksPerSecond() { return 0.0; }
	double GetInverseTicksPerMilliSecond() { return 0.0; }
	double GetInverseTicksPerMicroSecond() { return 0.0; }
	double GetInverseTicksPerNanoSecond() { return 0.0; }
	float GetInverseTicksPerSecondF() { return 0.0f; }
	float GetInverseTicksPerMilliSecondF() { return 0.0f; }
	float GetInverseTicksPerMicroSecondF() { return 0.0f; }
	float GetInverseTicksPerNanoSecondF() { return 0.0f; }
} // SystemClock

#endif




//------------------------------------------------------------------------------
// Converts machine Ticks to double precision seconds.
double TimeUtility::TicksToSeconds(uint64 ticks)
{
#if 0
	return double(ticks) / double(SystemClock::GetTicksPerSecond());
#else
	return double(ticks) * SystemClock::GetInverseTicksPerSecond();
#endif
} // TicksToSeconds()

//------------------------------------------------------------------------------
// Converts machine Ticks to double precision milli-seconds.
double TimeUtility::TicksToMilliSeconds(uint64 ticks)
{
#if 0
	return (1.0e3 * double(ticks)) / double(SystemClock::GetTicksPerMilliSecond());
#else
	return double(ticks) * SystemClock::GetInverseTicksPerMilliSecond();
#endif
} // TicksToMilliSeconds()

//------------------------------------------------------------------------------
// Converts machine Ticks to double precision micro-seconds.
double TimeUtility::TicksToMicroSeconds(uint64 ticks)
{
#if 0
	return (1.0e6 * double(ticks)) / double(SystemClock::GetTicksPerMicroSecond());
#else
	return double(ticks) * SystemClock::GetInverseTicksPerMicroSecond();
#endif
} // TicksToMicroSeconds()

//------------------------------------------------------------------------------\
// Converts machine Ticks to double precision nano-seconds.
double TimeUtility::TicksToNanoSeconds(uint64 ticks)
{
#if 0
	return (1.0e9 * double(ticks)) / double(SystemClock::GetTicksPerNanoSecond());
#else
	return double(ticks) * SystemClock::GetInverseTicksPerNanoSecond();
#endif
} // TicksToNanoSeconds()



//------------------------------------------------------------------------------
// Converts machine Ticks to floating precision seconds.
float TimeUtility::TicksToSecondsF(uint64 ticks)
{
#if 0
	return float(ticks) / float(SystemClock::GetTicksPerSecond());
#else
	return float(ticks) * SystemClock::GetInverseTicksPerSecondF();
#endif
} // TicksToSecondsF()

//------------------------------------------------------------------------------
// Converts machine Ticks to floating precision milli-seconds.
float TimeUtility::TicksToMilliSecondsF(uint64 ticks)
{
#if 0
	return (1.0e3f * float(ticks)) / float(SystemClock::GetTicksPerMilliSecond());
#else
	return float(ticks) * SystemClock::GetInverseTicksPerMilliSecondF();
#endif
} // TicksToMilliSecondsF()

//------------------------------------------------------------------------------
// Converts machine Ticks to floating precision micro-seconds.
float TimeUtility::TicksToMicroSecondsF(uint64 ticks)
{
#if 0
	return (1.0e6f * float(ticks)) / float(SystemClock::GetTicksPerMicroSecond());
#else
	return float(ticks) * SystemClock::GetInverseTicksPerMicroSecondF();
#endif
} // TicksToMicroSecondsF()

//------------------------------------------------------------------------------
// Converts machine Ticks to floating precision nano-seconds.
float TimeUtility::TicksToNanoSecondsF(uint64 ticks)
{
#if 0
	return (1.0e9f * float(ticks)) / float(SystemClock::GetTicksPerNanoSecond());
#else
	return float(ticks) * SystemClock::GetInverseTicksPerNanoSecondF();
#endif
} // TicksToNanoSecondsF()



//------------------------------------------------------------------------------
//
uint64 TimeUtility::SecondsToTicks(double seconds)
{
#if 1
	return uint64(double(SystemClock::GetTicksPerSecond()) * seconds);
#else
	return uint64(1.0 / (SystemClock::GetInverseTicksPerSecond() / seconds));
#endif
} // SecondsToTicks()

//------------------------------------------------------------------------------
//
uint64 TimeUtility::MilliSecondsToTicks(double ms)
{
#if 1
	return uint64(double(SystemClock::GetTicksPerMilliSecond()) * ms);
#else
	return uint64(1.0 / (SystemClock::GetInverseTicksPerMilliSecond() / ms));
#endif
} // MilliSecondsToTicks()

//------------------------------------------------------------------------------
//
uint64 TimeUtility::MicroSecondsToTicks(double mcs)
{
#if 1
	return uint64(double(SystemClock::GetTicksPerMicroSecond()) * mcs);
#else
	return uint64(1.0 / (SystemClock::GetInverseTicksPerMicroSecond() / mcs));
#endif
} // MicroSecondsToTicks()

//------------------------------------------------------------------------------
//
uint64 TimeUtility::NanoSecondsToTicks(double ns)
{
#if 1
	return uint64(double(SystemClock::GetTicksPerNanoSecond()) * ns);
#else
	return uint64(1.0 / (SystemClock::GetInverseTicksPerNanoSecond() / ns));
#endif
} // NanoSecondsToTicks()



//------------------------------------------------------------------------------
//
uint64 TimeUtility::SecondsToTicksF(float seconds)
{
#if 1
	return uint64(float(SystemClock::GetTicksPerSecond()) * seconds);
#else
	return uint64(1.0f / (SystemClock::GetInverseTicksPerSecondF() / seconds));
#endif
} // SecondsToTicks()

//------------------------------------------------------------------------------
//
uint64 TimeUtility::MilliSecondsToTicksF(float ms)
{
#if 1
	return uint64(float(SystemClock::GetTicksPerMilliSecond()) * ms);
#else
	return uint64(1.0f / (SystemClock::GetInverseTicksPerMilliSecondF() / ms));
#endif
} // MilliSecondsToTicks()

//------------------------------------------------------------------------------
//
uint64 TimeUtility::MicroSecondsToTicksF(float mcs)
{
#if 1
	return uint64(float(SystemClock::GetTicksPerMicroSecond()) * mcs);
#else
	return uint64(1.0f / (SystemClock::GetInverseTicksPerMicroSecondF() / mcs));
#endif
} // MicroSecondsToTicks()

//------------------------------------------------------------------------------
//
uint64 TimeUtility::NanoSecondsToTicksF(float ns)
{
#if 1
	return uint64(float(SystemClock::GetTicksPerNanoSecond()) * ns);
#else
	return uint64(1.0f / (SystemClock::GetInverseTicksPerNanoSecondF() / ns));
#endif
} // NanoSecondsToTicks()