#include "oglPlot.h"

#define _RGB(r, g, b) (r/255.0),(g/255.0),(b/255.0)

long oglPlot::m_maxDataPerSeries;
long oglPlot::m_maxDataTotal;

Series::Series()
{
	m_inUse = false;
	m_color = vec3(0.0);
	m_lineStyle = 0;
	m_rightAxisAlign = false;
	m_noValues = 0;
	m_begin = 0;
	m_end = 0;
	m_values = nullptr;
}

Series::~Series()
{
	if (m_values)
		free(m_values);
}

void Series::AddPoint(real valueTime, real y)
{
	if (m_noValues > 0)
	{
		m_values = (Value*)realloc(m_values, (m_noValues + 1) * sizeof(Value));
	}
	else
	{
		m_values = (Value*)malloc((m_noValues + 1) * sizeof(Value));
	}

	m_values[m_end].time = valueTime;
	m_values[m_end].value = y;

	m_noValues++;
	m_end++;

	if (m_end >= oglPlot::m_maxDataPerSeries)
	{
		m_end = 0;
	}

	if (m_begin == m_end)
	{
		m_begin++;
		if (m_begin >= oglPlot::m_maxDataPerSeries)
		{
			m_begin = 0;
		}
	}
}

void Series::Reset()
{
	m_noValues = 0;
	if (m_values)
		free(m_values);
	m_values = nullptr;

	m_begin = 0;
	m_end = 0;
}


oglPlot::oglPlot()
{
	//oglRect m_controlRect;
	//oglRect m_clientRect;
	//oglRect m_plotRect;
	//oglRect m_axisLYRect;
	//oglRect m_axisRYRect;
	//oglRect m_axisBXRect;

	//int m_leftMargin;
	//int m_rightMargin;
	//int m_topMargin;
	//int m_bottomMargin;

	m_controlColor = vec3(_RGB(255, 255, 255));
	m_plotBgColor = vec3(_RGB(255, 255, 255));
	m_legendBgColor = vec3(_RGB(255, 255, 255));
	m_gridColor = vec3(_RGB(127, 127, 127));

	m_controlBorder = true;
	m_plotBorder = true;
	m_legendBorder = true;
	m_primaryLegend = false;
	m_secondaryLegend = false;
	m_axisLY = true;
	m_axisRY = true;
	m_axisBX = true;
	m_autoScrollX = false;
	m_simulateMode = false;

	m_maxDataPerSeries = 10000;
	m_maxDataTotal = 100000;
	m_noData = 0.0;

	m_zoom = 1.0;

	//Legend m_primaryLegends[10];
	//Legend m_secondayLegends[10];

	//Series m_series[10];

	//Axis m_leftAxis;
	//Axis m_rightAxis;

	//TimeAxis m_timeAxis;

	m_lineArraySize = 1000;
	m_lineArray = new vec2[m_lineArraySize];

	SetBXRange(TimeUtility::GetTime() - 60.0, TimeUtility::GetTime());
}

oglPlot::~oglPlot()
{
	delete[] m_lineArray;
}

void oglPlot::Create(vec2 loc, vec2 extents)
{
	m_controlRect.left = loc.x;
	m_controlRect.top = loc.y;
	m_controlRect.right = loc.x + extents.x;
	m_controlRect.bottom = loc.y - extents.y;

	_ComputeInternals(true);
}

bool oglPlot::AddPoint(int id, real time, real value)
{
	if (m_series[id].m_noValues < m_maxDataPerSeries)
	{
		m_series[id].AddPoint(time, value);
		if (m_autoScrollX && time > m_timeAxis.m_maxTime)
		{
			real span = m_timeAxis.m_maxTime - m_timeAxis.m_minTime;
			real minTime = time - span;

			SetBXRange(minTime, time);
		}

		return true;
	}
	else
	{
		int temp = 0;
	}

	return false;
}

void oglPlot::SetBXRange(real fromTime, real toTime, bool move)
{
	m_timeAxis.m_minTime = fromTime;
	m_timeAxis.m_maxTime = toTime;
	if (!move)
	{
		m_timeAxis.m_secsDpi = ((real)(m_timeAxis.m_maxTime - m_timeAxis.m_minTime + 1)) / (real)m_plotRect.Width();
	}
}

void oglPlot::SetLYRange(double minrange, double maxrange)
{

}

void oglPlot::SetRYRange(double minrange, double maxrange)
{

}

void oglPlot::Reset()
{

}

void oglPlot::SetSeries(int id, int style, vec3 color, real minrange, real maxrange, const char* title, bool rightAlign /*= false*/)
{
	m_series[id].m_inUse = true;
	m_series[id].m_color = color;
	m_series[id].m_lineStyle = style;
	m_series[id].m_rightAxisAlign = rightAlign;
}

