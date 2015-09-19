
#if __EMSCRIPTEN__
#include <stdio.h>
#include <stdlib.h>
//#include <GL/glut.h>
//#include <EGL/egl.h>
//#include <emscripten.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <iostream>
#include <string>


#include "oglPlot.h"

//#pragma comment(lib, "OpenGL32.lib")

#define WINDOW_WIDTH 800.0f
#define WINDOW_HEIGHT 300.0f

// 1000.0 / 60.0
//#define UPDATE_INTERVAL 16
#define UPDATE_INTERVAL 300

typedef unsigned long long uint64;

bool quit = false;



//struct UserData
//{
//	GLuint programObject;
//};

//namespace
//{
//	template<size_t N>
//	struct CircularBuffer
//	{
//		CircularBuffer()
//		{
//			index = 0;
//			memset(buffer, 0, sizeof(buffer));
//		}
//
//		uint64 operator[](int i)
//		{
//			return buffer[i];
//		}
//
//		size_t GetSize()
//		{
//			return N;
//		}
//
//		void Add(uint64 n)
//		{
//			buffer[index] = n;
//			index++;
//
//			if (index >= N)
//				index = 0;
//		}
//
//		uint64 GetAverage()
//		{
//			uint64 accum = 0l;
//			for (int i = 0; i < N; i++)
//				accum += buffer[i];
//			accum /= N;
//			return accum;
//		}
//
//		uint64 GetPeak()
//		{
//			uint64 peak = 0l;
//			for (int i = 0; i < N; i++)
//				if (buffer[i] > peak)
//					peak = buffer[i];
//			return peak;
//		}
//
//		size_t index;
//		uint64 buffer[N];
//	};
//
//	struct ReadableClock
//	{
//		ReadableClock()
//		{
//			rawSeconds = 0.0f;
//		}
//
//		void Add(float seconds)
//		{
//			rawSeconds += seconds;
//		}
//
//		void Reset()
//		{
//			rawSeconds = 0.0f;
//		}
//
//		std::string GetHMS()
//		{
//			if (rawSeconds <= 0.0f)
//				return "";
//
//			int hours = rawSeconds / 3600.0f;
//			int minutes = rawSeconds / 60.0f;
//			int seconds = 0;
//
//			if (minutes >= 1)
//			{
//				seconds = fmodf(rawSeconds, (minutes * 60.0f));
//			}
//			else
//			{
//				seconds = (int)rawSeconds;
//			}
//
//			char buffer[256];
//			sprintf_s(buffer, 256, "%d : %d : %d"
//				, hours
//				, minutes
//				, seconds
//				);
//
//			return std::string(buffer);
//		}
//
//		float rawSeconds;
//	};
//
//
//	template <typename T1, typename T2>
//	struct less_second {
//		typedef std::pair<T1, T2> type;
//		bool operator ()(type const& a, type const& b) const {
//			return a.second > b.second;
//		}
//	};
//}


//CircularBuffer<100> testSeries_01;

oglPlot plot;

