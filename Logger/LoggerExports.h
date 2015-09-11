#include <string>

#define API_EXPORT __declspec(dllexport)

extern "C"
{
	API_EXPORT void CreateLogger(void*& api);
	API_EXPORT void ReleaseLogger(void*& api);

	// API

	API_EXPORT double GetNow(void* api);

	API_EXPORT void GenerateLogs(void* api);

	API_EXPORT void LogVariable(void* api, const std::string& name, double variable, double time = 0);
	API_EXPORT void LogFuncCall(void* api, const std::string& funcName, double called = 1, double time = 0);
	API_EXPORT void LogEvent(void* api, const char* name, double time = 0);

	API_EXPORT void RecordFuncCall(void* api, const std::string& funcName);
	API_EXPORT void RecordVariable(void* api, const std::string& varName, double value);

	API_EXPORT void SetAverageStride(void* api, double stride);
}