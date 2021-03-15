#pragma once

#include <string>
#include <ctime>

#include "Engine/Time.h"
#include "Engine/Log.h"

struct GameDate
{
	const int m_MonthsInYear = 12;
	size_t m_DaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	char* m_MonthName[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	size_t m_Day = 1;
	size_t month = 0;
	size_t year = 1100;

	float timeBeforeAddingDay = 0.3f;
	float addDayTimer = 0.3f;


	void Update()
	{
		addDayTimer -= Time::DeltaTime();

		if (addDayTimer <= 0.0f)
		{
			ProgressTime();
			addDayTimer = timeBeforeAddingDay;
		}
	}

	void ProgressTime()
	{
		++m_Day;

		if (m_Day > m_DaysInMonth[month])
		{
			m_Day = 1;
			++month;
			if (month >= m_MonthsInYear)
			{
				month = 0;
				++year;
			}
		}
	}

	std::string GetDateString()
	{
		return std::to_string(m_Day) + " " + m_MonthName[month] + " " + std::to_string(year);
	}
};