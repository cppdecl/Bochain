#ifndef BOCHAIN_UTILS_CLOCK_HH
#define BOCHAIN_UTILS_CLOCK_HH

#include "Types.hh"

#include <chrono>
#include <ctime>
#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Duration	= std::chrono::high_resolution_clock::duration;

namespace Clock
{
	inline auto Now() -> TimePoint
	{
		return std::chrono::high_resolution_clock::now();
	}

	inline auto ElapsedTime(const TimePoint& start, const TimePoint& end) -> Duration
	{
		return end - start;
	}

	inline auto GetSecondsSinceEpoch() -> i32
	{
		return std::chrono::duration_cast<std::chrono::seconds>(Now().time_since_epoch()).count();
	}

	inline auto TimePointToEpoch(TimePoint timePoint) -> i32
	{
		return std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();
	}

	inline auto EpochToTimePoint(i32 secondsSinceEpoch) -> TimePoint
	{
		TimePoint dt{std::chrono::seconds(secondsSinceEpoch)};
		return dt;
	}

	inline auto TimePointToEpochMilli(TimePoint timePoint) -> u64
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch()).count();
	}

	inline auto EpochMilliToTimePoint(i64 millisecondsSinceEpoch) -> TimePoint
	{
		TimePoint dt{std::chrono::milliseconds(millisecondsSinceEpoch)};
		return dt;
	}

	inline auto DiffNano(const TimePoint& start) -> long long
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(ElapsedTime(start, Now())).count();
	}

	inline auto DiffMicro(const TimePoint& start) -> long long
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(ElapsedTime(start, Now())).count();
	}

	inline auto DiffMilli(const TimePoint& start) -> long long
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime(start, Now())).count();
	}

	inline auto DiffSec(const TimePoint& start) -> double
	{
		return std::chrono::duration_cast<std::chrono::duration<double>>(ElapsedTime(start, Now())).count();
	}

	inline auto DiffMin(const TimePoint& start) -> double
	{
		return DiffSec(start) / 60.0;
	}

	inline auto DiffHour(const TimePoint& start) -> double
	{
		return DiffSec(start) / 3600.0;
	}

	inline auto DiffDay(const TimePoint& start) -> double
	{
		return DiffSec(start) / 86400.0;
	}

	inline auto DiffWeek(const TimePoint& start) -> double
	{
		return DiffSec(start) / 604800.0;
	}

	inline auto DiffMonth(const TimePoint& start) -> double
	{
		return DiffSec(start) / 2592000.0;
	}

	inline auto DiffYear(const TimePoint& start) -> double
	{
		return DiffSec(start) / 31536000.0;
	}

	inline auto TimePointToTimeStamp(const std::chrono::system_clock::time_point& timePoint) -> std::string
	{
		std::time_t time_tValue = std::chrono::system_clock::to_time_t(timePoint);

		std::tm* localTime = std::localtime(&time_tValue);

		std::stringstream ss;
		ss << std::put_time(localTime, "%B %d, %Y - %l:%M %p");

		return ss.str();
	}

	inline auto GetTimestamp() -> std::string
	{
		std::time_t now		= std::time(nullptr);
		std::tm*	curtime = std::localtime(&now);

		static const std::string daysOfWeek[]	= {"Sunday",   "Monday", "Tuesday", "Wednesday",
												   "Thursday", "Friday", "Saturday"};
		static const std::string monthsOfYear[] = {"January",	"February", "March",	"April",
												   "May",		"June",		"July",		"August",
												   "September", "October",	"November", "December"};

		std::stringstream formattedTime;
		formattedTime << std::put_time(curtime, "%A, %B %d, %Y - %I:%M %p");

		return std::format("{}", formattedTime.str());
	}
};

#endif	// BOCHAIN_UTILS_CLOCK_HH