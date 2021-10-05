#include <string>
#include <set>
#include <map>
#include <any>
#include <variant>
#include <optional>
#include <iostream>
#include "inc/static_vector.hpp"

int main()
{
	static_vector<std::string, 10> vec =
	{ "Cristi", "nu", "stie", "sa", "programeze" };

	auto it = vec.erase(vec.begin() + 1, vec.begin() + 2);

	for (const auto& word : vec)
	{
		std::cout << word << ' ';
	}
	std::cout << '\n';

	std::cout << vec.size() << '\n';

	std::cout << *it;
}
