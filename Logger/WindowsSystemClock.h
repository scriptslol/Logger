#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

/*
//  [9/5/2014 Anthony]
//	System specific implementation of querying hardware
//	for use in timing functionality.
//
//	Platform: Windows
//
//	Supported Queries:
//		Integer Ticks
//		Double precision Ticks
//		Floating precision Ticks
//
//	Suported Scales:
//		Seconds
//		Milli-Seconds
//		Micro-Seconds
//		Nano-Seconds
//
*/

#define WINCALL(x) x

typedef unsigned long long uint64;

//------------------------------------------------------------------------------
// Get current system ticks via Win32 QueryPerformanceCounter
uint64 SystemClock::GetTickCounter()
{
	LARGE_INTEGER i;
	WINCALL(QueryPerformanceCounter(&i));
	return uint64(i.QuadPart);
} // GetTickCounter()



//------------------------------------------------------------------------------
// Get system tick frequency via Win32 QueryPerformanceFrequency
// Scale: Seconds
// Returns unsigned 64bit integer
uint64 SystemClock::GetTicksPerSecond()
{
	static uint64 frequency = 0;
	if (frequency == 0)
	{
		WINCALL(QueryPerformanceFrequency((LARGE_INTEGER*)&frequency));
		frequency *= TO_SECONDS_I;
	}
	return frequency;
} // GetTicksPerSecond()

//------------------------------------------------------------------------------
// Get system tick frequency via Win32 QueryPerformanceFrequency
// Scale: Milli-Seconds
// Returns unsigned 64bit integer
uint64 SystemClock::GetTicksPerMilliSecond()
{
	static uint64 frequency = 0;
	if (frequency == 0)
	{
		WINCALL(QueryPerformanceFrequency((LARGE_INTEGER*)&frequency));
		frequency *= TO_MILLISECONDS_I;
	}
	return frequency;
} // GetTicksPerMilliSecond()

//------------------------------------------------------------------------------
// Get system tick frequency via Win32 QueryPerformanceFrequency
// Scale: Micro-Seconds
// Returns unsigned 64bit integer
uint64 SystemClock::GetTicksPerMicroSecond()
{
	static uint64 frequency = 0;
	if (frequency == 0)
	{
		WINCALL(QueryPerformanceFrequency((LARGE_INTEGER*)&frequency));
		frequency *= TO_MICROSECONDS_I;
	}
	return frequency;
} // GetTicksPerMicroSecond()

//------------------------------------------------------------------------------
// Get system tick frequency via Win32 QueryPerformanceFrequency
// Scale: Nano-Seconds
// Returns unsigned 64bit integer
uint64 SystemClock::GetTicksPerNanoSecond()
{
	static uint64 frequency = 0;
	if (frequency == 0)
	{
		WINCALL(QueryPerformanceFrequency((LARGE_INTEGER*)&frequency));
		frequency *= TO_NANOSECONDS_I;
	}
	return frequency;
} // GetTicksPerNanoSecond()



//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0 / frequency) via Win32 QueryPerformanceFrequency
// Scale: Seconds
// Returns double precision
double SystemClock::GetInverseTicksPerSecond()
{
	static double iFrequency = 0.0;
	if (iFrequency == 0.0)
	{
		LARGE_INTEGER frequency;
		WINCALL(QueryPerformanceFrequency(&frequency));
		iFrequency = 1.0 / double(frequency.QuadPart * TO_SECONDS_I);
	}

	return iFrequency;
} // GetInverseTicksPerSecond()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0 / frequency) via Win32 QueryPerformanceFrequency
// Scale: Milli-Seconds
// Returns double precision
double SystemClock::GetInverseTicksPerMilliSecond()
{
	static double iFrequency = 0.0;
	if (iFrequency == 0.0)
	{
		LARGE_INTEGER frequency;
		WINCALL(QueryPerformanceFrequency(&frequency));;
		iFrequency = 1.0 / double(frequency.QuadPart * TO_MILLISECONDS_I);
	}

	return iFrequency;
} // GetInverseTicksPerSecond()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0 / frequency) via Win32 QueryPerformanceFrequency
// Scale: Micro-Seconds
// Returns double precision
double SystemClock::GetInverseTicksPerMicroSecond()
{
	static double iFrequency = 0.0;
	if (iFrequency == 0.0)
	{
		LARGE_INTEGER frequency;
		WINCALL(QueryPerformanceFrequency(&frequency));;
		iFrequency = 1.0 / double(frequency.QuadPart * TO_MICROSECONDS_I);
	}

	return iFrequency;
} // GetInverseTicksPerSecond()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0 / frequency) via Win32 QueryPerformanceFrequency
// Scale: Nano-Seconds
// Returns double precision
double SystemClock::GetInverseTicksPerNanoSecond()
{
	static double iFrequency = 0.0;
	if (iFrequency == 0.0)
	{
		LARGE_INTEGER frequency;
		WINCALL(QueryPerformanceFrequency(&frequency));;
		iFrequency = 1.0 / double(frequency.QuadPart * TO_NANOSECONDS_I);
	}

	return iFrequency;
} // GetInverseTicksPerSecond()



//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0f / frequency) via Win32 QueryPerformanceFrequency
// Scale: Seconds
// Returns float precision
float SystemClock::GetInverseTicksPerSecondF()
{
	static float iFrequency = 0.0f;
	if (iFrequency == 0.0f)
	{
		LARGE_INTEGER frequency;
		WINCALL(QueryPerformanceFrequency(&frequency));;
		iFrequency = 1.0f / float(frequency.QuadPart * TO_SECONDS_I);
	}

	return iFrequency;
} // GetInverseTicksPerSecondF()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0f / frequency) via Win32 QueryPerformanceFrequency
// Scale: Milli-Seconds
// Returns float precision
float SystemClock::GetInverseTicksPerMilliSecondF()
{
	static float iFrequency = 0.0f;
	if (iFrequency == 0.0f)
	{
		LARGE_INTEGER frequency;
		WINCALL(QueryPerformanceFrequency(&frequency));;
		iFrequency = 1.0f / float(frequency.QuadPart * TO_MILLISECONDS_I);
	}

	return iFrequency;
} // GetInverseTicksPerSecondF()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0f / frequency) via Win32 QueryPerformanceFrequency
// Scale: Micro-Seconds
// Returns float precision
float SystemClock::GetInverseTicksPerMicroSecondF()
{
	static float iFrequency = 0.0f;
	if (iFrequency == 0.0f)
	{
		LARGE_INTEGER frequency;
		WINCALL(QueryPerformanceFrequency(&frequency));;
		iFrequency = 1.0f / float(frequency.QuadPart * TO_MICROSECONDS_I);
	}

	return iFrequency;
} // GetInverseTicksPerSecondF()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0f / frequency) via Win32 QueryPerformanceFrequency
// Scale: Nano-Seconds
// Returns float precision
float SystemClock::GetInverseTicksPerNanoSecondF()
{
	static float iFrequency = 0.0f;
	if (iFrequency == 0.0f)
	{
		LARGE_INTEGER frequency;
		WINCALL(QueryPerformanceFrequency(&frequency));;
		iFrequency = 1.0f / float(frequency.QuadPart * TO_NANOSECONDS_I);
	}

	return iFrequency;
} // GetInverseTicksPerSecondF()