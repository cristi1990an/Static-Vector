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
	static_vector<std::string, 10> vec = { "Cristi", "este", "foarte", "tare" };

	std::for_each(vec.crbegin(), vec.crend(), [](const auto& word) {std::cout << word << ' '; });

}
