#pragma once

template <typename T>
class KPoint
{
public:
    KPoint(size_t K);
private:
	std::vector<T> coordinates;
};

