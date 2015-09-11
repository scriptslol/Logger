#include "LoggerDLL.h"

int main()
{
	LoggerDLL log;

	for (int i = 0; i < 100; i++)
	{
		log.LogVariable("FPS", (rand() % 60) + 1);
		log.LogVariable("Memory", (rand() % 5) + 20);
		Sleep(10);
	}

	log.GenerateLogs();

	return 0;
}