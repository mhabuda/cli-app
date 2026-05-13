#include "DataPoint.h"

DataPoint::DataPoint(double value_)
	: value(value_), creationTime(std::chrono::system_clock::now()) { }

DataPoint::DataPoint(double value_, std::chrono::system_clock::time_point time)
	: value(value_), creationTime(time) { }

double DataPoint::getValue() const { return value; }

std::chrono::system_clock::time_point DataPoint::getCreationTime() const { return creationTime; }
