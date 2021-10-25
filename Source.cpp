#include <string>
#include <set>
#include <map>
#include <list>
#include <any>
#include <variant>
#include <optional>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <vector>
#include "inc/static_vector.hpp"

int main()
{
	static_vector<int, 100> vec;

	for (int i = 1; i <= 50; i++)
	{
		vec.push_back(i);
	}

	for (const auto val : vec)
	{
		std::cout << val << ' ';
	}
	std::cout << '\n';
}
