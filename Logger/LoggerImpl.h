#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "TimeUtility.h"

class Logger
{
public:

	Logger();
	~Logger();

	void Initialize();
	void Release();

	double GetNow();

	void GenerateLogs();

	void LogVariable(const std::string& name, double variable, double time = 0);
	void LogFuncCall(const std::string& funcName, double called = 1, double time = 0);
	void LogEvent(const char* name, double time = 0);

	void RecordFuncCall(const std::string& funcName);
	void RecordVariable(const std::string& varName, double value);

	void SetAverageStride(double stride);

private:

	void PrepareData();

	struct TrackedFuncCall
	{
		std::vector<std::pair<double, double>> m_funcCallHistory;
		double m_totalCalls;
	};

	typedef std::map<std::string, std::vector<std::pair<double, double>>> VariableLog;
	VariableLog m_variableLog;

	std::map<std::string, int> m_funcCallRecord;
	std::vector<std::pair<double, const char*>> m_eventLog;
	std::map<std::string, double> m_variableRecord;
	std::map<std::string, TrackedFuncCall> m_funcCallLog;

	double m_avgStride;

	PerformanceStopWatch m_defaultTimer;
};