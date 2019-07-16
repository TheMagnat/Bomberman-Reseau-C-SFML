#pragma once

#include <cstring>
#include <vector>

class StringToVec {
public:

	StringToVec(const char * data);

	int takeElement();

public:
	std::vector<int> data_;
	size_t size_;
	size_t itte_;
};