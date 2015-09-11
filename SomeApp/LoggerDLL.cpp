#include "LoggerDLL.h"

namespace
{
	std::wstring GetLastErrorString()
	{
		LPWSTR msgBuff = nullptr;
		size_t size = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, 
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPWSTR)&msgBuff, 
			0, 
			NULL);

		std::wstring msg(msgBuff, size);
		LocalFree(msgBuff);

		return msg;
	}
}


LoggerDLL::LoggerDLL()
{
	m_dll = nullptr;
	m_api = nullptr;

	m_fCreateLogger = nullptr;
	m_fReleaseLogger = nullptr;
	m_fGetNow = nullptr;
	m_fGenerateLogs = nullptr;
	m_fLogVariable = nullptr;
	m_fLogFuncCall = nullptr;
	m_fLogEvent = nullptr;
	m_fRecordFuncCall = nullptr;
	m_fRecordVariable = nullptr;

	Load();
}

LoggerDLL::~LoggerDLL()
{
	if (IsLoaded())
		Unload();
}

bool LoggerDLL::Load()
{
	m_dll = LoadLibrary(L"Logger.dll");

	if (!m_dll)
	{
		OutputDebugStringW(GetLastErrorString().c_str());
		return false;
	}

	DLL_FUNC_LINK(m_dll, CreateLogger);
	DLL_FUNC_LINK(m_dll, ReleaseLogger);
	DLL_FUNC_LINK(m_dll, GetNow);
	DLL_FUNC_LINK(m_dll, GenerateLogs);
	DLL_FUNC_LINK(m_dll, LogVariable);
	DLL_FUNC_LINK(m_dll, LogFuncCall);
	DLL_FUNC_LINK(m_dll, LogEvent);
	DLL_FUNC_LINK(m_dll, RecordFuncCall);
	DLL_FUNC_LINK(m_dll, RecordVariable);

	if (m_fCreateLogger && m_fReleaseLogger)
	{
		m_fCreateLogger(m_api);
		return true;
	}
	else
	{
		Unload();
		return false;
	}
}

bool LoggerDLL::Reload()
{
	Unload();
	return Load();
}

bool LoggerDLL::Unload()
{
	m_fReleaseLogger(m_api);

	if (m_dll)
	{
		FreeLibrary(m_dll);
		m_dll = nullptr;

		m_fCreateLogger = nullptr;
		m_fReleaseLogger = nullptr;
		m_fGetNow = nullptr;
		m_fGenerateLogs = nullptr;
		m_fLogVariable = nullptr;
		m_fLogFuncCall = nullptr;
		m_fLogEvent = nullptr;
		m_fRecordFuncCall = nullptr;
		m_fRecordVariable = nullptr;
	}

	return true;
}

double LoggerDLL::GetNow()
{
	if (m_fGetNow)
		return m_fGetNow(m_api);
	else
		return 0.0;
}

void LoggerDLL::GenerateLogs()
{
	if (m_fGenerateLogs)
		m_fGenerateLogs(m_api);
}

void LoggerDLL::LogVariable(const std::string& name, double variable, double time /*= 0*/)
{
	if (m_fLogVariable)
		m_fLogVariable(m_api, name, variable, time);
}

void LoggerDLL::LogFuncCall(const std::string& funcName, double called /*= 1*/, double time /*= 0*/)
{
	if (m_fLogFuncCall)
		m_fLogFuncCall(m_api, funcName, called, time);
}

void LoggerDLL::LogEvent(const char* name, double time /*= 0*/)
{
	if (m_fLogEvent)
		m_fLogEvent(m_api, name, time);
}

void LoggerDLL::RecordFuncCall(const std::string& funcName)
{
	if (m_fRecordFuncCall)
		m_fRecordFuncCall(m_api, funcName);
}

void LoggerDLL::RecordVariable(const std::string& varName, double value)
{
	if (m_fRecordVariable)
		m_fRecordVariable(m_api, varName, value);
}