void drawText(float x, float y, const std::string& text)
{
	//glRasterPos2f(x, y);
	//glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

//void drawSeries()
//{
//
//
//	for (int i = 0; i < testSeries_01.GetSize(); i++)
//	{
//
//	}
//
//
//}

void drawFunc()
//void drawFunc(ESContext *esContext)
{
	//glClearColor(0, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);

	//glLoadIdentity();

	//drawSeries();

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

	plot.Draw();

	//glutSwapBuffers();
	//glDrawArrays(GL_L)
}



void update(int value)
{
	//glutTimerFunc(UPDATE_INTERVAL, update, 0);

	real r = (rand() % 1000) + 500;
	plot.AddPoint(0, TimeUtility::GetTime(), r);

	//glutPostRedisplay();
}

//void updateFunc(ESContext *esContext, float v)
//{
//	real r = (rand() % 1000) + 500;
//	plot.AddPoint(0, TimeUtility::GetTime(), r);
//}

void ApplyOrtho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
	float a = 2.0f / (right - left);
	float b = 2.0f / (top - bottom);
	float c = -2.0f / (farZ - nearZ);

	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(farZ + nearZ) / (farZ - nearZ);

	float ortho[16] = {
		a, 0, 0, tx,
		0, b, 0, ty,
		0, 0, c, tz,
		0, 0, 0, 1
	};

	//GLint projectionUniform = glGetUniformLocation(_shaderProgram, "Projection");
	//glUniformMatrix4fv(projectionUniform, 1, 0, &ortho[0]);


	mat4 projectionMatrix = mat4(
		2.0 / WINDOW_HEIGHT, 0.0, 0.0, -1.0,
		0.0, 2.0 / WINDOW_WIDTH, 0.0, -1.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	
	//glGetUniformLocation(0, "Projection");
}



int init(SDL_Window** window, SDL_Renderer** renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Fail
		return 1;
	}

	//auto start_time = SDL_GetTicks();

	*window = SDL_CreateWindow("Logger",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	//SDL_GetWindowr

	if (window == NULL)
	{
		// fail
		return 1;
	}

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef USE_LEGACY_MANUAL
	mainContext = SDL_GL_CreateContext(*window);

	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	GLfloat matrixData[] = { 2.0 / WINDOW_WIDTH, 0, 0, 0,
		0, -2.0 / WINDOW_HEIGHT, 0, 0,
		0, 0, -1, 0,
		-1, 1, 0, 1 };
	glLoadMatrixf(matrixData); // test loadmatrix

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#else
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
#endif
	
	//SDL_GL_SetSwapInterval(1);

	//glewExperimental = GL_TRUE;
	//glewInit();

	if (renderer == NULL)
	{
		// Fail
		return 1;
	}

	//SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);

#ifndef EMSCRIPTEN
	//int imgFlags = IMG_INIT_PNG;
	//if (!(IMG_Init(imgFlags) & imgFlags))
	//{
	//	// Fail
	//	return 1;
	//}
#endif
	return 0;
}

void loop(void)
{
	static size_t lastTicks = 0;
	static SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		// Window Events
		if(event.type == SDL_QUIT)
		{
			quit = true;
		}

		// Input Events
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
		{
			switch (event.key.keysym.sym)
			{
			default:
				break;
			}
		}
		else if (event.type == SDL_KEYUP && event.key.repeat == 0)
		{
			switch (event.key.keysym.sym)
			{
			default:
				break;
			}
		}
	}

	if (SDL_GetTicks() - lastTicks >= UPDATE_INTERVAL)
	{
		lastTicks += UPDATE_INTERVAL;

		// Logic		
		real r = (rand() % 1000) + 500;
		//std::cout << "Value: " << std::to_string(r) << std::endl;
		plot.AddPoint(0, TimeUtility::GetTime(), r);

		// Render
#ifdef USE_LEGACY_MANUAL
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
#else
		SDL_SetRenderDrawColor(plot.renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(plot.renderer);
#endif

		//////////////////////////////////////////////////////////////////////////
		//SDL_SetRenderDrawColor(plot.renderer, 0xFF, 0x00, 0x00, 0xFF);
		//SDL_RenderDrawLine(renderer, 343.0, 250.0, 343.0, 263.0);
		//////////////////////////////////////////////////////////////////////////
		//glBegin(GL_LINES);
		//glColor3f(1.0, 0.0, 0.0);
		//glVertex2f(0.0, 0.0);
		//glVertex2f(10.0, 10.0);
		//glEnd();
		//////////////////////////////////////////////////////////////////////////

		plot.Draw();

#ifdef USE_LEGACY_MANUAL
		SDL_GL_SwapWindow(window);
#else
		SDL_RenderPresent(plot.renderer);
#endif
	}
	// Other events...

#ifdef EMSCRIPTEN
	//emscripten_cancel_main_loop();
#endif
}

#ifdef __EMSCRIPTEN__
int main(int argc, char* argv[])
{
#else
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	int argc = __argc;
	char** argv = __argv;
#endif

	//printf("TEST\n");

//#ifdef __EMSCRIPTEN__
//	printf("TEST");
//	emscripten_set_main_loop(loop, 0, 0);
//#endif

#ifdef __EMSCRIPTEN__
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &plot.renderer);
#else
	if (init(&window, &plot.renderer) == 1)
	{
		return 1;
	}
#endif



	//printf("TEST\n");
	//screen_surface = SDL_GetWindowSurface(window);

	plot.Create(vec2(10, 290), vec2(400, 50));
	plot.SetSeries(0, 0, vec3(1.0, 0.0, 0.0), 0.0, 60.0, "Test");
	plot.m_autoScrollX = true;

#ifdef __EMSCRIPTEN__
	//printf("TEST\n");
	//emscripten_cancel_main_loop();
	emscripten_set_main_loop(loop, 0, 1);
#else
	while (!quit)
	{
		loop();
	}
#endif

//	printf("TEST");
//#ifndef __EMSCRIPTEN__
//	while (!quit)
//	{
//		loop();
//	}
//#endif
	
	//SDL_GL_DeleteContext(mainContext);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}








//#ifdef _WIN32
//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//	int argc = __argc;
//	char** argv = __argv;
//#elif __EMSCRIPTEN__
//int main(int argc, char* argv[])
//{
//#else
//int main(int argc, char* argv[])
//{
//#endif
//
//	//plot.Create(vec2(10, 290), vec2(400, 50));
//	//plot.SetSeries(0, 0, vec3(1.0, 0.0, 0.0), 0.0, 60.0, "Test");
//	//plot.m_autoScrollX = true;
//
//	//ESContext esContext;
//	//UserData userData;
//
//	//esInitContext(&esContext);
//	//esContext.userData = &userData;
//
//	//esCreateWindow(&esContext, "Hello Triangle", WINDOW_WIDTH, WINDOW_HEIGHT, ES_WINDOW_RGB);
//
//	//esRegisterDrawFunc(&esContext, drawFunc);
//	//esRegisterUpdateFunc(&esContext, updateFunc);
//
//	//esMainLoop(&esContext);
//
//
//	//glutInit(&argc, argv);
//	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//	//glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//
//	//glutCreateWindow("Some3dApp");
//
//	//glutDisplayFunc(drawFunc);
//	//glutTimerFunc(UPDATE_INTERVAL, update, 0);
//
//	//glViewport(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
//	//glMatrixMode(GL_PROJECTION);
//	//glLoadIdentity();
//
//	//glOrtho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.0f, 1.0f);
//
//	//glMatrixMode(GL_MODELVIEW);
//	//glLoadIdentity();
//
//	//glutMainLoop();
//
//	return 0;
//}