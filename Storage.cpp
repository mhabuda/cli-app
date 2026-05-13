#include "Storage.h"
#include <fstream>
#include <ostream>
#include "RingBuffer.h"
#include <chrono>

bool Storage::save(MetricStore& store, const std::string filename) const
{
	std::ofstream metricStoreFile;
	metricStoreFile.open(filename, std::ios_base::app);
	if (!metricStoreFile.is_open()) {
		std::cerr << "Failed to open file for writing: " << filename << " (permission denied?)\n";
		return false;
	}
	const auto& storeMetrics = store.getMetrics();

	for (const auto& [name, buffer] : storeMetrics) {
		metricStoreFile << name << " " << buffer.getSize() << std::endl;

		for (size_t i = 0; i < buffer.getSize(); ++i) {
			size_t idx = (buffer.getHead() - buffer.getSize() + i + buffer.getList().size()) % buffer.getList().size();
			auto dataPoint = buffer.getList()[idx];
			auto ms =
				std::chrono::duration_cast<std::chrono::milliseconds>(
					dataPoint.getCreationTime().time_since_epoch()
				).count();
			metricStoreFile << dataPoint.getValue() << " " << ms << std::endl;
		}
	}
	metricStoreFile.close();
	if (metricStoreFile.fail()) {
		std::cerr << "Failed to write to file: " << filename << "\n";
		return false;
	}
	return true;
}

bool Storage::clear(const std::string filename) const
{
	std::ofstream metricStoreFile(filename, std::ios::trunc);
	if (!metricStoreFile.is_open()) {
		std::cerr << "Failed to clear file: " << filename << " (permission denied?)\n";
		return false;
	}
	return true;
}

void Storage::load(MetricStore& store, const std::string filename) const
{
	std::ifstream metricStoreFile;
	metricStoreFile.open(filename);
	if (!metricStoreFile.is_open())
		return;

	std::string metricName;
	size_t count;
	while (metricStoreFile >> metricName >> count) {
		for (size_t i = 0; i < count; ++i) {
			double value;
			long long ms;
			metricStoreFile >> value >> ms;
			auto tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(ms));
			store.getMetrics()[metricName].push(DataPoint(value, tp));
		}
	}
	metricStoreFile.close();
}
