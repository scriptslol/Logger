#pragma once

#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES

#if __EMSCRIPTEN__
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <string>
#include <ctime>
#include "TimeUtility.h"

//#include "esUtil.h"

//#include <GL/glut.h>
//#include <GLES2/gl2.h>
//#include <EGL/egl.h>


//#define HIGH_PRECISION
#define MAXLEGENDS	10
#define MAXSERIES	50

#ifdef HIGH_PRECISION

#define GLM_PRECISION_HIGHP_INT
#define GLM_PRECISION_HIGHP_FLOAT
#define GLM_PRECISION_HIGHP_UINT

typedef double real;
#else

#define GLM_PRECISION_MEDIUMP_INT
#define GLM_PRECISION_MEDIUMP_FLOAT
#define GLM_PRECISION_MEDIUMP_UINT

typedef float real;
#endif

#ifndef _MSC_VER
#define GLM_FORCE_CXX11
#endif

#include "glm/glm.hpp"

using namespace glm;


enum TimeMode
{
	DEFAULT = 0
};

struct Value
{
	real value;
	real time;
};

struct oglRect
{
	oglRect()
	{
		left = right = top = bottom = 0.0;
	}

	real Height()
	{
		return glm::abs(top - bottom);
	}

	real Width()
	{
		return glm::abs(left - right);
	}

#if __EMSCRIPTEN__
	real left, right, top, bottom;
#else
	union
	{
		struct { vec4 data; };
		struct { real left, right, top, bottom; };
	};
#endif
};


//////////////////////////////////////////////////////////////////////////


class Axis
{
public:

	Axis()
	{
		m_title = "Value Axis";

		m_minRange = 0.0;
		m_maxRange = 2000.0;

		m_valueDpi = 1.0;

		//glGetUniformLocation(0, "Projection");
		//glGetUniformLocation(0, "asd");
	}

//private:

	real m_minRange;
	real m_maxRange;

	real m_valueDpi;

	std::string m_title;
};


class TimeAxis
{
public:

	TimeAxis()
	{
		m_title = "Time Axis";

		m_minTime = 0.0;
		m_maxTime = 600.0;
		m_timeMode = (TimeMode)0;
		m_secsDpi = 1.0;
	}

//private:

	real m_minTime;
	real m_maxTime;

	real m_secsDpi;

	TimeMode m_timeMode;

	std::string m_title;
};


class Legend
{
public:

	Legend()
	{
		m_inUse = false;
		m_color = 0;
		m_style = 1;
		m_title = "";
	}

//private:

	bool m_inUse;
	unsigned int m_color;
	int m_style;
	std::string m_title;
};


class Series
{
public:

	Series();
	~Series();

	void AddPoint(real valueTime, real y);
	void Reset();

//private:

	bool m_inUse;
	vec3 m_color;
	int m_lineStyle;
	bool m_rightAxisAlign;
	Value* m_values;
	long m_noValues;
	long m_begin;
	long m_end;
};


class oglPlot
{
public:

	oglPlot();
	~oglPlot();

	void Create(vec2 loc, vec2 extents);

	bool AddPoint(int id, real time, real value);
	void SetBXRange(real fromTime, real toTime, bool move = true);
	void SetLYRange(double minrange, double maxrange);
	void SetRYRange(double minrange, double maxrange);
	void Reset();

	void SetSeries(int id, int style, vec3 color, real minrange, real maxrange, const char* title, bool rightAlign = false);
	void SetLegend(int l, int style, vec3 color, const char *text);
	void SetBXTitle(const char *title);
	void SetLYTitle(const char *title);
	void SetRYTitle(const char *title);

	void Draw();

//private:

	void DrawGrid();
	void DrawPlot();

	void _DrawSeries(int id);
	void _DrawXAxisGrid();
	void _DrawYAxisGrid();

	void _ComputeInternals(bool initialize);

	oglRect m_controlRect;
	oglRect m_clientRect;
	oglRect m_plotRect;
	oglRect m_legendRect;
	oglRect m_axisLYRect;
	oglRect m_axisRYRect;
	oglRect m_axisBXRect;

	int m_leftMargin;
	int m_rightMargin;
	int m_topMargin;
	int m_bottomMargin;

	vec3 m_controlColor;
	vec3 m_plotBgColor;
	vec3 m_legendBgColor;
	vec3 m_gridColor;

	bool m_controlBorder;
	bool m_plotBorder;
	bool m_legendBorder;
	bool m_primaryLegend;
	bool m_secondaryLegend;
	bool m_axisLY;
	bool m_axisRY;
	bool m_axisBX;
	bool m_autoScrollX;
	bool m_simulateMode;

	static long m_maxDataPerSeries;
	static long m_maxDataTotal;

	real m_noData;

	Legend m_primaryLegends[MAXLEGENDS];
	Legend m_secondayLegends[MAXLEGENDS];

	Series m_series[MAXSERIES];

	Axis m_leftAxis;
	Axis m_rightAxis;

	TimeAxis m_timeAxis;

	vec2* m_lineArray;
	long m_lineArraySize;

	// Font?
	real m_zoom;
	int m_textHeight;
};