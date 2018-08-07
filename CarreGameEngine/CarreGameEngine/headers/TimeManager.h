#pragma once

#include <iostream>
#include <chrono>													
#include <thread>													

	/**
	* @class TimeManager
	* @brief Time manager class used to manage anything that uses time
	*
	* This class uses a singleton to calculate the framerate and provide time calculations.
	*
	* @date 29/03/2018
	*
	*/
class TimeManager
{
public:

		/**
		* @brief Singleton instance
		*
		* This function returns a singleton instance of the time class so that there is only
		* one instance of time.
		*
		* @return static TimeManager&
		*/
	static TimeManager& Instance()
	{
		static TimeManager instance;
		
		return instance;
	}

		/**
		* @brief Calculates the framerate
		*
		* This function calculates the framerate per second and prints it to the console.
		*
		* @param bool writeToConsole
		* @return double
		*/
	double CalculateFrameRate(bool writeToConsole);

		/**
		* @brief Returns the current world time
		*
		* This function returns the real time in seconds.
		*
		* @return double
		*/
	double TimeManager::GetTime();

		/**
		* @brief Sleeps the current thread
		*
		* This function sleeps the current thread for an amount of time given by the
		* parameter in milliseconds.
		*
		* @param int milliseconds
		* @return void
		*/
	void Sleep(int milliseconds);

	/// Stores the change in time since the last frame
	double DeltaTime = 0;

	/// Stores the current time in seconds
	double CurrentTime = 0;

private:
		/**
		* @brief Default constructor
		*
		* This is the default constuctor that is empyty and private so it cannot be accessed.
		*
		* @return null
		*/
	TimeManager() { }

		/**
		* @brief Copy constructor
		*
		* This is the copy constuctor and is private so it cannot be accessed.
		*
		* @return null
		*/
	TimeManager(TimeManager const&);

		/**
		* @brief Overloaded assignment operator
		*
		* This is the overloaded assignment operator.
		*
		* @param TimeManager const&
		* @return TimeManager&
		*/
	TimeManager& operator=(TimeManager const&);
};
