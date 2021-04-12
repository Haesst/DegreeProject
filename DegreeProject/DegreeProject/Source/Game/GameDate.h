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

	GameDate()
	{
		for (auto& days : m_DaysInMonth)
		{
			m_DaysInAYear += days;
		}
	}

	void update()
	{
		addDayTimer -= Time::deltaTime();

		if (addDayTimer <= 0.0f)
		{
			progressTime();
			addDayTimer = timeBeforeAddingDay;
		}
	}

	void progressTime()
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

			for (auto& action : m_MonthChangeSubscribers)
			{
				action(m_Date);
			}
		}

		for (auto& action : m_DayChangeSubscribers)
		{
			//action(m_Date);
			action.m_Action(action.data);
		}
	}

	Date getRandomDate(bool futureDate, size_t minYearDifference, size_t maxYearDifference)
	{
		size_t yearRange = maxYearDifference - minYearDifference + 1;
		size_t yearDiff = std::rand() % yearRange + minYearDifference;

		size_t month = std::rand() % m_MonthsInYear + 1;
		
		size_t day = std::rand() % m_DaysInMonth[month - 1] + 1;

		size_t year = futureDate ? m_StartDate.m_Year + yearDiff : m_StartDate.m_Year - yearDiff;

		return Date(day, month, year);
	}

	char* getDaySuffix()
	{
		int lastDayDigit = m_Date.m_Day % 10;

		switch (lastDayDigit)
		{
		case 1: return "st";
		case 2: return "nd";
		case 3: return "rd";
		default:
			return "th";
		}
	}

	std::string getDateString()
	{
		return std::to_string(m_Date.m_Day) + getDaySuffix() + " of " + m_MonthName[m_Date.m_Month] + ". " + std::to_string(m_Date.m_Year) + " AD";
	}

	unsigned int getAge(const Date& birthday)
	{
		if ((m_Date.m_Month >= birthday.m_Month && m_Date.m_Day >= birthday.m_Day) || m_Date.m_Month > birthday.m_Month)
		{
			return m_Date.m_Year - birthday.m_Year;
		}
		else
		{
			return m_Date.m_Year - birthday.m_Year - 1;
		}
	}

	unsigned int getDaysBetweenDates(const Date& lhs, const Date& rhs)
	{
		long int n1 = lhs.m_Year * 365 + lhs.m_Day;

		for (size_t i = 0; i < lhs.m_Month; ++i)
		{
			n1 += m_DaysInMonth[i];
		}

		long int n2 = rhs.m_Year * 365 + rhs.m_Day;

		for (size_t i = 0; i < rhs.m_Month; ++i)
		{
			n2 += m_DaysInMonth[i];
		}

		return std::abs(n2 - n1);
	}

	void subscribeToMonthChange(std::function<void(Date)> action)
	{
		m_MonthChangeSubscribers.push_back(action);
	}

	int subscribeToDayChange(CallbackMember* callback, void* data)
	{
		Callback cb = Callback(callback, data);
		m_DayChangeSubscribers.push_back(cb);
		return cb.id;
	}

	void unsubscribeToDayChange(unsigned int id)
	{
		int index = 0;
		bool found = false;

		for (; index < m_MonthsInYear; ++index)
		{
			if (m_DayChangeSubscribers[index].id == id)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			m_DayChangeSubscribers.erase(std::next(m_DayChangeSubscribers.begin(), index));
		}
	}
};