void oglPlot::SetLegend(int l, int style, vec3 color, const char *text)
{

}

void oglPlot::SetBXTitle(const char *title)
{

}

void oglPlot::SetLYTitle(const char *title)
{

}

void oglPlot::SetRYTitle(const char *title)
{

}

void oglPlot::Draw()
{
	//DrawGrid();
	DrawPlot();
}

void oglPlot::DrawGrid()
{
	_DrawXAxisGrid();
	//_DrawYAxisGrid();
}

void oglPlot::DrawPlot()
{
	for (int i = 0; i < MAXSERIES; i++)
	{
		if (m_series[i].m_inUse)
		{
			_DrawSeries(i);
		}
	}
}

void oglPlot::_DrawSeries(int id)
{
	bool more = true;
	bool draw;
	vec2 p;

	int ly;

	long y = m_series[id].m_begin;
	long m = m_series[id].m_end;
	long a = m_series[id].m_noValues;
	bool rightAxis = m_series[id].m_rightAxisAlign;

	while (more)
	{
		draw = false;
		more = false;

		ly = 0;

		while (y != m && !draw)
		{
			if (m_series[id].m_values[y].value == m_noData)
			{
				draw = true;
				more = true;
			}
			else
			{
				real valueTime = m_series[id].m_values[y].time;
				p.x = (int)(m_plotRect.left + ((valueTime - m_timeAxis.m_minTime) / m_timeAxis.m_secsDpi));

				if (rightAxis)
				{
					p.y = (int)(m_plotRect.bottom + ((m_series[id].m_values[y].value - m_rightAxis.m_minRange) / m_rightAxis.m_valueDpi));
				}
				else
				{
					p.y = (int)(m_plotRect.bottom + ((m_series[id].m_values[y].value - m_leftAxis.m_minRange) / m_leftAxis.m_valueDpi));
				}

				if ((ly == 0 || p.x != m_lineArray[ly].x || p.y != m_lineArray[ly].y)
					&& (p.x >= m_plotRect.left && p.x <= m_plotRect.right))
				{
					m_lineArray[ly].x = p.x;
					m_lineArray[ly].y = p.y;
					ly++;

					if (ly >= m_lineArraySize)
						break;
				}
			}

			y++;
			if (y > a)
				y = 0;
		}

		// Draw lines
		if (ly > 0)
		{
			vec3 c = m_series[id].m_color;
			//glColor3f(c.r, c.g, c.b);

			//glBegin(GL_LINES);

			for (int i = 0; i < ly; i++)
			{
				vec2& v1 = m_lineArray[i];
				//glVertex2f(v1.x, v1.y);

				if (!((i + 1) >= ly))
				{
					vec2& v2 = m_lineArray[i + 1];
					//glVertex2f(v2.x, v2.y);
				}
			}

			//glEnd();
		}
	}
}

void oglPlot::_DrawXAxisGrid()
{
	long yGrid = m_timeAxis.m_minTime;
	long delta = (long)(10.0 + (long)(m_timeAxis.m_secsDpi) * 10);
	long d10 = (long)(delta / 10.0);

	long diff = ((long)yGrid) % ((long)delta);

	yGrid = yGrid - diff;

	//glBegin(GL_LINES);

#if 0
	for (long sx = m_timeAxis.m_minTime - diff; sx < m_timeAxis.m_maxTime; sx += d10)
	{
		int off = 3;

		if ((long)sx % (long)delta == 0)
		{
			off = 5;
		}

		if (sx > m_timeAxis.m_minTime)
		{
			int x = (int)(m_plotRect.left + ((sx - m_timeAxis.m_minTime) / m_timeAxis.m_secsDpi));

			glColor3f(0, 0, 0);
			glVertex2f(x, m_plotRect.bottom);
			glVertex2f(x + off, m_plotRect.bottom);

			glColor3f((192.0 / 255.0), (192.0 / 255.0), (192.0 / 255.0));
			glVertex2f(x, m_plotRect.bottom - 1);
			glVertex2f(x, m_plotRect.top + 1);
		}
	}
#endif

	while (yGrid <= m_timeAxis.m_maxTime)
	{
		int x = (int)(m_plotRect.left + ((yGrid - m_timeAxis.m_minTime) / m_timeAxis.m_secsDpi));

		if (yGrid > m_timeAxis.m_minTime && yGrid < m_timeAxis.m_maxTime)
		{
			//glColor3f(0.1, 1, 0.2);
			//glVertex2f(x, m_plotRect.bottom - 1);
			//glVertex2f(x, m_plotRect.top + 1);
		}

		yGrid += delta;
	}

	//glEnd();
}

