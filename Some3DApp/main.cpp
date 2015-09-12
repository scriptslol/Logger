
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include "freeglut/include/GL/freeglut.h"
#include "sdl/SDL.h"

#define WINDOW_WIDTH 800.0f
#define WINDOW_HEIGHT 300.0f

// 1000.0 / 60.0
#define UPDATE_INTERVAL 16.66666666666667f

typedef unsigned long long uint64;

namespace
{
	template<size_t N>
	struct CircularBuffer
	{
		CircularBuffer()
		{
			index = 0;
			memset(buffer, 0, sizeof(buffer));
		}

		uint64 operator[](int i)
		{
			return buffer[i];
		}

		size_t GetSize()
		{
			return N;
		}

		void Add(uint64 n)
		{
			buffer[index] = n;
			index++;

			if (index >= N)
				index = 0;
		}

		uint64 GetAverage()
		{
			uint64 accum = 0l;
			for (int i = 0; i < N; i++)
				accum += buffer[i];
			accum /= N;
			return accum;
		}

		uint64 GetPeak()
		{
			uint64 peak = 0l;
			for (int i = 0; i < N; i++)
				if (buffer[i] > peak)
					peak = buffer[i];
			return peak;
		}

		size_t index;
		uint64 buffer[N];
	};

	struct ReadableClock
	{
		ReadableClock()
		{
			rawSeconds = 0.0f;
		}

		void Add(float seconds)
		{
			rawSeconds += seconds;
		}

		void Reset()
		{
			rawSeconds = 0.0f;
		}

		std::string GetHMS()
		{
			if (rawSeconds <= 0.0f)
				return "";

			int hours = rawSeconds / 3600.0f;
			int minutes = rawSeconds / 60.0f;
			int seconds = 0;

			if (minutes >= 1)
			{
				seconds = fmodf(rawSeconds, (minutes * 60.0f));
			}
			else
			{
				seconds = (int)rawSeconds;
			}

			char buffer[256];
			sprintf_s(buffer, 256, "%d : %d : %d"
				, hours
				, minutes
				, seconds
				);

			return std::string(buffer);
		}

		float rawSeconds;
	};


	template <typename T1, typename T2>
	struct less_second {
		typedef std::pair<T1, T2> type;
		bool operator ()(type const& a, type const& b) const {
			return a.second > b.second;
		}
	};
}


CircularBuffer<100> testSeries_01;


void drawText(float x, float y, const std::string& text)
{
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

void drawSeries()
{


	for (int i = 0; i < testSeries_01.GetSize(); i++)
	{

	}



}

void drawFunc()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	drawSeries();

	//glColor3f(0.1, 1, 0.2);
	//glBegin(GL_LINES);
	//glVertex2f(-1.0, 1.0);
	//glVertex2f(1.0, -1.0);
	//glVertex2f(-0.5, -0.5);
	//glVertex2f(0.5, 0.5);
	//glVertex2f(0.0, 0.0);
	//glVertex2f(0.1, 0.4);
	//glEnd();

	//drawText(0, 0, "HELP");

	glutSwapBuffers();
}



void update(int value)
{
	glutTimerFunc(UPDATE_INTERVAL, update, 0);


	testSeries_01.Add((rand() % 60) + 1);
	//avgLuaScriptFuncCalls.Add(GLOBALS->m_luaScriptFuncCallCount);
	//avgLuaCFuncCalls.Add(GLOBALS->m_luaCFuncCallCount);


	glutPostRedisplay();
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("Some3dApp");

	glutDisplayFunc(drawFunc);
	glutTimerFunc(UPDATE_INTERVAL, update, 0);

	glViewport(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutMainLoop();

	return 0;
}