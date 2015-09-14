#pragma once


typedef unsigned long long uint64;

//------------------------------------------------------------------------------
// Get current system ticks via Win32 QueryPerformanceCounter
uint64 SystemClock::GetTickCounter()
{
	return 0;
} // GetTickCounter()



//------------------------------------------------------------------------------
// Get system tick frequency via Win32 QueryPerformanceFrequency
// Scale: Seconds
// Returns unsigned 64bit integer
uint64 SystemClock::GetTicksPerSecond()
{
	return 1000;
} // GetTicksPerSecond()

//------------------------------------------------------------------------------
// Get system tick frequency via Win32 QueryPerformanceFrequency
// Scale: Milli-Seconds
// Returns unsigned 64bit integer
uint64 SystemClock::GetTicksPerMilliSecond()
{
	return 0;
} // GetTicksPerMilliSecond()

//------------------------------------------------------------------------------
// Get system tick frequency via Win32 QueryPerformanceFrequency
// Scale: Micro-Seconds
// Returns unsigned 64bit integer
uint64 SystemClock::GetTicksPerMicroSecond()
{
	return 0;
} // GetTicksPerMicroSecond()

//------------------------------------------------------------------------------
// Get system tick frequency via Win32 QueryPerformanceFrequency
// Scale: Nano-Seconds
// Returns unsigned 64bit integer
uint64 SystemClock::GetTicksPerNanoSecond()
{
	return 0;
} // GetTicksPerNanoSecond()



//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0 / frequency) via Win32 QueryPerformanceFrequency
// Scale: Seconds
// Returns double precision
double SystemClock::GetInverseTicksPerSecond()
{
	return 0;
} // GetInverseTicksPerSecond()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0 / frequency) via Win32 QueryPerformanceFrequency
// Scale: Milli-Seconds
// Returns double precision
double SystemClock::GetInverseTicksPerMilliSecond()
{
	return 0;
} // GetInverseTicksPerSecond()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0 / frequency) via Win32 QueryPerformanceFrequency
// Scale: Micro-Seconds
// Returns double precision
double SystemClock::GetInverseTicksPerMicroSecond()
{
	return 0;
} // GetInverseTicksPerSecond()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0 / frequency) via Win32 QueryPerformanceFrequency
// Scale: Nano-Seconds
// Returns double precision
double SystemClock::GetInverseTicksPerNanoSecond()
{
	return 0;
} // GetInverseTicksPerSecond()



//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0f / frequency) via Win32 QueryPerformanceFrequency
// Scale: Seconds
// Returns float precision
float SystemClock::GetInverseTicksPerSecondF()
{
	return 0;
} // GetInverseTicksPerSecondF()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0f / frequency) via Win32 QueryPerformanceFrequency
// Scale: Milli-Seconds
// Returns float precision
float SystemClock::GetInverseTicksPerMilliSecondF()
{
	return 0;
} // GetInverseTicksPerSecondF()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0f / frequency) via Win32 QueryPerformanceFrequency
// Scale: Micro-Seconds
// Returns float precision
float SystemClock::GetInverseTicksPerMicroSecondF()
{
	return 0;
} // GetInverseTicksPerSecondF()

//------------------------------------------------------------------------------
// Get system tick inverse frequency (1.0f / frequency) via Win32 QueryPerformanceFrequency
// Scale: Nano-Seconds
// Returns float precision
float SystemClock::GetInverseTicksPerNanoSecondF()
{
	return 0;
} // GetInverseTicksPerSecondF()