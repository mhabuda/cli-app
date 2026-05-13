#pragma once
#include <string>
#include "MetricStore.h"
struct Storage
{
	bool save(MetricStore& store, const std::string filename) const;
	void load(MetricStore& store, const std::string filename) const;
	bool clear(const std::string filename) const;
};

