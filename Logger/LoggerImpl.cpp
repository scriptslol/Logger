#include "LoggerImpl.h"
#include <ctime>

Logger::Logger()
{
	m_avgStride = 0.0;
	m_defaultTimer = PerformanceStopWatch(0);
}

Logger::~Logger()
{

}

void Logger::Initialize()
{

}

void Logger::Release()
{

}

double Logger::GetNow()
{
	return m_defaultTimer.GetElapsedSeconds();
}

void Logger::LogVariable(const std::string& name, double variable, double time /* = 0 */)
{
	if (time == 0)
		time = m_defaultTimer.GetElapsedSeconds();

	auto findIT = m_variableLog.find(name);
	if (findIT != m_variableLog.end())
	{
		if (findIT->second.back().first == time)
		{
			time += 0.1;
		}
	}

	m_variableLog[name].push_back(std::make_pair(time, variable));
}

void Logger::LogFuncCall(const std::string& funcName, double called /* = 1 */, double time /* = 0 */)
{
	auto findIT = m_funcCallLog.find(funcName);
	if (findIT != m_funcCallLog.end())
	{
		TrackedFuncCall& tfc = findIT->second;

		// if adding at same time-index stack the called value
		if (tfc.m_funcCallHistory.back().first == time)
		{
			tfc.m_funcCallHistory.back().second += called;
			tfc.m_totalCalls += called;
			return;
		}
	}

	TrackedFuncCall& tfc = m_funcCallLog[funcName];
	tfc.m_funcCallHistory.push_back(std::make_pair(time, called));
	tfc.m_totalCalls += called;
}

void Logger::LogEvent(const char* name, double time /* = 0 */)
{
	if (time == 0)
		time = m_defaultTimer.GetElapsedSeconds();

	m_eventLog.push_back(std::make_pair(time, name));
}

void Logger::RecordFuncCall(const std::string& funcName)
{
	auto findIT = m_funcCallRecord.find(funcName);
	if (findIT == m_funcCallRecord.end())
	{
		m_funcCallRecord.insert(std::make_pair(funcName, 1));
	}
	else
	{
		m_funcCallRecord[funcName]++;
	}
}

void Logger::RecordVariable(const std::string& varName, double value)
{
	m_variableRecord[varName] = value;
}

