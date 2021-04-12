#pragma once

#include <string>
#include <ctime>

#include "Engine/Log.h"
#include "Engine/Time.h"
#include "Game/Callback.h"

struct Date
{
	size_t m_Day;
	size_t m_Month;
	size_t m_Year;

	Date(size_t day, size_t month, size_t year)
		: m_Day(day), m_Month(month), m_Year(year)
	{}

	Date()
		: m_Day(0), m_Month(0), m_Year(0)
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

	inline bool operator == (const Date& other)
	{
		return m_Year == other.m_Year && m_Month == other.m_Month && m_Day == other.m_Day;
	}
};

struct GameDate
{
	const int m_MonthsInYear = 12;
	size_t m_DaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int m_DaysInAYear = 0;
	char* m_MonthName[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	const Date m_StartDate = { 1, 0, 1100 };
	Date m_Date = { 1, 0, 1100 };

	float timeBeforeAddingDay = 0.3f;
	float addDayTimer = 0.3f;

	std::vector<Callback> m_DayChangeSubscribers;
	std::vector<std::function<void(Date)>> m_MonthChangeSubscribers;

	GameDate();

	void update();

	void progressTime();

	Date getRandomDate(bool futureDate, size_t minYearDifference, size_t maxYearDifference);

	char* getDaySuffix();

	std::string getDateString();

	unsigned int getAge(const Date& birthday);

	unsigned int getDaysBetweenDates(const Date& lhs, const Date& rhs);

	void subscribeToMonthChange(std::function<void(Date)> action);

	int subscribeToDayChange(CallbackMember* callback, void* data);

	void unsubscribeToDayChange(unsigned int id);
};