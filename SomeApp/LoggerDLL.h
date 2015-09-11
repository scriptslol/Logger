#pragma once
#include "Windows.h"
#include <string>

#define DLL_FUNC_DECL(rtrn, f, ...) \
	typedef rtrn (*__ ## f)(__VA_ARGS__); \
	__ ## f m_f ## f

#define DLL_FUNC_LINK(d, f) \
	m_f ## f = (__ ## f)GetProcAddress(d, #f)

class LoggerDLL
{
public:

	LoggerDLL();
	~LoggerDLL();

	bool IsLoaded() { return m_dll != nullptr; }

	// Logger API
	double GetNow();

	void GenerateLogs();

	void LogVariable(const std::string& name, double variable, double time = 0);
	void LogFuncCall(const std::string& funcName, double called = 1, double time = 0);
	void LogEvent(const char* name, double time = 0);

	void RecordFuncCall(const std::string& funcName);
	void RecordVariable(const std::string& varName, double value);

private:

	// DLL Control
	bool Load();
	bool Reload();
	bool Unload();

	HMODULE m_dll;
	void* m_api;

	DLL_FUNC_DECL(void, CreateLogger, void*&);
	DLL_FUNC_DECL(void, ReleaseLogger, void*&);
	DLL_FUNC_DECL(double, GetNow, void*);
	DLL_FUNC_DECL(void, GenerateLogs, void*);
	DLL_FUNC_DECL(void, LogVariable, void*, const std::string&, double, double);
	DLL_FUNC_DECL(void, LogFuncCall, void*, const std::string&, double, double);
	DLL_FUNC_DECL(void, LogEvent, void* api, const char*, double);
	DLL_FUNC_DECL(void, RecordFuncCall, void*, const std::string&);
	DLL_FUNC_DECL(void, RecordVariable, void*, const std::string&, double);
};