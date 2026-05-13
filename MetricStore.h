#pragma once
#include "DataPoint.h"
#include "RingBuffer.h"
#include <unordered_map>

class MetricStore
{
	static constexpr size_t MAX_BUFFER_SIZE = 3;
	std::unordered_map<std::string, RingBuffer<DataPoint, MAX_BUFFER_SIZE>> metrics;

public:
	bool insert(std::string name, double value);
	bool query(std::string name);
	bool queryLast(long long amount, char unit);
	std::unordered_map<std::string, RingBuffer<DataPoint, MAX_BUFFER_SIZE>>& getMetrics() { return metrics; }
};

