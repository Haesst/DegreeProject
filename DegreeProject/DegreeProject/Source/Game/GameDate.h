#pragma once

#include <string>
#include <ctime>

#include "Engine/Log.h"
#include "Engine/Time.h"

struct Date
{
	size_t m_Day;
	size_t m_Month;
	size_t m_Year;

	Date(size_t day, size_t month, size_t year)
		: m_Day(day), m_Month(month), m_Year(year)
	{}

	inline bool operator < (const Date& other) const
	{
		if (m_Year < other.m_Year)
		{
			return true;
		}

		if (m_Month < other.m_Month)
		{
			return true;
		}

		if (m_Day < other.m_Day)
		{
			return true;
		}

		return false;
	}

	inline void operator = (const Date& other)
	{
		m_Year = other.m_Year;
		m_Month = other.m_Month;
		m_Day = other.m_Day;
	}
};

struct GameDate
{
	const int m_MonthsInYear = 12;
	size_t m_DaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	char* m_MonthName[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	Date m_Date = { 1, 0, 1100 };

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
		++m_Date.m_Day;

		if (m_Date.m_Day > m_DaysInMonth[m_Date.m_Month])
		{
			m_Date.m_Day = 1;
			++m_Date.m_Month;
			if (m_Date.m_Month >= (size_t)m_MonthsInYear)
			{
				m_Date.m_Month = 0;
				++m_Date.m_Year;
			}
		}
	}

	std::string GetDateString()
	{
		return std::to_string(m_Date.m_Day) + " " + m_MonthName[m_Date.m_Month] + " " + std::to_string(m_Date.m_Year);
	}
};