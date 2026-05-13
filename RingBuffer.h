#pragma once
#include <iostream>
#include <array>
#include <memory>
#include <type_traits>

template <typename T>
concept Storable = std::copyable<T>;

template <Storable Data, size_t Capacity>
	requires (Capacity > 0)
class RingBuffer {
	std::array<Data, Capacity> list{};
	size_t size_ = 0;
	size_t head_ = 0;

public:

	void push(Data data_) {
		list[head_] = data_;
		head_ = (head_ + 1) % Capacity;
		if (size_ < Capacity) size_++;
	}
	std::array<Data, Capacity> getList() const { return list; }
	size_t getSize() const { return size_; }
	size_t getHead() const { return head_; }
};