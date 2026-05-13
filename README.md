# cli-app — minimal time-series CLI

A lightweight command-line tool for storing and querying named metrics 
over time. Built as a systems programming exercise in modern C++20.

## What it does

Insert named metrics with a timestamp and query them back, optionally 
filtering by a time window.

## Build

Requirements: CMake 3.20+, GCC or Clang with C++20 support

```bash
git clone https://github.com/mhabuda/cli-app
cd cli-app
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
# Insert values
./Main insert cpu_usage 87.3
./Main insert cpu_usage 54.1
./Main insert memory 42.0

# Query all values for a metric
./Main query cpu_usage

# Query with time filter
./Main query cpu_usage --last 10m
./Main query cpu_usage --last 1h
./Main query cpu_usage --last 30s

# Chain commands in one run
./Main insert cpu_usage 87.3 insert memory 42.0 query cpu_usage
```

## Architecture

**`RingBuffer<T, Capacity>`** — fixed-size circular buffer backed by 
`std::array`. When full, overwrites the oldest entry rather than 
growing. Chosen over `std::vector` or `std::deque` because random 
access by index is simpler and the memory layout is contiguous and 
cache-friendly. Constrained with C++20 concepts.

**`MetricStore`** — owns an `std::unordered_map` of metric name to 
`RingBuffer`. Guarded by `std::shared_mutex` — multiple readers can 
query simultaneously while writes are exclusive. The mutex is kept even 
though the final CLI is single-threaded, as a foundation for a future 
daemon mode.

**`Storage`** — handles persistence to a flat text file using a 
length-prefix format: each metric entry starts with its name and 
datapoint count, followed by that many value/timestamp pairs. This 
avoids ambiguity when parsing and supports metric names that look like 
numbers.

**`DataPoint`** — holds a `double` value and a 
`std::chrono::system_clock::time_point` timestamp set at insertion time.

## Design decisions

- Templates used for `RingBuffer` only — domain classes like 
  `MetricStore` are not templated since they always store `DataPoint`. 
  Over-templating adds noise without benefit.
- File persistence overwrites on every save (`std::ios::trunc`) to keep 
  the format simple and avoid duplicate entries.
- CLI parsing uses raw `argv` pointer arithmetic rather than a library — 
  keeps dependencies at zero.

## Future work

- Aggregations: avg, min, max, count over query results
- Delta-of-delta compression on the binary file format
- HTTP endpoint for Grafana compatibility
- Lock-free ingestion queue for high-throughput daemon mode
