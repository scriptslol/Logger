#include "LoggerExports.h"
#include "LoggerImpl.h"

#define SAFE_API_CALL(c, i, f, ...) \
	if(i) { \
		static_cast<c*>(i)->f(__VA_ARGS__); \
	}

#define SAFE_API_CALL_RTRN(c, i, f, r, ...) \
	if(i) { \
		return static_cast<c*>(i)->f(__VA_ARGS__); \
	} else { \
		return r; \
	}


API_EXPORT void CreateLogger(void*& api)
{
	api = new Logger;
}

API_EXPORT void ReleaseLogger(void*& api)
{
	if (api)
	{
		delete api;
		api = nullptr;
	}
}

API_EXPORT double GetNow(void* api)
{
	SAFE_API_CALL_RTRN(Logger, api, GetNow, 0.0);
}

API_EXPORT void GenerateLogs(void* api)
{
	SAFE_API_CALL(Logger, api, GenerateLogs);
}

API_EXPORT void LogVariable(void* api, const std::string& name, double variable, double time/* = 0*/)
{
	SAFE_API_CALL(Logger, api, LogVariable, name, variable, time);
}

API_EXPORT void LogFuncCall(void* api, const std::string& funcName, double called /*= 1*/, double time/* = 0*/)
{
	SAFE_API_CALL(Logger, api, LogFuncCall, funcName, called, time);
}

API_EXPORT void LogEvent(void* api, const char* name, double time/* = 0*/)
{
	SAFE_API_CALL(Logger, api, LogEvent, name, time);
}

API_EXPORT void RecordFuncCall(void* api, const std::string& funcName)
{
	SAFE_API_CALL(Logger, api, RecordFuncCall, funcName);
}

API_EXPORT void RecordVariable(void* api, const std::string& varName, double value)
{
	SAFE_API_CALL(Logger, api, RecordVariable, varName, value);
}

API_EXPORT void SetAverageStride(void* api, double stride)
{
	SAFE_API_CALL(Logger, api, SetAverageStride, stride);
}