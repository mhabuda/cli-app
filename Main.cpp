#include <iostream>
#include "DataPoint.h"
#include "RingBuffer.h"
#include "MetricStore.h"
#include <thread>
#include <string>
#include "Storage.h"

int main(int argc, char* argv[])
{
	Storage storage;
	MetricStore store;

	auto it = argv + 1;
	while (it != argv + argc) {
		std::string arg = std::string(*it);
		if (arg == "--last") {
			if (it + 1 >= argv + argc) {
				std::cerr << "Invalid arguments for --last. Usage: ./main --last <duration>\n";
				return 1;
			}
			std::string opt = std::string(*(it + 1));

			if (opt.empty()) {
				std::cerr << "Duration cannot be empty.\n";
				return 1;
			}

			char unit = opt.back();
			if (unit != 'm' && unit != 'h' && unit != 's') {
				std::cerr << "Invalid unit. Use s, m, or h.\n";
				return 1;
			}

			long long amount;
			try {
				amount = std::stoll(opt.substr(0, opt.size() - 1));
			}
			catch (...) {
				std::cerr << "Invalid duration value.\n";
				return 1;
			}

			if (!store.queryLast(amount, unit))
				return 1;
			it += 2;
		}
		else if (arg == "insert") {
			if (it + 2 >= argv + argc) {
				std::cerr << "Invalid arguments for insert. Usage: ./main insert <metric> <value>\n";
				return 1;
			}
			std::string buffer = std::string(*(it + 1));
			if (buffer.empty()) {
				std::cerr << "Metric name cannot be empty.\n";
				return 1;
			}
			try {
				double data = std::stod(std::string(*(it + 2)));
				if (!store.insert(buffer, data))
					return 1;
			}
			catch (const std::invalid_argument&) {
				std::cerr << "Invalid value for insert: not a number.\n";
				return 1;
			}
			catch (const std::out_of_range&) {
				std::cerr << "Invalid value for insert: out of double range.\n";
				return 1;
			}
			it += 3;
		}
		else if (arg == "query") {
			if (it + 1 >= argv + argc) {
				std::cerr << "Invalid arguments for query. Usage: ./main query <metric>\n";
				return 1;
			}
			std::string buffer = std::string(*(it + 1));
			if (!store.query(buffer))
				return 1;
			it += 2;
		}
		else if (arg == "clear") {
			if (!storage.clear("metrics.dat"))
				return 1;
			it += 1;
		}
		else {
			std::cerr << "Invalid arguments. Usage: ./main query/insert/clear <metric> <value>\n";
			return 1;
		}
	}
}