namespace
{
	void _AppendTimestamp(std::string& s, double _seconds)
	{
		int hours = _seconds / 3600.0;
		_seconds -= hours * 3600;

		int minutes = _seconds / 60.0;
		_seconds -= minutes * 60;

		int seconds = _seconds;
		_seconds -= seconds;

		int mSeconds = _seconds * 1000.0;

		s += (hours < 10 ? "0" : "") + std::to_string(hours) + ":";
		s += (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":";
		s += (seconds < 10 ? "0" : "") + std::to_string(seconds) + ".";

		s += std::to_string(mSeconds);
	}
}

void Logger::PrepareData()
{
	// Average every 1 second
	if (m_avgStride <= 0.0)
		return;

	const double AVG_STRIDE = m_avgStride;

	VariableLog avgVariableLog;

	for (auto vl : m_variableLog)
	{
		double currentTime = 0.0;
		double prevTime = 0.0;

		double timeAccum = 0.0;
		double valueTotal = 0.0;
		double entries = 0.0;

		for (auto v : vl.second)
		{
			entries++;

			timeAccum += v.first - prevTime;
			prevTime = v.first;

			valueTotal += v.second;

			if (timeAccum >= AVG_STRIDE)
			{
				if (currentTime == 0.0)
					currentTime = v.first;
				else
					currentTime += timeAccum;

				if (valueTotal > 0.0)
					avgVariableLog[vl.first].push_back(std::make_pair(currentTime, valueTotal / entries));

				timeAccum = 0.0;
				valueTotal = 0.0;
				entries = 0.0;
			}
		}

		if (entries > 0)
		{
			currentTime += timeAccum;
			avgVariableLog[vl.first].push_back(std::make_pair(currentTime, valueTotal / entries));
		}
	}

	// Overwrite the old data with the averaged data
	m_variableLog = avgVariableLog;
}

void Logger::GenerateLogs()
{
	PrepareData();

	time_t t = time(0);
	struct tm now;

	localtime_s(&now, &t);

	if (!m_variableLog.empty())
	{
		std::ofstream file("Logs/VariableLog_" + std::to_string(now.tm_mon + 1) + std::to_string(now.tm_mday) + std::to_string(now.tm_hour) + std::to_string(now.tm_min) + std::to_string(now.tm_sec) + ".html");

		// Write head
		file << "<!DOCTYPE html>\n"
			"<html>\n"
			"<head>\n"
			"<title>Activity Log Graph</title>\n"
			"<link href = \"tools/c3.css\" rel = \"stylesheet\" type = \"text/css\">\n"
			"</head>\n"
			"<body>\n"
			"<script src = \"tools/d3.min.js\" charset = \"utf-8\"></script>\n"
			"<script src = \"tools/c3.min.js\"></script>\n";

		// create the final timestamp
		std::string finalTimeStamp("");
		_AppendTimestamp(finalTimeStamp, m_defaultTimer.GetElapsedSeconds());

		//////////////////////////////////////////////////////////////////////////
		// build the event grid
		std::string eventGrid("");

		eventGrid += "grid: {\n"
			"x: {\n"
			"show: true,\n"
			"lines: [\n";

		for (auto e : m_eventLog)
		{
			std::string timestamp("");
			_AppendTimestamp(timestamp, e.first);
			eventGrid += "{value: '" + timestamp + "', text: '" + std::string(e.second) + "'},\n";
		}

		eventGrid += "]\n"
			"},\n";

		eventGrid += "y: {\n"
			"show: true,\n"
			"}\n"
			"}\n";

		//////////////////////////////////////////////////////////////////////////

		// Write chart for each tracked variable
		int i = 0;
		for (auto v : m_variableLog)
		{
			i++;

			std::string chartID("chart" + std::to_string(i));

			file << "<div id = \"" << chartID << "\"></div>\n"
				"<script>\n"
				"var " << chartID << " = c3.generate({\n"
				"bindto: '#" << chartID << "',\n"
				"data : {\n"
				"x: 'x',\n"
				"xFormat : '%H:%M:%S.%L',\n"
				"columns: [" << std::endl;

			std::string axis("['x'");
			std::string data("['" + std::string(v.first) + "'");

			for (auto e : v.second)
			{
				//axis += ", " + std::to_string(e.first);
				axis += ", '";
				_AppendTimestamp(axis, e.first);
				axis += "'";

				data += ", " + std::to_string(e.second);
			}

			axis += "],";
			data += "]";

			file << axis << std::endl;
			file << data << std::endl;
			file << "]\n},\n";

			// Add zooming
			file << "zoom: {\n"
				"enabled: true\n"
				"}," << std::endl;

			// Disable transitions for perf gain
			file << "transition: {\n"
				"duration: 0\n"
				"},\n";

			// Disable points for perf gain
			file << "point: {\n"
				"show: false\n"
				"},\n" << std::endl;

			// Setup the axis format and set extents
			file << "axis: {\n"
				"x: {\n"
				"min: '00:00:00.000',\n"
				"max: '" << finalTimeStamp << "',\n"
				"type: 'timeseries',\n"
				"tick : {\n"
				"fit: false,\n"
				"format : \"%H:%M:%S.%L\"\n"
				"}\n"
				"}\n"
				"}," << std::endl;

			// Add event lines
			file << eventGrid << std::endl;

			// Finished
			file << "});\n"
				"</script>\n" << std::endl;
		}

		// Write tail
		file << "</body>\n"
			"</html>" << std::endl;

		//file << "]\n"
		//	"},\n"
		//	"subchart: {\n"
		//	"show: true\n"
		//	"	},\n"
		//	"zoom : {\n"
		//	"enabled: true\n"
		//	"},\n"
		//	"legend : {\n"
		//	"  position: 'right'\n"
		//	"},\n"
		//	//"grid : {"
		//	//"	 x: {"
		//	//"	 lines: ["
		//	//"	 {value: 0, text : 'Log Start'},"
		//	//"	 { value: 29, text : 'Enter Menu' },"
		//	//"	 { value: 91, text : 'Start Game' },"
		//	//"	 { value: 106, text : 'Leave Game' },"
		//	//"	 { value: 108, text : 'Log End' }"
		//	//"			]"
		//	//"	 }"
		//	//"},"
		//	"axis: {\n"
		//	"		x: {\n"
		//	"		extent: [-1, 30]\n"
		//	"		}\n"
		//	"}\n"
		//	"});\n"
		//	"</script>\n"
		//	"</body>\n"
		//	"</html>" << std::endl;

		file.close();
	}

	//if (!m_funcCallLog.empty())
	if (!m_funcCallRecord.empty())
	{
		std::ofstream funcFile("Logs/FunctionCallLog_" + std::to_string(now.tm_mon + 1) + std::to_string(now.tm_mday) + std::to_string(now.tm_hour) + std::to_string(now.tm_min) + std::to_string(now.tm_sec) + ".html");

		funcFile << "<!DOCTYPE html>\n"
			"<html>\n"
			"<head>\n"
			"<title>Activity Log Graph</title>\n"
			"<link href = \"tools/c3.css\" rel = \"stylesheet\" type = \"text/css\">\n"
			"</head>\n"
			"<body>\n"
			"<script src = \"tools/d3.min.js\" charset = \"utf-8\"></script>\n"
			"<script src = \"tools/c3.min.js\"></script>\n";


		std::string chartID2("chart"/*+ std::to_string(i)*/);

		funcFile << "<div id = \"" << chartID2 << "\"></div>\n"
			"<script>\n"
			"var " << chartID2 << " = c3.generate({\n"
			"bindto: '#" << chartID2 << "',\n"
			"data : {\n"
			"type: 'pie',\n"
			"columns: [" << std::endl;

		//std::string axis("['x'");
		//std::string data("['" + std::string(v.first) + "'");

		//for (auto tfc : m_funcCallLog)
		for (auto fc : m_funcCallRecord)
		{
			//funcFile << "['" << tfc.first << "'";
			//for (auto fch : tfc.second.m_funcCallHistory)
			//{
			//	funcFile << ", " << fch.second;
			//}
			//funcFile << "]," << std::endl;

			//funcFile << "['" << tfc.first << "', " << tfc.second.m_totalCalls << "],\n";
			funcFile << "['" << fc.first << "', " << fc.second << "],\n";
		}

		funcFile << "],\n"
			"}," << std::endl;

		// Disable legend - way too many entries for that
		funcFile << "legend: {\n"
			"show: false\n"
			"}," << std::endl;

		// Add zooming
		//funcFile << "zoom: {\n"
		//	"enabled: true\n"
		//	"}," << std::endl;

		// Disable transitions for perf gain
		//funcFile << "transition: {\n"
		//	"duration: 0\n"
		//	"},\n";

		// Disable points for perf gain
		//funcFile << "point: {\n"
		//	"show: false\n"
		//	"},\n" << std::endl;

		// Setup the axis format and set extents
		//funcFile << "axis: {\n"
		//	"x: {\n"
		//	"min: '00:00:00.000',\n"
		//	"max: '" << finalTimeStamp << "',\n"
		//	"type: 'timeseries',\n"
		//	"tick : {\n"
		//	"fit: false,\n"
		//	"format : \"%H:%M:%S.%L\"\n"
		//	"}\n"
		//	"}\n"
		//	"}," << std::endl;

		// Add event lines
		//funcFile << eventGrid << std::endl;

		// Finished
		funcFile << "});\n"
			"</script>\n" << std::endl;

		funcFile.close();
	}

	//if (!m_funcCallRecord.empty())
	//{
	//	std::ofstream funcRecordFile("Logs/FunctionCallRecord_" + std::to_string(now.tm_mon + 1) + std::to_string(now.tm_mday) + std::to_string(now.tm_hour) + std::to_string(now.tm_min) + std::to_string(now.tm_sec) + ".csv");

	//	for (auto fc : m_funcCallRecord)
	//		funcRecordFile << fc.second << "," << fc.first << std::endl;

	//	funcRecordFile.close();
	//}


	//if (!m_variableRecord.empty())
	//{
	//	std::ofstream variableRecord("Logs/VariableRecord_" + std::to_string(now.tm_mon + 1) + std::to_string(now.tm_mday) + std::to_string(now.tm_hour) + std::to_string(now.tm_min) + std::to_string(now.tm_sec) + ".csv");

	//	std::vector<std::pair<std::string, double> > mapcopy(m_variableRecord.begin(), m_variableRecord.end());
	//	std::sort(mapcopy.begin(), mapcopy.end(), less_second<std::string, double>());

	//	for (auto vr : mapcopy)
	//		variableRecord << vr.second << "," << vr.first << std::endl;

	//	variableRecord.close();
	//}
}

void Logger::SetAverageStride(double stride)
{
	m_avgStride = stride;
}