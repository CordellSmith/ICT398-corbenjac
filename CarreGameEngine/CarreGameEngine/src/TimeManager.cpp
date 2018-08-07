#include "..\headers\TimeManager.h"																																	

double TimeManager::CalculateFrameRate(bool writeToConsole = false)
{
	static double framesPerSecond = 0.0f;								
	static double startTime = GetTime();								
	static double lastTime = GetTime();									
	static char strFrameRate[50] = { 0 };								
	static double currentFPS = 0.0f;									

	CurrentTime = GetTime();

	// Calculate delta time
	DeltaTime = CurrentTime - lastTime;
	lastTime = CurrentTime;

	// Increase frame counter
	++framesPerSecond;
	
	if ( CurrentTime - startTime > 0.5f )
	{
		startTime = CurrentTime;

		// Display fps in console
		if (writeToConsole)
			std::cout << "Current Frames Per Second: " << (int)framesPerSecond << std::endl;
	
		currentFPS = framesPerSecond;
		framesPerSecond = 0;
	}

	return currentFPS;
}

double TimeManager::GetTime()
{
	auto beginningOfTime = std::chrono::system_clock::now().time_since_epoch();

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(beginningOfTime).count();

	return ms * 0.001;
}

void TimeManager::Sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}