void oglPlot::_DrawYAxisGrid()
{
	real yGrid = m_leftAxis.m_minRange;
	real delta = 25.0 + (long)(((m_leftAxis.m_valueDpi))) * 25;

	if ((long)delta % 50 != 0 && delta > 20.0)
	{
		delta += 25.0;
	}

	real d10 = delta / 5.0;

	long diff = ((long)yGrid) % ((long)delta);
	yGrid = yGrid - diff;

	//glBegin(GL_LINES);

	for (long sy = (long)((long)(m_leftAxis.m_minRange) - diff); sy < m_leftAxis.m_maxRange; sy += (long)d10)
	{
		int off = 3;
		if ((long)sy % (long)delta == 0)
		{
			off = 5;
		}

		if (sy > m_leftAxis.m_minRange)
		{
			int y = (int)(m_plotRect.bottom - ((sy - m_leftAxis.m_minRange) / m_leftAxis.m_valueDpi));

			//glColor3f(0, 0, 0);
			//glVertex2f(m_plotRect.left, y);
			//glVertex2f(m_plotRect.left - off, y);
			//glVertex2f(m_plotRect.right, y);
			//glVertex2f(m_plotRect.right + off, y);

			//glColor3f((192.0 / 255.0), (192.0 / 255.0), (192.0 / 255.0));
			//glVertex2f(m_plotRect.left + 1, y);
			//glVertex2f(m_plotRect.right - 1, y);
		}
	}

	while (yGrid <= m_leftAxis.m_maxRange)
	{
		real yy = m_plotRect.bottom + ((yGrid - m_leftAxis.m_minRange) / m_leftAxis.m_valueDpi);
		int y = (int)yy;

		if (yGrid > m_leftAxis.m_minRange && yGrid < m_leftAxis.m_maxRange)
		{
			//glColor3f(0.1, 1, 0.2);
			//glVertex2f(m_plotRect.left + 1, y);
			//glVertex2f(m_plotRect.right - 1, y);
		}

		// draw text?

		yGrid += delta;
	}

	//glEnd();
}

void oglPlot::_ComputeInternals(bool initialize)
{
	int w = 0;
	int n = 0;

	//vec2 z(glutBitmapWidth(GLUT_BITMAP_8_BY_13, 'i'), glutBitmapHeight(GLUT_BITMAP_8_BY_13));
	vec2 z(8.0, 14.0);

	m_zoom = ((real)m_controlRect.Height() / (real)z.y) / 25.0;

	if (m_controlBorder)
	{
		m_clientRect.left = m_controlRect.left + 2;
		m_clientRect.right = m_controlRect.right - 2;
		m_clientRect.top = m_controlRect.top + 2;
		m_clientRect.bottom = m_controlRect.bottom - 2;
	}
	else
	{
		m_clientRect = m_controlRect;
	}

	if (initialize)
	{
		m_topMargin = m_bottomMargin = m_clientRect.Height() / 10;
		m_leftMargin = m_rightMargin = m_clientRect.Width() / 10;
	}

	m_plotRect.left = m_clientRect.left + m_leftMargin;
	m_plotRect.right = m_clientRect.right - m_rightMargin;
	m_plotRect.top = m_clientRect.top + m_topMargin;
	m_plotRect.bottom = m_clientRect.bottom - m_bottomMargin;

	//if (initialize)
	//{
	//	m_legendRect.left = m_plotRect.left + (m_leftMargin / 5);
	//	m_legendRect.right = m_plotRect.left + (m_plotRect.Width() / 5);
	//	m_legendRect.top = m_plotRect.top - (m_topMargin / 2);
	//	m_legendRect.bottom = m_plotRect.top + (m_topMargin);

	//	int w = 0;
	//	int n = 0;

	//	for (int x = 0; x < MAXLEGENDS; x++)
	//	{
	//		if (m_primaryLegends[x].m_inUse)
	//		{
	//			n++;
	//			//z = glutbit
	//		}
	//	}
	//}

	m_timeAxis.m_secsDpi = ((real)(m_timeAxis.m_maxTime - m_timeAxis.m_minTime)) / (real)m_plotRect.Width();
	m_leftAxis.m_valueDpi = ((real)(m_leftAxis.m_maxRange - m_leftAxis.m_minRange) / (real)m_plotRect.Height());
	m_rightAxis.m_valueDpi = ((real)(m_rightAxis.m_maxRange - m_rightAxis.m_minRange) / (real)m_plotRect.Height());
}