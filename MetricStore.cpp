#include "MetricStore.h"
#include "Storage.h"
#include <chrono>

bool MetricStore::insert(std::string name, double value) {
	Storage storage;
	DataPoint dp(value);
	metrics[name].push(dp);
	return storage.save(*this, "metrics.dat");
}

bool MetricStore::queryLast(long long amount, char unit) {
	Storage storage;
	storage.load(*this, "metrics.dat");
	auto now = std::chrono::system_clock::now();
	for (const auto& [name, buffer] : metrics) {
		for (size_t i = 0; i < buffer.getSize(); ++i) {
			size_t idx = (buffer.getHead() - buffer.getSize() + i + buffer.getList().size()) % buffer.getList().size();
			auto dp = buffer.getList()[idx];
			auto diff = now - dp.getCreationTime();
			long long diffInUnits = 0;
			switch (unit) {
				case 's': diffInUnits = std::chrono::duration_cast<std::chrono::seconds>(diff).count(); break;
				case 'm': diffInUnits = std::chrono::duration_cast<std::chrono::minutes>(diff).count(); break;
				case 'h': diffInUnits = std::chrono::duration_cast<std::chrono::hours>(diff).count(); break;
			}
			if (diffInUnits < amount)
				std::cout << name << " " << dp.getValue() << "\n";
		}
	}
	return true;
}

bool MetricStore::query(std::string name) {
	Storage storage;
	storage.load(*this, "metrics.dat");
	auto it = metrics.find(name);
	if (it == metrics.end()) {
		std::cerr << "No metric found with name: " << name << "\n";
		return false;
	}
	std::cout << "Metric: " << name << std::endl;
	for (size_t i = 0; i < it->second.getSize(); ++i) {
		std::cout << "Data Point: " << it->second.getList().at(i) << std::endl;
	}
	return true;
}