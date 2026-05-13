#pragma once
#include <chrono>
#include <ostream>

class DataPoint
{
	double value = 0.0;
	std::chrono::system_clock::time_point creationTime = std::chrono::system_clock::now();
public:
	DataPoint() = default;
	DataPoint(double value);
	DataPoint(double value, std::chrono::system_clock::time_point time);
	double getValue() const;
	std::chrono::system_clock::time_point getCreationTime() const;
	friend std::ostream& operator<<(std::ostream& os, const DataPoint& dp) {
		os << "Value: " << dp.getValue() << ", Creation Time: " << dp.getCreationTime().time_since_epoch().count() << "ms";
		return os;
	}
};

std::ostream& operator<<(std::ostream& os, const DataPoint& dp);
