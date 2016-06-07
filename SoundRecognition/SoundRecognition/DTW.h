#pragma once

#include <vector>
#include <algorithm>

template <typename obj_t, typename distance_t, typename ret_t = double>
ret_t distanceDTW(const std::vector<obj_t> & a, const std::vector<obj_t> & b, distance_t distance) {
	std::vector<ret_t> DTWTable(a.size() * b.size());

	const int as = a.size();
	const int bs = b.size();

	for (int i = 0; i < as; ++i) {
		for (int j = 0; j < bs; ++j) {
			DTWTable[i * bs + j] = distance(a[i], b[j]);
		}
	}


	for (int i = 1; i < bs; ++i) {
		DTWTable[i] += DTWTable[i - 1];
	}

	for (int i = 1; i < as; ++i) {
		DTWTable[i * bs] += DTWTable[i * bs - bs];
	}

	for (int i = 1; i < as; ++i) {
		for (int j = 1; j < bs; ++j) {
			ret_t min = std::min(std::min(DTWTable[i * bs + j - 1], DTWTable[i * bs + j - bs - 1]), DTWTable[i * bs + j - bs]);
			DTWTable[i * bs + j] += min;
		}
	}

	return DTWTable.back();
}